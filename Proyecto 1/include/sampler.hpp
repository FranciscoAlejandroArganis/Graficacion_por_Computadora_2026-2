/// @file pipeline.hpp

# pragma once

# include "gmath.hpp"
# include "dip.hpp"

/// @brief Filtros soportados para un muestreador
enum SamplerFilter : uint64_t {
    SAMPLER_FILTER_NEAREST,
    SAMPLER_FILTER_LINEAR
};

/// @brief Tipos soportados de manejo de coordenadas fuera de la imagen
enum SamplerAddressMode : uint64_t {
    SAMPLER_ADDRESS_MODE_CLAMP,
    SAMPLER_ADDRESS_MODE_REPEAT
};

/// @brief Enumeración de las cara de un cubo
enum SamplerCubeFace : uint64_t {
    SAMPLER_CUBE_FACE_LEFT,
    SAMPLER_CUBE_FACE_RIGHT,
    SAMPLER_CUBE_FACE_BACK,
    SAMPLER_CUBE_FACE_FRONT,
    SAMPLER_CUBE_FACE_DOWN,
    SAMPLER_CUBE_FACE_UP
};

/// @brief Muestreado de una imagen 2D
struct Sampler {
    
    Image image;
    SamplerFilter filter;
    SamplerAddressMode address_mode;

    /// @brief Realiza un muestreo directo de la imagen con el filtro de vecinos más cercanos
    /// @param position es la posición dentro de la imagen donde se va a muestrear.
    /// `position [0]` debe estar en el rango `[0, width - 1]` y `position [1]` en `[0, height - 1]`.
    /// @param sample es donde se guardan los valores directos del muestreo, en el rango `[0,255]`
    void raw_sample_nearest (double * position, double * sample);

    /// @brief Realiza un muestreo directo de la imagen con el filtro de interpolación bilineal
    /// @param position es la posición dentro de la imagen donde se va a muestrear.
    /// `position [0]` debe estar en el rango `[0, width - 1]`.
    /// `position [1]` debe estar en el rango `[0, height - 1]`.
    /// @param sample es donde se guardan los valores directos del muestreo.
    /// Los valores escritos en `sample` están en el rangpo `[0,255]`.
    void raw_sample_bilinear (double * position, double * sample);

    /// @brief Realiza un muestreo normalizado de la imagen
    /// @param position es la posición normalizada donde se va  a muestrear.
    /// Los valores en `position` pueden estar fuera del rango normalizado `[0,1]`.
    /// @param sample es donde se guardan los valores del muestreo.
    /// Los valores escritos en `sample` están en el rangpo `[0,1]`.
    void sample (double * position, double * sample);

};

/// @brief Muestreado de un cubo
struct SamplerCube {

    Image cube_map [6];
    SamplerFilter filter;

    /// @brief Realiza un muestreo normalizado del cubo
    /// @param direction es una vector de dirección en 3D, distinto del origen
    /// @param sample es donde se guardan los valores del muestreo.
    /// Los valores escritos en `sample` están en el rangpo `[0,1]`.
    void sample (double * direction, double * sample);

};