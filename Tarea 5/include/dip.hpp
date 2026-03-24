/// @file dip.hpp

# pragma once

# include <cstdint>
# include <cmath>

# include <QColor>

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
    void * data;
};

/// @brief Kernel simétrico para operaciones de convolución
struct Kernel {
    uint64_t radius;
    void * data;
};

/// @brief Convierte un valor flotante a un componente entero de color de 8 bits
/// @param value el valor flotante
/// @return el componente de color correspondiente
uint8_t to_color_component (double value);

/// @brief Calcula la suma de los productos de cada elemento de `image` en `window` por el elemento correspondiente en `kernel`
/// @param image es la imagen
/// @param window es la ventana
/// @param kernel es el kernel
/// @param center son las coordenadas del centro de la ventana en la imagen
/// @return la suma de los productos de cada elemento en `window` por el elemento correspondiente en `kernel`
uint32_t apply_kernel (Image image, Rect window, Kernel kernel, Coord center);

/// @brief Construye una ventana con centro en un pixel dado para aplicar un kernel
/// @param rect es la región de la imagen donde se aplica la operación de convolución
/// @param center son las coordenadas del centro de la ventana en la imagen
/// @param radius es el radio del kernel
/// @return una ventana centrada en `center` para un kernel de radio `radius` que no se sale de la región `rect`
Rect build_kernel_window (Rect rect, Coord center, uint64_t radius);

/// Aplica un filtro de convolución en orden primero horizontal
/// @param image es la imagen
/// @param rect es la región de la imagen donde se aplica el filtro
/// @param kernel es el kernel
/// @param queue es una cola de capacidad al menos `kernel.radius * (image.extent.width + 1)`
void convolution_filter_h (Image image, Rect rect, Kernel kernel, uint32_t * queue);

/// Aplica un filtro de convolución en orden primero vertical
/// @param image es la imagen
/// @param rect es la región de la imagen donde se aplica el filtro
/// @param kernel es el kernel
/// @param queue es una cola de capacidad al menos `kernel.radius * (image.extent.height + 1)`
void convolution_filter_v (Image image, Rect rect, Kernel kernel, uint32_t * queue);

/// Aplica un filtro de convolución
/// @param image es la imagen
/// @param rect es la región de la imagen donde se aplica el filtro
/// @param kernel es el kernel
/// @param queue es una cola de capacidad al menos `kernel.radius * (min (image.extent.width, image.extent.height) + 1)`
void convolution_filter (Image image, Rect rect, Kernel kernel, uint32_t * queue);
