# include "pipeline.hpp"

Pipeline pipeline;

void Pipeline::clear_buffers () {
    uint64_t n = width * height;
    memset (color_buffer, 0, n * sizeof (uint32_t));
    double * depth_buffer_ = reinterpret_cast <double *> (depth_buffer);
    for (uint64_t i = 0; i < n; i ++)
        depth_buffer_ [i] = 1;
}

void Pipeline::draw () {
    Vertex (* vertex_buffer_) [3] = reinterpret_cast <Vertex (*) [3]> (vertex_buffer);
    for (primitive_id = 0; primitive_id < num_primitives; primitive_id ++) {
        for (vertex_id = 0; vertex_id < 3; vertex_id ++) {
            double position [4];
            Vertex vertex = vertex_buffer_ [primitive_id] [vertex_id];
            out [vertex_id] = vertex_shader (vertex, position);
            homogeneous_coordinates [vertex_id] = position [3];
            vec_from_homogeneous (position, positions_3d [vertex_id], 4);
        }
        if (! clipping_test ())
            continue;
        to_plane ();
        to_viewport ();
        rasterize ();
    }
}

bool Pipeline::clipping_test () {
    double clip_space [] [3] = {
        { -1, 0, -1 },
        { 1, 1, 1 }
    };
    bound_points (positions_3d, boundary_3d, 3, 3);
    if (boundary_3d [0] [1] < 0 || 1 < boundary_3d [1] [1])
        return false;
    aabb_intersection (boundary_3d, clip_space, boundary_3d, 3);
    return ! aabb_empty (boundary_3d, 3);
}

void Pipeline::to_plane () {
    for (uint64_t i = 0; i < 3; i ++) {
        positions_2d [i] [0] = positions_3d [i] [0];
        positions_2d [i] [1] = positions_3d [i] [2];
    }
    for (uint64_t i = 0; i < 2; i ++) {
        boundary_2d [i] [0] = boundary_3d [i] [0];
        boundary_2d [i] [1] = boundary_3d [i] [2];
    }
}

void Pipeline::to_viewport () {
    for (uint64_t i = 0; i < 3; i ++) {
        for (uint64_t j = 0; j < 2; j ++)
            positions_2d [i] [j] = dlerp (positions_2d [i] [j], viewport [0] [j], viewport [1] [j]);
    }
    for (uint64_t i = 0; i < 2; i ++) {
        for (uint64_t j = 0; j < 2; j ++) {
            boundary_2d [i] [j] = dlerp (boundary_2d [i] [j], viewport [0] [j], viewport [1] [j]);
            raster_rect [i] [j] = round (boundary_2d [i] [j]);
        }
    }
}

void Pipeline::rasterize () {
    linear_param (positions_2d, positions_2d, 3, 2);
    double alpha = positions_2d [1] [0] * positions_2d [2] [1] - positions_2d [2] [0] * positions_2d [1] [1];
    if (front_face_clockwise) {
        if (alpha >= 0)
            return;
    } else {
        if (alpha <= 0)
            return;
    }
    alpha = - 1 / alpha;
    for (fragment.y = raster_rect [0] [1]; fragment.y <= raster_rect [1] [1]; fragment.y ++) {
        for (fragment.x = raster_rect [0] [0]; fragment.x <= raster_rect [1] [0]; fragment.x ++) {
            double delta [] = { positions_2d [0] [0] - fragment.x, positions_2d [0] [1] - fragment.y };
            fragment.u = alpha * (delta [0] * positions_2d [2] [1] - positions_2d [2] [0] * delta [1]);
            fragment.v = alpha * (positions_2d [1] [0] * delta [1] - delta [0] * positions_2d [1] [1]);
            if (fragment.u >= 0 && fragment.v >= 0 && fragment.u + fragment.v <= 1 && depth_test ()) {
                map_barycentric_coordinates ();
                build_payload ();
                fragment_shader (in, fragment.color);
                blend_fragment ();
            }
        }
    }
}

bool Pipeline::depth_test () {
    double (* depth_buffer_) [width] = reinterpret_cast <double (*) [width]> (depth_buffer);
    fragment.depth = blerp (fragment.u, fragment.v, positions_3d [0] [1], positions_3d [1] [1], positions_3d [2] [1]);
    return 0 <= fragment.depth && fragment.depth <= depth_buffer_ [fragment.y] [fragment.x];
}

void Pipeline::map_barycentric_coordinates () {
    double t = (1 - fragment.u - fragment.v) / homogeneous_coordinates [0];
    double u = fragment.u / homogeneous_coordinates [1];
    double v = fragment.v / homogeneous_coordinates [2];
    double scale = 1 / (t + u + v);
    fragment.u = scale * u;
    fragment.v = scale * v;
}

void Pipeline::build_payload () {
    for (uint64_t i = 0; i < 3; i ++) {
        in.view [i] = blerp (fragment.u, fragment.v, out [0].view [i], out [1].view [i], out [2].view [i]);
        in.normal [i] = blerp (fragment.u, fragment.v, out [0].normal [i], out [1].normal [i], out [2].normal [i]);
        for (uint64_t light = 0; light < NUM_LIGHTS; light ++) {
            if (active_lights & 1lu << light)
                in.light [light] [i] = blerp (fragment.u, fragment.v, out [0].light [light] [i], out [1].light [light] [i], out [2].light [light] [i]);
        }
    }
    for (uint64_t i = 0; i < 2; i ++)
        in.texture [i] = blerp (fragment.u, fragment.v, out [0].texture [i], out [1].texture [i], out [2].texture [i]);
}

void Pipeline::blend_fragment () {
    uint32_t (* color_buffer_) [width] = reinterpret_cast <uint32_t (*) [width]> (color_buffer);
    double (* depth_buffer_) [width] = reinterpret_cast <double (*) [width]> (depth_buffer);
   
    for (uint64_t i = 0; i < 4; i ++)
        fragment.color [i] = clamp (fragment.color [i], 0, 1);
    double a = UINT8_MAX * fragment.color [3];
    double b = 1 - fragment.color [3];
    uint8_t red = round (a * fragment.color [0] + b * qRed (color_buffer_ [fragment.y] [fragment.x]));
    uint8_t green = round (a * fragment.color [1] + b * qGreen (color_buffer_ [fragment.y] [fragment.x]));
    uint8_t blue = round (a * fragment.color [2] + b * qBlue (color_buffer_ [fragment.y] [fragment.x]));
    uint8_t alpha = round (a);
    
    if (invert_y_viewport)
        color_buffer_ [viewport [1] [1] - (fragment.y - viewport [0] [1])] [fragment.x] = qRgba (red, green, blue, alpha);
    else
        color_buffer_ [fragment.y] [fragment.x] = qRgba (red, green, blue, alpha);
    
    if (! depth_write_disabled)
        depth_buffer_ [fragment.y] [fragment.x] = fragment.depth;
}