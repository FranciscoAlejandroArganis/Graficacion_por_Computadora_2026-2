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
        for (vertex_id = 0; vertex_id < 3; vertex_id ++)
            out [vertex_id] = vertex_shader (vertex_buffer_ [primitive_id] [vertex_id], positions_3d [vertex_id]);
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
        positions_2d [i] [0] = dlerp (positions_2d [i] [0], viewport [0] [0], viewport [1] [0]);
        positions_2d [i] [1] = dlerp (positions_2d [i] [1], viewport [1] [1], viewport [0] [1]);
    }
    for (uint64_t i = 0; i < 2; i ++) {
        boundary_2d [i] [0] = dlerp (boundary_2d [i] [0], viewport [0] [0], viewport [1] [0]);
        boundary_2d [i] [1] = dlerp (boundary_2d [i] [1], viewport [1] [1], viewport [0] [1]);
    }
    for (uint64_t i = 0; i < 2; i ++) {
        raster_rect [i] [0] = static_cast <uint64_t> (round (boundary_2d [i] [0]));
        raster_rect [i] [1] = static_cast <uint64_t> (round (boundary_2d [1 - i] [1]));
    }
}

void Pipeline::rasterize () {
    linear_param (positions_2d, positions_2d, 3, 2);
    double alpha = positions_2d [1] [0] * positions_2d [2] [1] - positions_2d [2] [0] * positions_2d [1] [1];
    if (front_face_clockwise) {
        if (alpha <= 0)
            return;
    } else {
        if (alpha >= 0)
            return;
    }
    alpha = - 1 / alpha;
    for (fragment.y = raster_rect [0] [1]; fragment.y <= raster_rect [1] [1]; fragment.y ++) {
        for (fragment.x = raster_rect [0] [0]; fragment.x <= raster_rect [1] [0]; fragment.x ++) {
            double delta [] = { positions_2d [0] [0] - fragment.x, positions_2d [0] [1] - fragment.y };
            fragment.u = alpha * (delta [0] * positions_2d [2] [1] - positions_2d [2] [0] * delta [1]);
            fragment.v = alpha * (positions_2d [1] [0] * delta [1] - delta [0] * positions_2d [1] [1]);
            if (fragment.u >= 0 && fragment.v >= 0 && fragment.u + fragment.v <= 1 && depth_test ()) {
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

void Pipeline::build_payload () {
    for (uint64_t i = 0; i < 3; i ++) {
        in.view [i] = blerp (fragment.u, fragment.v, out [0].view [i], out [1].view [i], out [2].view [i]);
        in.normal [i] = blerp (fragment.u, fragment.v, out [0].normal [i], out [1].normal [i], out [2].normal [i]);
        for (uint64_t j = 0; j < NUM_LIGHTS; j ++)
            in.light [j] [i] = blerp (fragment.u, fragment.v, out [0].light [j] [i], out [1].light [j] [i], out [2].light [j] [i]);
        in.color [i] = blerp (fragment.u, fragment.v, out [0].color [i], out [1].color [i], out [2].color [i]);
    }
}

void Pipeline::blend_fragment () {
    uint32_t (* color_buffer_) [width] = reinterpret_cast <uint32_t (*) [width]> (color_buffer);
    double (* depth_buffer_) [width] = reinterpret_cast <double (*) [width]> (depth_buffer);
    uint8_t color [4];
    for (uint64_t i = 0; i < 4; i ++)
        color [i] = static_cast <uint8_t> (round (255 * clamp (fragment.color [i], 0, 1)));
    color_buffer_ [fragment.y] [fragment.x] = qRgba (color [0], color [1], color [2], color [3]);
    depth_buffer_ [fragment.y] [fragment.x] = fragment.depth;
}
