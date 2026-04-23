/// @file pipeline.hpp

# pragma once

# include "app.hpp"

/// @brief Fragmento generado por el rasterizador
struct Fragment {
    uint64_t x, y;
    double u, v, depth;
    double color [4];
};

/// @brief Línea de renderizado por rasterización
struct Pipeline {

    uint64_t num_primitives, width, height, primitive_id, vertex_id;
    Fragment fragment;
    Payload out [3];
    Payload in;
    uint64_t viewport [2] [2];
    uint64_t raster_rect [2] [2];
    double homogeneous_coordinates [3];
    double positions_3d [3] [3];
    double boundary_3d [2] [3];
    double positions_2d [3] [2];
    double boundary_2d [2] [2];
    void * color_buffer;
    void * depth_buffer;
    void * vertex_buffer;
    Payload (* vertex_shader) (Vertex, double *);
    void (* fragment_shader) (Payload, double *);
    bool front_face_clockwise;
    bool invert_y_viewport;
    bool depth_write_disabled;

    /// @brief Limpia los buffers de color y profundidad
    void clear_buffers ();

    /// @brief Dibuja triángulos
    void draw ();

    /// @brief Realiza la prueba de recorte
    /// @return `true` si y solo si la primitiva actual está en el volumen de visión
    bool clipping_test ();

    /// @brief Pasa la primitiva actual al plano de proyección
    void to_plane ();

    /// @brief Pasa la primitiva actual a la región de dibujo en la imagen
    void to_viewport ();

    /// @brief Rasteriza la primitiva actual
    void rasterize ();

    /// @brief Realiza la prueba de profundidad
    /// @return `true` si y solo si el fragmento actual es visible dada su profundidad
    bool depth_test ();

    /// @biref Transforma las coordenadas baricéntricas de un fragmento en el plano a sus coordenadas baricéntricas en el espacio antes de la proyección
    void map_barycentric_coordinates ();

    /// @brief Construye la carga del fragmento actual
    void build_payload ();

    /// @brief Asigna el color del fragmento al buffer de color
    void blend_fragment ();

};

/// @brief Línea de renderizado de la aplicación
extern Pipeline pipeline;
