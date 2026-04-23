# include "app.hpp"

# define TINYOBJLOADER_IMPLEMENTATION
# include "tiny_obj_loader.h"

# include "pipeline.hpp"

QImage image;
Vertex skybox_vertices [] = {
    { .position = { - 1, .5, - 1 } },
    { .position = {   1, .5, - 1 } },
    { .position = {   1, .5,   1 } },
    { .position = { - 1, .5, - 1 } },
    { .position = {   1, .5,   1 } },
    { .position = { - 1, .5,   1 } }
};
std::vector <Vertex> object_vertices;
Transform object_transforms [] = {
    {
        .position = { 0, 0, 0 },
        .rotation = { 1.57, 0, 3.14 },
        .scale = { 15, 15, 15 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 1.57, 0, -1.57 },
        .scale = { 75, 75, 75 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 1.57, 0, 3.14 },
        .scale = { 175, 175, 175 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 1.57, 0, 0 },
        .scale = { 10, 10, 10 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 0, 0, 0 },
        .scale = { 1, 1, 1 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 0, 0, 0 },
        .scale = { 10, 10, 10 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 1.57, 0, 3.14 },
        .scale = { .24, .24, .24 }
    }
};
Material materials [] = {
    {
        .ambient = { 0, 0, 0, 1 },
        .diffuse = { .5, .5, .5, 1 },
        .specular = { .7, .7, .7, 1 },
        .shininess = 32
    },
    {
        .ambient = { .23125, .23125, .23125, 1 },
        .diffuse = { .2775, .2775, .2775, 1 },
        .specular = { .773911, .773911, .773911, 1 },
        .shininess = 89.6
    }
};
CameraTransform camera_transforms [] = {
    {
        .position = { 11.3, 62, 61.1 },
        .rotation = { - .77, 0, 2.96 }
    },
    {
        .position = { - 55.7, -19, 57.3 },
        .rotation { - .77, 0, - 1.24 }
    },
    {
        .position = { 50, - 34.6, 65.5 },
        .rotation = { - .82, 0, .97 }
    }
};
double camera_ref [] = { 0, 0, 1 };
double model_mat [4] [4];
double view_mat [4] [4];
double projection_mat [4] [4];
double normal_mat [4] [4];
double mvp_mat [4] [4];
double inv_view_mat [4] [4];
double inv_proj_mat [4] [4];
double skybox_mat [4] [4];
void * mvp_chain [] = { projection_mat, view_mat, model_mat };
bool perspective_flag = true;
bool reflective_material_flag = false;
Light lights [] = {
    {
        .position = { 15.3, 37.4, 70 },
        .ambient = { .1, .1, .1, 1 },
        .diffuse = { 1, 1, 1, 1 },
        .specular = { 1, 1, 1, 1 }
    },
    {
        .position = { - 2, - 32.6, 70 },
        .ambient = { .04, .16, .2, 1 },
        .diffuse = { .2, .8, 1, 1 },
        .specular = { .2, .8, 1, 1 }
    }
};
double global_ambient [] = { .1, .1, .1, 1 };
uint64_t active_lights = 3;
uint64_t camera_id = 0;
uint64_t skybox_id = 0;
uint64_t object_id = 0;
uint64_t light_id = 0;
uint64_t material_id = 0;
uint64_t texture_id = 0;
uint32_t color_buffer [HEIGHT] [WIDTH];
double depth_buffer [HEIGHT] [WIDTH];
QImage texture_image;
Sampler texture_sampler = { .filter = SAMPLER_FILTER_LINEAR };
QImage cube_map_images [6];
std::string skybox_dir_names [] = { "", "Cielo", "Espacio", "Yokohama", "Nissi" };
SamplerCube skybox_sampler = { .filter = SAMPLER_FILTER_LINEAR };
std::string model_file_names [] = { "bunny.obj", "dragon.obj", "Happy_Buddha.obj", "Teapot.obj", "Cube_Quads.obj", "Sphere_Patches.obj", "Nathan.obj" };
std::string texture_file_names [] = { "", "Madera.png", "Ladrillos.png", "Baldosas.png", "Placas.png", "Nathan.png" };
std::filesystem::path texture_prefix = "textures";
std::filesystem::path obj_prefix = "obj";

