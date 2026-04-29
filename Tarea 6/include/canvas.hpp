/// @file canvas.hpp

# pragma once

# include "app.hpp"

/// @brief Lienzo donde se muestra la imagen en la ventana
struct Canvas : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Compatibility {

    // Inicializa el estado y recursos necesarios de OpenGL
    void initializeGL ();

    // Maneja los cambios de tamaño del lienzo
    void resizeGL (int w, int h);

    // Dinuja la escena
    void paintGL ();

    // Actualiza la rotación de la cámara para que apunte hacia la posición del objeto actual
    void center_view ();

};