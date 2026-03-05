# pragma once

# include "app.hpp"

/// @brief Lienzo donde se muestra la imagen en la ventana
struct Canvas : public QWidget {

    /// @brief Respuesta a un evento de redibujar el lienzo
    /// @param event el evento recibido
    void paintEvent(QPaintEvent * event);

};

/// @brief Puerto para el control de la proyección
struct ProjectionDock : public QDockWidget {

    QWidget widget;
    QVBoxLayout layout;
    QRadioButton perspective, orthogonal;

    /// @brief Construye un nuevo puerto
    ProjectionDock ();

};

/// @brief Puerto para el control de la transformación
struct TransformDock : public QDockWidget {

    QWidget widget;
    QGridLayout layout;
    QLabel labels [2] [3];
    QDoubleSpinBox boxes [3] [3];

    /// @brief Construye un nuevo puerto
    TransformDock ();

};

/// @brief Ventana principal de la aplicación
struct Window : public QMainWindow {

    Canvas canvas;
    ProjectionDock projection;
    TransformDock transform;

    /// @brief Construye una nueva ventana
    Window ();

    /// @brief Renderiza las escenas vistas por las cámaras y actualiza el lienzo
    void draw ();

    /// @brief Respuesta a un cambio en la posición x en el puerto de transformación
    /// @param value el nuevo valor de la posición x
    void slot_pos_x (double value);

    /// @brief Respuesta a un cambio en la posición y en el puerto de transformación
    /// @param value el nuevo valor de la posición y
    void slot_pos_y (double value);

    /// @brief Respuesta a un cambio en la posición z en el puerto de transformación
    /// @param value el nuevo valor de la posición z
    void slot_pos_z (double value);

    /// @brief Respuesta a un cambio en la rotación x en el puerto de transformación
    /// @param value el nuevo valor de la rotación x
    void slot_rot_x (double value);

    /// @brief Respuesta a un cambio en la rotación y en el puerto de transformación
    /// @param value el nuevo valor de la rotación y
    void slot_rot_y (double value);

    /// @brief Respuesta a un cambio en la rotación z en el puerto de transformación
    /// @param value el nuevo valor de la rotación z
    void slot_rot_z (double value);

    /// @brief Respuesta a un cambio en la escala x en el puerto de transformación
    /// @param value el nuevo valor de la escala x
    void slot_scale_x (double value);

    /// @brief Respuesta a un cambio en la escala y en el puerto de transformación
    /// @param value el nuevo valor de la escala y
    void slot_scale_y (double value);

    /// @brief Respuesta a un cambio en la escala z en el puerto de transformación
    /// @param value el nuevo valor de la escala z
    void slot_scale_z (double value);

    /// @brief Respuesta a un cambio en el puerto de proyección
    /// @param checked el nuevo valor de la opción de proyección en perspectiva
    void slot_proj (bool checked);

};
