/// @file app.hpp

# pragma once

# include <QtWidgets>

# include "tiny_obj_loader.h"
# include "gmath.hpp"
# include "dip.hpp"

# define WIDTH 800
# define HEIGHT 400
# define ASPECT_RATIO (WIDTH / HEIGHT)
# define FOV_V (M_PI / 3)
# define EXT_V 100
# define NEAR 0.1
# define FAR 1000
# define KERNEL_RADIUS 1
# define KERNEL_SIZE 3
# define QUEUE_SIZE 401
# define NUM_LIGHTS 2

/// @brief Vértice que se procesa en la línea de renderizado
struct Vertex {
    double position [3];
    double normal [3];
    double color [3];
};

/// @brief Carga que se transfiere entre etapas de la línea de renderizado
struct Payload {
    double view [3];
    double normal [3];
    double light [NUM_LIGHTS] [3];
    double color [3];
};

/// @brief Propiedades de una luz puntual
struct Light {
    double position [3];
    double ambient [3];
    double diffuse [3];
    double specular [3];
};

/// @brief Propiedades de un material
struct Material {
    double ambient [3];
    double diffuse [3];
    double specular [3];
    double shininess;
};

/// @brief Transformación de un objeto
struct Transform {
    double position [3];
    double rotation [4];
    double scale [3];
};

/// @brief La imagen que se muestra en la ventana
extern QImage image;

/// @brief Los vértices del cubo
extern std::vector <Vertex> vertices;

/// @brief La transformación del cubo
extern Transform cube_transform;

/// @brief El material del cubo
extern Material cube_material;

/// @brief Las posiciones de las cámaras
extern double camera_positions [2] [3];

/// @brief El punto hacia donde ven las cámaras
extern double camera_target [3];

/// @brief El vector de referencia hacia arriba de las cámaras
extern double camera_ref [3];

/// @brief La matriz de modelo
extern double model_mat [4] [4];

/// @brief La matriz de vista
extern double view_mat [4] [4];

/// @brief La matriz de proyección
extern double projection_mat [4] [4];

/// @brief La matriz normal
extern double normal_mat [4] [4];

/// @brief El producto de las matrices de proyección, vista y modelo
extern double mvp_mat [4] [4];

/// @brief Arreglo con las matrices de proyección, vista y modelo
extern void * mvp_chain [3];

/// @brief Bandera que indica si la proyección actual es en perspectiva
extern bool perspective_flag;

/// @brief Arreglo con las luces de la escena
extern Light lights [NUM_LIGHTS];

/// @brief Componentes de la luz ambiental global de la escena
extern double global_ambient [3];

/// @brief Máscara de las luces encendidas
extern uint64_t active_lights;

/// @brief Índice de la cámara seleccionada
extern uint64_t camera_id;

/// @brief Índice de la luz seleccionada
extern uint64_t light_id;

/// @brief Bandera que indica si se aplica el filtro de posprocesamiento
extern bool filter;

/// @brief Kernel usado en el filtro de posprocesamiento
extern Kernel kernel;

/// @brief Cola usada para las operaciones de convolución
extern uint32_t queue [QUEUE_SIZE];

/// @brief Buffer de color que forma la imagen mostrada en el lienzo
extern uint32_t color_buffer [HEIGHT] [WIDTH];

/// @brief Buffer usado en la prueba de profundidad
extern double depth_buffer [HEIGHT] [WIDTH];

/// @brief Genera la posición final de un vértice
/// @param vertex es el vértice
/// @param position es donde se guarda la posición final
/// @return la carga que se envía a los fragmentos
Payload vertex_shader (Vertex vertex, double * position);

/// @brief Genera el color final de un fragmento
/// @param in es la carga que se recibe
/// @param color es donde se guarda el color final
void fragment_shader (Payload in, double * color);

/// @brief Construye el buffer de vértices a partir de un archivo obj con caras formadas por cuadriláteros.
/// Se asigna un color aleatorio para cada cara.
bool load_obj (std::string path);
