# include "app.hpp"

QImage image (WIDTH, HEIGHT, QImage::Format_RGB32);
double cube_vertices [] [3] = {
    { -1, -1, -1 },
    { -1, -1,  1 },
    { -1,  1, -1 },
    { -1,  1,  1 },
    {  1, -1, -1 },
    {  1, -1,  1 },
    {  1,  1, -1 },
    {  1,  1,  1 }
};
double cube_position [] = { 0, 0, 0 };
double cube_rotation [] = { 0, 0, 0 };
double cube_scale [] = { 15, 20, 10 };
double camera_positions [] [3] = {
    { 40, 60, 50 },
    { -20, -40, 60 }
};
double camera_target [] = { 0, 0, 0 };
double camera_ref [] = { 0, 0, 1 };
uint64_t viewport_starts [] [2] = {
    { 0, 0 },
    { WIDTH >> 1, 0 }
};
uint64_t viewport_ends [] [2] = {
    { (WIDTH >> 1) - 1, HEIGHT - 1 },
    { WIDTH - 1, HEIGHT - 1 }
};
double model_mat [16];
double view_mat [16];
double projection_mat [16];
double mvp_mat [16];
double * mvp_chain [] = { projection_mat, view_mat, model_mat };
bool perspective_flag = true;

void render_camera (double * camera_position, uint64_t * viewport_start, uint64_t * viewport_end) {
    camera_look_at_mat4 (camera_position, camera_target, camera_ref, view_mat);
    mat4_chain_mul (mvp_chain, 3, mvp_mat);
    for (uint64_t i = 0; i < NUM_VERTICES; i ++) {
        double vertex [4];
        double temp [4];
        vec_to_homogeneous (cube_vertices [i], vertex, 3);
        mat_vec_mul (mvp_mat, vertex, temp, 4, 4);
        vec_from_homogeneous (temp, vertex, 4);
        if (
            - 1 <= vertex [0] && vertex [0] <= 1 &&
              0 <= vertex [1] && vertex [1] <= 1 &&
            - 1 <= vertex [2] && vertex [2] <= 1
        ) {
            double pixel_coordinates [2] = {
                .5 * (viewport_end [0] * (vertex [0] + 1) - viewport_start [0] * (vertex [0] - 1)),
                .5 * (viewport_start [1] * (vertex [2] + 1) - viewport_end [1] * (vertex [2] - 1))
            };
            image.setPixelColor (
                round (pixel_coordinates [0]),
                round (pixel_coordinates [1]),
                Qt::white
            );
        }
    }
}