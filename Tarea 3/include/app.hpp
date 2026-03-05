# pragma once

# include <QtWidgets>

# include "gmath.hpp"

# define WIDTH 800
# define HEIGHT 400
# define ASPECT_RATIO 1
# define FOV_V (M_PI / 3)
# define EXT_V 100
# define NEAR 0.1
# define FAR 1000

# define NUM_VERTICES 8

/// @brief La imagen que se muestra en la ventana
extern QImage image;

/// @brief Los vértices del cubo
extern double cube_vertices [NUM_VERTICES] [3];

/// @brief La posición del cubo
extern double cube_position [3];

/// @brief La rotación del cubo en ángulos de euler ZYX
extern double cube_rotation [3];

/// @brief La escala del cubo
extern double cube_scale [3];

/// @brief Las posiciones de las cámaras
extern double camera_positions [2] [3];

/// @brief El punto hacia donde ven las cámaras
extern double camera_target [3];

/// @brief El vector de referencia hacia arriba de las cámaras
extern double camera_ref [3];

/// @brief Coordenadas donde inician las regiones de la imagen asignadas a las cámaras
extern uint64_t viewport_starts [2] [2];

/// @brief Coordenadas donde terminan las regiones de la imagen asignadas a las cámaras
extern uint64_t viewport_ends [2] [2];

/// @brief La matriz de modelo
extern double model_mat [16];

/// @brief La matriz de vista
extern double view_mat [16];

/// @brief La matriz de proyección
extern double projection_mat [16];

/// @brief El producto de las matrices de proyección, vista y modelo
extern double mvp_mat [16];

/// @brief Arreglo con las matrices de proyección, vista y modelo
extern double * mvp_chain [3];

/// @brief Bandera que indica si la proyección actual es en perspectiva
extern bool perspective_flag;

/// @brief Renderiza la escena vista por una cámara en una región de la imagen
/// @param camera_position es la posición de la cámara
/// @param viewport_start es donde incia la región de la imagen
/// @param viewport_end es donde termina la región de la imagen
void render_camera (double * camera_position, uint64_t * viewport_start, uint64_t * viewport_end);