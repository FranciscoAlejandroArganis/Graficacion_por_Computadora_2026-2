# include "app.hpp"

# define TINYOBJLOADER_IMPLEMENTATION
# include "tiny_obj_loader.h"

QImage image;
std::vector <Vertex> vertices;
Transform cube_transform = {
    .position = { 0, 0, 0 },
    .rotation = { 0, 0, 0 },
    .scale = { 1, 1, 1 }
};
Material cube_material = {
    .ambient = { .3, .3, .3 },
    .diffuse = { .07568, .61424, .07568 },
    .specular = { .633, .727811, .633 },
    .shininess = 76.8
};
double camera_positions [] [3] = {
    { 40, 60, 50 },
    { - 20, - 40, 60 }
};
double camera_target [] = { 0, 0, 0 };
double camera_ref [] = { 0, 0, 1 };
double model_mat [4] [4];
double view_mat [4] [4];
double projection_mat [4] [4];
double normal_mat [4] [4];
double mvp_mat [4] [4];
void * mvp_chain [] = { projection_mat, view_mat, model_mat };
bool perspective_flag = true;
Light lights [] = {
    {
        .position = { 70, 50, 100 },
        .ambient = { .1, .1, .1 },
        .diffuse = { .8, .8, .8 },
        .specular = { .8, .8, .8 }
    },
    {
        .position = { 70, -50, 100 },
        .ambient = { .1, 0, 0 },
        .diffuse = { .8, 0, 0 },
        .specular = { .8, 0, 0 }
    }
};
double global_ambient [] = { .01, .01, .01 };
uint64_t active_lights = 3;
uint64_t camera_id = 0;
uint64_t light_id = 0;
bool filter = true;
Kernel kernel = {
    .radius = KERNEL_RADIUS,
    .data = (double [] [KERNEL_SIZE]) {
        { - 1. / 9, - 1. / 9, - 1. / 9 },
        { - 1. / 9, 2 - 1. / 9, - 1. / 9 },
        { - 1. / 9, - 1. / 9, - 1. / 9 }
    }
};
uint32_t queue [QUEUE_SIZE];
uint32_t color_buffer [HEIGHT] [WIDTH];
double depth_buffer [HEIGHT] [WIDTH];

Payload vertex_shader (Vertex vertex, double * position) {
    Payload out;
    double vec_in [4];
    double vec_out [4];
    vec_to_homogeneous (vertex.position, vec_in, 3);
    mat_vec_mul (mvp_mat, vec_in, vec_out, 4, 4);
    vec_from_homogeneous (vec_out, position, 4);
    mat_vec_mul (model_mat, vec_in, vec_out, 4, 4);
    vec_sub (camera_positions [camera_id], vec_out, out.view, 3);
    for (uint64_t i = 0; i < NUM_LIGHTS; i ++) {
        if (active_lights & 1lu << i)
            vec_sub (lights [i].position, vec_out, out.light [i], 3);
    }
    vec_to_homogeneous (vertex.normal, vec_in, 3);
    mat_vec_mul (normal_mat, vec_in, vec_out, 4, 4);
    memcpy (out.normal, vec_out, 3 * sizeof (double));
    memcpy (out.color, vertex.color, 3 * sizeof (double));
    return out;
}

void fragment_shader (Payload in, double * color) {
    vec_unit (in.view, in.view, 3);
    vec_unit (in.normal, in.normal, 3);
    vec_emul (cube_material.ambient, global_ambient, color, 3);
    for (uint64_t i = 0; i < NUM_LIGHTS; i ++) {
        if (! (active_lights & 1lu << i))
            continue;
        vec_unit (in.light [i], in.light [i], 3);
        double dot;
        double temp [3];

        vec_emul (cube_material.ambient, lights [i].ambient, temp, 3);
        vec_add (color, temp, color, 3);

        dot = vec_dot_prod (in.normal, in.light [i], 3);
        if (dot < 0)
            dot = 0;
        vec_emul (cube_material.diffuse, lights [i].diffuse, temp, 3);
        vec_scalar_mul (temp, dot, temp, 3);
        vec_add (color, temp, color, 3);

        vec_add (in.light [i], in.view, temp, 3);
        vec_unit (temp, temp, 3);
        dot = vec_dot_prod (temp, in.normal, 3);
        if (dot < 0)
            dot = 0;
        dot = pow (dot, cube_material.shininess);
        vec_emul (cube_material.specular, lights [i].specular, temp, 3);
        vec_scalar_mul (temp, dot, temp, 3);
        vec_add (color, temp, color, 3);
    }
    vec_emul (color, in.color, color, 3);
}

bool load_obj (std::string path) {
    tinyobj::ObjReader reader;
    if (! reader.ParseFromFile (path))
        return false;
    tinyobj::attrib_t attrib = reader.GetAttrib ();
    tinyobj::real_t (* positions) [3] = reinterpret_cast <tinyobj::real_t (*) [3]> (attrib.vertices.data ());
    for (tinyobj::shape_t shape : reader.GetShapes ()) {
        tinyobj::index_t (* indices) [3] = reinterpret_cast <tinyobj::index_t (*) [3]> (shape.mesh.indices.data ());
        for (uint64_t i = 0; i < shape.mesh.num_face_vertices.size (); i ++) {
            for (uint64_t j = 0; j < 3; j ++) {
                uint64_t index = indices [i] [j].vertex_index;
                Vertex vertex = {
                    .position = { positions [index] [0], positions [index] [1], positions [index] [2] },
                    .color = { 1, 1, 1 }
                };
                vertices.push_back (vertex);
            }
        }
    }
    for (uint64_t i = 0; i < vertices.size (); i += 3) {
        plane_normal (vertices [i].position, vertices [i + 1].position, vertices [i + 2].position, vertices [i].normal);
        vec_unit (vertices [i].normal, vertices [i].normal, 3);
        memcpy (vertices [i + 1].normal, vertices [i].normal, 3 * sizeof (double));
        memcpy (vertices [i + 2].normal, vertices [i].normal, 3 * sizeof (double));
    }
    std::random_device rd;
    std::mt19937 gen (rd ());
    std::uniform_real_distribution <double> dis (.75, 1);
    for (uint64_t i = 0; i < vertices.size (); i += 6) {
        vertices [i].color [0] = dis (gen);
        vertices [i].color [1] = dis (gen);
        vertices [i].color [2] = dis (gen);
        for (uint64_t j = 1; j < 6; j ++)
            memcpy (vertices [i + j].color, vertices [i].color, 3 * sizeof (double));
    }
    return true;
}