bool load_obj (std::string file) {
    tinyobj::ObjReader reader;
    if (! reader.ParseFromFile ((obj_prefix / file).c_str ()))
        return false;
    object_vertices.clear ();
    tinyobj::attrib_t attrib = reader.GetAttrib ();
    tinyobj::real_t (* positions) [3] = reinterpret_cast <tinyobj::real_t (*) [3]> (attrib.vertices.data ());
    tinyobj::real_t (* normals) [3] = reinterpret_cast <tinyobj::real_t (*) [3] > (attrib.normals.data ());
    tinyobj::real_t (* texture_coords) [2] = reinterpret_cast <tinyobj::real_t (*) [2] > (attrib.texcoords.data ());
    for (tinyobj::shape_t shape : reader.GetShapes ()) {
        tinyobj::index_t (* indices) [3] = reinterpret_cast <tinyobj::index_t (*) [3]> (shape.mesh.indices.data ());
        for (uint64_t i = 0; i < shape.mesh.num_face_vertices.size (); i ++) {
            bool missing_normal_data = false;
            bool missing_texture_data = false;
            for (uint64_t j = 0; j < 3; j ++) {
                int64_t index = indices [i] [j].vertex_index;
                Vertex vertex = { .position = { positions [index] [0], positions [index] [1], positions [index] [2] } };
                index = indices [i] [j].normal_index;
                if (index >= 0) {
                    vertex.normal [0] = normals [index] [0];
                    vertex.normal [1] = normals [index] [1];
                    vertex.normal [2] = normals [index] [2];
                } else
                    missing_normal_data = true;
                index = indices [i] [j].texcoord_index;
                if (index >= 0) {
                    vertex.texture [0] = texture_coords [index] [0];
                    vertex.texture [1] = texture_coords [index] [1];
                } else
                    missing_texture_data = true;
                object_vertices.push_back (vertex);
            }
            Vertex * triangle = object_vertices.data () + 3 * i;
            if (missing_normal_data) {
                plane_normal (triangle [0].position, triangle [1].position, triangle [2].position, triangle [0].normal);
                vec_unit (triangle [0].normal, triangle [0].normal, 3);
                memcpy (triangle [1].normal, triangle [0].normal, 3 * sizeof (double));
                memcpy (triangle [2].normal, triangle [0].normal, 3 * sizeof (double));
            }
            if (missing_texture_data) {
                if (i & 1) {
                    triangle [0].texture [0] = 0;
                    triangle [0].texture [1] = 1;
                    triangle [1].texture [0] = 1;
                    triangle [1].texture [1] = 1;
                    triangle [2].texture [0] = 1;
                    triangle [2].texture [1] = 0;
                } else {
                    triangle [0].texture [0] = 0;
                    triangle [0].texture [1] = 0;
                    triangle [1].texture [0] = 0;
                    triangle [1].texture [1] = 1;
                    triangle [2].texture [0] = 1;
                    triangle [2].texture [1] = 0;
                }
            }
        }
    }
    return true;
}

bool load_cube_map (std::string dir) {
    cube_map_images [SAMPLER_CUBE_FACE_LEFT] = QImage ((texture_prefix / dir / "Left.png").c_str ());
    if (cube_map_images [SAMPLER_CUBE_FACE_LEFT].isNull ())
        return false;
    cube_map_images [SAMPLER_CUBE_FACE_RIGHT] = QImage ((texture_prefix / dir / "Right.png").c_str ());
    if (cube_map_images [SAMPLER_CUBE_FACE_RIGHT].isNull ())
        return false;
    cube_map_images [SAMPLER_CUBE_FACE_BACK] = QImage ((texture_prefix / dir / "Back.png").c_str ());
    if (cube_map_images [SAMPLER_CUBE_FACE_BACK].isNull ())
        return false;
    cube_map_images [SAMPLER_CUBE_FACE_FRONT] = QImage ((texture_prefix / dir / "Front.png").c_str ());
    if (cube_map_images [SAMPLER_CUBE_FACE_FRONT].isNull ())
        return false;
    cube_map_images [SAMPLER_CUBE_FACE_DOWN] = QImage ((texture_prefix /dir / "Down.png").c_str ());
    if (cube_map_images [SAMPLER_CUBE_FACE_DOWN].isNull ())
        return false;
    cube_map_images [SAMPLER_CUBE_FACE_UP] = QImage ((texture_prefix / dir / "Up.png").c_str ());
    if (cube_map_images [SAMPLER_CUBE_FACE_UP].isNull ())
        return false;
    for (uint64_t i = 0; i < 6; i ++) {
        skybox_sampler.cube_map [i].extent.width = cube_map_images [i].width ();
        skybox_sampler.cube_map [i].extent.height = cube_map_images [i].height ();
        skybox_sampler.cube_map [i].channels = 4;
        skybox_sampler.cube_map [i].data = cube_map_images [i].bits ();
    }
    return true;
}

bool load_texture (std::string file) {
    texture_image = QImage ((texture_prefix / file).c_str ());
    if (texture_image.isNull ())
        return false;
    texture_sampler.image.extent.width = texture_image.width ();
    texture_sampler.image.extent.height = texture_image.height ();
    texture_sampler.image.channels = 4;
    texture_sampler.image.data = texture_image.bits ();
    return true;
}

Payload skybox_vertex_shader (Vertex vertex, double * position) {
    Payload out;
    double vec [4];
    vec_to_homogeneous (vertex.position, position, 3);
    memcpy (out.view, vertex.position, 3 * sizeof (double));
    return out;
}

void skybox_fragment_shader (Payload in, double * color) {
    double vec_in [4];
    double vec_out [4];
    in.view [1] = 1;
    vec_to_homogeneous (in.view, vec_in, 3);
    mat_vec_mul (skybox_mat, vec_in, vec_out, 4, 4);
    vec_from_homogeneous (vec_out, vec_in, 4);
    vec_sub (vec_in, camera_transforms [camera_id].position, vec_in, 3);
    skybox_sampler.sample (vec_in, color);
}

