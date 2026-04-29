/// @file app.hpp

# pragma once

# include <GL/glu.h>
# include <QOpenGLFunctions_4_5_Compatibility>
# include <QtOpenGLWidgets>

# include "tiny_obj_loader.h"

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
# define RAD_TO_DEG 57.29577951308232

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

/// @brief La transformación de los objetos
extern Transform object_transforms [NUM_OBJECTS];

/// @brief Los materiales
extern Material materials [NUM_MATERIALS];

/// @brief Las transformaciones de las cámaras
extern CameraTransform camera_transforms [NUM_CAMERAS];

/// @brief El vector de referencia hacia arriba de las cámaras
extern double camera_ref [3];

/// @brief Bandera que indica si la proyección actual es en perspectiva
extern bool perspective_flag;

/// @brief Bandera que indica si se usan coordenadas de textura generadas automáticamente
extern bool gen_text_coords_flag;

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

/// @brief Relación de aspecto del framebuffer
extern double aspect_ratio;

/// @brief Bandera que indica que se va a redibujar la escena
extern bool draw_commit;

/// @brief Los vértices de los objetos
extern std::vector <Vertex> object_vertices [NUM_OBJECTS];

/// @brief Imágenes de las texturas
extern QImage texture_images [NUM_TEXTURES];

/// @brief Nombres de OpenGL para las texturas
extern GLuint texture_names [NUM_TEXTURES];

/// @brief Carga los vértices de un archivo obj al buffer de vértices del objecto seleccionado
/// @param file es el archivo de donde se leen los vértices
/// @return `true` si y solo si se pudo cargar el modelo obj con éxito
bool load_obj (std::string file);

/// @brief Carga la imagen de un archivo a la textura seleccionada
/// @param file es el archivo con la imagen
/// @return `true` si y solo si se pudo cargar la imagen con éxito
bool load_texture (std::string file);
