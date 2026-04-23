/// @file app.hpp

# pragma once

# include <iostream>
# include <filesystem>
# include <QtWidgets>

# include "tiny_obj_loader.h"
# include "gmath.hpp"
# include "sampler.hpp"

# define WIDTH 400
# define HEIGHT 400
# define ASPECT_RATIO ((double) WIDTH / HEIGHT)
# define FOV_V (M_PI / 3)
# define EXT_V 100
# define NEAR 0.1
# define FAR 1000
# define NUM_CAMERAS 3
# define NUM_SKYBOXES 5
# define NUM_OBJECTS 7
# define NUM_LIGHTS 2
# define NUM_MATERIALS 2
# define NUM_TEXTURES 6

/// @brief Vértice que se procesa en la línea de renderizado
struct Vertex {
    double position [3];
    double normal [3];
    double texture [2];
};

/// @brief Carga que se transfiere entre etapas de la línea de renderizado
struct Payload {
    double view [3];
    double normal [3];
    double light [NUM_LIGHTS] [3];
    double texture [2];
};

/// @brief Propiedades de una luz puntual
struct Light {
    double position [3];
    double ambient [4];
    double diffuse [4];
    double specular [4];
};

/// @brief Propiedades de un material
struct Material {
    double ambient [4];
    double diffuse [4];
    double specular [4];
    double shininess;
};

/// @brief Transformación de un objeto
struct Transform {
    double position [3];
    double rotation [4];
    double scale [3];
};

/// @brief Transformación de una cámara
struct CameraTransform {
    double position [3];
    double rotation [4];
};

/// @brief La imagen que se muestra en la ventana
extern QImage image;

/// @brief Los vértices del skybox
extern Vertex skybox_vertices [6];

/// @brief Los vértices del objeto
extern std::vector <Vertex> object_vertices;

/// @brief La transformación de los objetos
extern Transform object_transforms [NUM_OBJECTS];

/// @brief Los materiales
extern Material materials [NUM_MATERIALS];

/// @brief Las transformaciones de las cámaras
extern CameraTransform camera_transforms [NUM_CAMERAS];

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

/// @brief La matriz del skybox
extern double skybox_mat [4] [4];

/// @brief Arreglo con las matrices de proyección, vista y modelo
extern void * mvp_chain [3];

/// @brief Bandera que indica si la proyección actual es en perspectiva
extern bool perspective_flag;

/// @brief Bandera que indica si el material es reflexivo
extern bool reflective_material_flag;

/// @brief Arreglo con las luces de la escena
extern Light lights [NUM_LIGHTS];

/// @brief Componentes de la luz ambiental global de la escena
extern double global_ambient [4];

/// @brief Máscara de las luces encendidas
extern uint64_t active_lights;

/// @brief Índice de la cámara seleccionada
extern uint64_t camera_id;

/// @brief Índice del skybox seleccionado
extern uint64_t skybox_id;

/// @brief Índice del objeto seleccionado
extern uint64_t object_id;

/// @brief Índice de la luz seleccionada
extern uint64_t light_id;

/// @brief Índice del material seleccionado
extern uint64_t material_id;

/// @brief Índice de la textura seleccionada
extern uint64_t texture_id;

/// @brief Buffer de color que forma la imagen mostrada en el lienzo
extern uint32_t color_buffer [HEIGHT] [WIDTH];

/// @brief Buffer usado en la prueba de profundidad
extern double depth_buffer [HEIGHT] [WIDTH];

/// @brief Imagen con la textura del material
extern QImage texture_image;

/// @brief Muestreador de la textura
extern Sampler texture_sampler;

/// @brief Imágenes de las caras del skybox
extern QImage cube_map_images [6];

/// @brief Nombres de las carpetas con los skyboxes
extern std::string skybox_dir_names [NUM_SKYBOXES];

/// @brief Muestreador del skybox
extern SamplerCube skybox_sampler;

/// @brief Nombres de los archivos con los modelos de los objetos
extern std::string model_file_names [NUM_OBJECTS];

/// @brief Nombres de los archivos con las texturas
extern std::string texture_file_names [NUM_TEXTURES];

/// @brief Prefijo para formar la ruta a una textura
extern std::filesystem::path texture_prefix;

/// @brief Prefijo para formar la ruta a un modelo obj
extern std::filesystem::path obj_prefix;

/// @brief Carga los vértices de un archivo obj al buffer de vértices
/// @param file es el archivo de donde se leen los vértices
/// @return `true` si y solo si se pudo cargar el modelo obj con éxito
bool load_obj (std::string file);

/// @brief Carga el skybox en una carpeta y actualiza el muestreador de skybos
/// @param dir es la carpeta donde se encuentran las imágenes de las caras
/// @return `true` si y solo si se pudo cargar el skybox con éxito
bool load_cube_map (std::string dir);

/// @brief Carga la textura del archivo especificado y actualiza el muestreador de textura
/// @param file es el archivo con la textura
/// @return `true` si y solo si se pudo cargar la textura con éxito
bool load_texture (std::string file);

/// @brief Genera la posición final de un vértice del skybox
/// @param vertex es el vértice
/// @param position es donde se guarda la posición final
/// @return la carga que se envía a los fragmentos
Payload skybox_vertex_shader (Vertex vertex, double * position);

/// @brief Genera el color final de un fragmento del skybox
/// @param in es la carga que se recibe
/// @param color es donde se guarda el color final
void skybox_fragment_shader (Payload in, double * color);

/// @brief Genera la posición final de un vértice del objeto
/// @param vertex es el vértice
/// @param position es donde se guarda la posición final
/// @return la carga que se envía a los fragmentos
Payload object_vertex_shader (Vertex vertex, double * position);

/// @brief Genera el color final de un fragmento del objeto
/// @param in es la carga que se recibe
/// @param color es donde se guarda el color final
void object_fragment_shader (Payload in, double * color);

/// @brief Ejecuta la lógica para renderizar la escena
void render_scene ();

/// @brief Actualiza la rotación de la cámara para que apunte hacia la posición del objeto actual
void center_view ();