Payload object_vertex_shader (Vertex vertex, double * position) {
    Payload out;
    double vec_in [4];
    double vec_out [4];
    vec_to_homogeneous (vertex.position, vec_in, 3);
    mat_vec_mul (mvp_mat, vec_in, position, 4, 4);
    mat_vec_mul (model_mat, vec_in, vec_out, 4, 4);
    vec_sub (camera_transforms [camera_id].position, vec_out, out.view, 3);
    for (uint64_t light = 0; light < NUM_LIGHTS; light ++) {
        if (active_lights & 1lu << light)
            vec_sub (lights [light].position, vec_out, out.light [light], 3);
    }
    vec_to_homogeneous (vertex.normal, vec_in, 3);
    mat_vec_mul (normal_mat, vec_in, vec_out, 4, 4);
    memcpy (out.normal, vec_out, 3 * sizeof (double));
    memcpy (out.texture, vertex.texture, 2 * sizeof (double));
    return out;
}

void object_fragment_shader (Payload in, double * color) {
    vec_unit (in.view, in.view, 3);
    vec_unit (in.normal, in.normal, 3);
    double temp [4];
    if (reflective_material_flag) {
        if (skybox_id > 0) {
            vec_reflect (in.view, in.normal, temp, 3);
            vec_neg (temp, temp, 3);
            skybox_sampler.sample (temp, color);
        } else {
            color [0] = 0;
            color [1] = 0;
            color [2] = 0;
            color [3] = 1;
        }
        return;
    }
    vec_emul (materials [material_id].ambient, global_ambient, color, 4);
    for (uint64_t light = 0; light < NUM_LIGHTS; light ++) {
        if (! (active_lights & 1lu << light))
            continue;
        vec_unit (in.light [light], in.light [light], 3);
        double dot;

        vec_emul (materials [material_id].ambient, lights [light].ambient, temp, 4);
        vec_add (color, temp, color, 4);

        dot = vec_dot_prod (in.normal, in.light [light], 3);
        if (dot < 0)
            dot = 0;
        vec_emul (materials [material_id].diffuse, lights [light].diffuse, temp, 4);
        vec_scalar_mul (temp, dot, temp, 4);
        vec_add (color, temp, color, 4);

        vec_reflect (in.light [light], in.normal, temp, 3);
        dot = - vec_dot_prod (temp, in.view, 3);
        if (dot < 0)
            dot = 0;
        dot = pow (dot, materials [material_id].shininess);
        vec_emul (materials [material_id].specular, lights [light].specular, temp, 4);
        vec_scalar_mul (temp, dot, temp, 4);
        vec_add (color, temp, color, 4);
    }
    if (texture_id > 0) {
        texture_sampler.sample (in.texture, temp);
        vec_emul (color, temp, color, 4);
    }
}

void render_scene () {
    pipeline.clear_buffers ();
    
    transform_euler_zyx_mat4 (object_transforms [object_id].position, object_transforms [object_id].rotation, object_transforms [object_id].scale, model_mat);
    camera_euler_zyx_mat4 (camera_transforms [camera_id].position, camera_transforms [camera_id].rotation, view_mat);
    if (perspective_flag)
        perspective_proj_mat4 (ASPECT_RATIO, FOV_V, NEAR, FAR, projection_mat);
    else
        orthogonal_proj_mat4 (ASPECT_RATIO, EXT_V, 0, FAR, projection_mat);
    
    double temp [4] [4];
    mat_mul (projection_mat, view_mat, temp, 4, 4, 4);
    mat_mul (temp, model_mat, mvp_mat, 4, 4, 4);

    if (skybox_id > 0) {
        mat_inverse (temp, skybox_mat, 4);

        pipeline.num_primitives = 2;
        pipeline.vertex_buffer = skybox_vertices;
        pipeline.vertex_shader = skybox_vertex_shader;
        pipeline.fragment_shader = skybox_fragment_shader;
        pipeline.depth_write_disabled = true;
        pipeline.draw ();
    }

    memcpy (normal_mat, model_mat, 16 * sizeof (double));
    mat_inverse (normal_mat, temp, 4);
    mat_transpose (temp, normal_mat, 4, 4);

    pipeline.num_primitives = object_vertices.size () / 3;
    pipeline.vertex_buffer = object_vertices.data ();
    pipeline.vertex_shader = object_vertex_shader;
    pipeline.fragment_shader = object_fragment_shader;
    pipeline.depth_write_disabled = false;
    pipeline.draw ();
}

void center_view () {
    camera_look_at_mat4 (camera_transforms [camera_id].position, object_transforms [object_id].position, camera_ref, view_mat);
    double mat [4] [4];
    mat_transpose (view_mat, mat, 4, 4);
    mat4_to_euler_zyx (mat, camera_transforms [camera_id].rotation);
}