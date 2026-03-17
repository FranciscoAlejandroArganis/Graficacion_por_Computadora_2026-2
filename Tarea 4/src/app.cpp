# include "app.hpp"

QImage image;
Vertex vertex_buffer [] = {
    { .position = { 1, 1, 1 }, .normal = { 1, 0, 0 }, .color = { 1, .75, .75 } },
    { .position = { 1, 1, -1 }, .normal = { 1, 0, 0 }, .color = { 1, .75, .75 } },
    { .position = { 1, -1, -1 }, .normal = { 1, 0, 0 }, .color = { 1, .75, .75 } },

    { .position = { 1, 1, 1 }, .normal = { 1, 0, 0 }, .color = { 1, .75, .75 } },
    { .position = { 1, -1, -1 }, .normal = { 1, 0, 0 }, .color = { 1, .75, .75 } },
    { .position = { 1, -1, 1 }, .normal = { 1, 0, 0 }, .color = { 1, .75, .75 } },

    { .position = { 1, 1, -1 }, .normal = { 0, 0, -1 }, .color = { .75, .75, 1 } },
    { .position = { -1, 1, -1 }, .normal = { 0, 0, -1 }, .color = { .75, .75, 1 } },
    { .position = { 1, -1, -1 }, .normal = { 0, 0, -1 }, .color = { .75, .75, 1 } },

    { .position = { 1, -1, -1 }, .normal = { 0, 0, -1 }, .color = { .75, .75, 1 } },
    { .position = { -1, 1, -1 }, .normal = { 0, 0, -1 }, .color = { .75, .75, 1 } },
    { .position = { -1, -1, -1 }, .normal = { 0, 0, -1 }, .color = { .75, .75, 1 } },

    { .position = { -1, -1, 1 }, .normal = { 0, -1, 0 }, .color = { .75, 1, .75 } },
    { .position = { 1, -1, -1 }, .normal = { 0, -1, 0 }, .color = { .75, 1, .75 } },
    { .position = { -1, -1, -1 }, .normal = { 0, -1, 0 }, .color = { .75, 1, .75 } },

    { .position = { -1, -1, 1 }, .normal = { 0, -1, 0 }, .color = { .75, 1, .75 } },
    { .position = { 1, -1, 1 }, .normal = { 0, -1, 0 }, .color = { .75, 1, .75 } },
    { .position = { 1, -1, -1 }, .normal = { 0, -1, 0 }, .color = { .75, 1, .75 } },

    { .position = { -1, -1, 1 }, .normal = { -1, 0, 0 }, .color = { 1, .75, .75 } },
    { .position = { -1, -1, -1 }, .normal = { -1, 0, 0 }, .color = { 1, .75, .75 } },
    { .position = { -1, 1, -1 }, .normal = { -1, 0, 0 }, .color = { 1, .75, .75 } },

    { .position = { -1, -1, 1 }, .normal = { -1, 0, 0 }, .color = { 1, .75, .75 } },
    { .position = { -1, 1, -1 }, .normal = { -1, 0, 0 }, .color = { 1, .75, .75 } },
    { .position = { -1, 1, 1 }, .normal = { -1, 0, 0 }, .color = { 1, .75, .75 } },

    { .position = { 1, 1, 1 }, .normal = { 0, 0, 1 }, .color = { .75, .75, 1 } },
    { .position = { 1, -1, 1 }, .normal = { 0, 0, 1 }, .color = { .75,.75, 1 } },
    { .position = { -1, 1, 1 }, .normal = { 0, 0, 1 }, .color = { .75, .75, 1 } },

    { .position = { 1, -1, 1 }, .normal = { 0, 0, 1 }, .color = { .75, .75, 1 } },
    { .position = { -1, -1, 1 }, .normal = { 0, 0, 1 }, .color = { .75, .75, 1 } },
    { .position = { -1, 1, 1 }, .normal = { 0, 0, 1 }, .color = { .75, .75, 1 } },

    { .position = { 1, 1, -1 }, .normal = { 0, 1, 0 }, .color = { .75, 1, .75 } },
    { .position = { 1, 1, 1 }, .normal = { 0, 1, 0 }, .color = { .75, 1, .75 } },
    { .position = { -1, 1, 1 }, .normal = { 0, 1, 0 }, .color = { .75, 1, .75 } },

    { .position = { -1, 1, 1 }, .normal = { 0, 1, 0 }, .color = { .75, 1, .75 } },
    { .position = { -1, 1, -1 }, .normal = { 0, 1, 0 }, .color = { .75, 1, .75 } },
    { .position = { 1, 1, -1 }, .normal = { 0, 1, 0 }, .color = { .75, 1, .75 } }
};
Transform cube_transform = {
    .position = { 0, 0, 0 },
    .rotation = { 0, 0, 0 },
    .scale = { 15, 20, 10 }
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
double global_ambient [3] = { .01, .01, .01 };
uint64_t active_lights = 3;
uint64_t camera_id = 0;
uint64_t light_id = 0;

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

        vec_reflect (in.light [i], in.normal, temp, 3);
        dot = - vec_dot_prod (temp, in.view, 3);
        if (dot < 0)
            dot = 0;
        dot = pow (dot, cube_material.shininess);
        vec_emul (cube_material.specular, lights [i].specular, temp, 3);
        vec_scalar_mul (temp, dot, temp, 3);
        vec_add (color, temp, color, 3);
    }
    vec_emul (color, in.color, color, 3);
}
