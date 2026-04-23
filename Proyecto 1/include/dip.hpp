/// @file dip.hpp

# pragma once

# include <cstdint>

/// @brief Coordenadas de un pixel en una imagen
struct Coord {
    uint64_t x, y;
};

/// @brief Región rectangular de pixeles en una imagen
struct Rect {
    Coord start, end;
};

/// @brief Tamaño de una región rectangular en una imagen
struct Extent {
    uint64_t width, height;
};

/// @brief Imagen con información del tamaño
struct Image {
    Extent extent;
    uint64_t channels;
    void * data;
};

/// @brief Kernel simétrico para operaciones de convolución
struct Kernel {
    uint64_t radius;
    void * data;
};