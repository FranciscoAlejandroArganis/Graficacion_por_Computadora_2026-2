/// @file gui.hpp

# pragma once

# include "pipeline.hpp"

/// @brief Lienzo donde se muestra la imagen en la ventana
struct Canvas : public QWidget {

    /// @brief Respuesta a un evento de redibujar el lienzo
    /// @param event el evento recibido
    void paintEvent(QPaintEvent * event);

};

/// @brief Puerto para el control de la proyección
struct ViewDock : public QDockWidget {

    QWidget widget;
    QVBoxLayout layout;
    QGroupBox cam_group;
    QVBoxLayout cam_group_layout;
    QRadioButton cam_group_buttons [2];
    QGroupBox proj_group;
    QVBoxLayout proj_group_layout;
    QRadioButton proj_group_buttons [2];

    /// @brief Construye un nuevo puerto
    ViewDock ();

    /// @brief Sincroniza los valores del puerto con los valores de las variables de la aplicación
    void syncValues ();

};

/// @brief Puerto para el control de la transformación
struct TransformDock : public QDockWidget {

    QWidget widget;
    QGridLayout layout;
    QLabel row_labels [3];
    QLabel col_labels [3];
    QDoubleSpinBox pos_boxes [3];
    QDoubleSpinBox rot_boxes [3];
    QDoubleSpinBox scale_boxes [3];

    /// @brief Construye un nuevo puerto
    TransformDock ();

    /// @brief Sincroniza los valores del puerto con los valores de las variables de la aplicación
    void syncValues ();

};

/// @brief Puerto para el control de las luces
struct LightsDock : public QDockWidget {

    QWidget widget;
    QGridLayout layout;
    QComboBox dropdown;
    QCheckBox checkbox;
    QLabel row_labels [3];
    QLabel col_labels [3];
    QDoubleSpinBox amb_boxes [3];
    QDoubleSpinBox diff_boxes [3];
    QDoubleSpinBox spec_boxes [3];

    /// @brief Construye un nuevo puerto
    LightsDock ();

    /// @brief Sincroniza los valores del puerto con los valores de las variables de la aplicación
    void syncValues ();

};

/// @brief Puerto para el control del material
struct MaterialDock : public QDockWidget {

    QWidget widget;
    QGridLayout layout;
    QLabel row_labels [3];
    QLabel col_labels [3];
    QDoubleSpinBox amb_boxes [3];
    QDoubleSpinBox diff_boxes [3];
    QDoubleSpinBox spec_boxes [3];
    QLabel sh_label;
    QDoubleSpinBox sh_box;

    /// @brief Construye un nuevo puerto
    MaterialDock ();

    /// @brief Sincroniza los valores del puerto con los valores de las variables de la aplicación
    void syncValues ();

};

struct PostProcessingDock : public QDockWidget {

    QWidget widget;
    QGridLayout layout;
    QCheckBox checkbox;
    QLabel label;
    QDoubleSpinBox boxes [3] [3];

    PostProcessingDock ();

    void syncValues ();

};

/// @brief Ventana principal de la aplicación
struct Window : public QMainWindow {

    Canvas canvas;
    ViewDock view_dock;
    TransformDock transform_dock;
    LightsDock lights_dock;
    MaterialDock material_dock;
    PostProcessingDock post_processing_dock;

    /// @brief Construye una nueva ventana
    Window ();

    /// @brief Renderiza las escenas vistas por las cámaras y actualiza el lienzo
    void draw ();

    void sync_all ();

    /// @brief Conecta las señales de los puertos
    void connect_all ();

    /// @brief Respuesta a un cambio en la posición x en el puerto de transformación
    /// @param value el nuevo valor x
    void slot_pos_x (double value);

    /// @brief Respuesta a un cambio en la posición y en el puerto de transformación
    /// @param value el nuevo valor y
    void slot_pos_y (double value);

    /// @brief Respuesta a un cambio en la posición z en el puerto de transformación
    /// @param value el nuevo valor z
    void slot_pos_z (double value);

    /// @brief Respuesta a un cambio en la rotación x en el puerto de transformación
    /// @param value el nuevo valor x
    void slot_rot_x (double value);

    /// @brief Respuesta a un cambio en la rotación y en el puerto de transformación
    /// @param value el nuevo valor y
    void slot_rot_y (double value);

    /// @brief Respuesta a un cambio en la rotación z en el puerto de transformación
    /// @param value el nuevo valor z
    void slot_rot_z (double value);

    /// @brief Respuesta a un cambio en la escala x en el puerto de transformación
    /// @param value el nuevo valor x
    void slot_scale_x (double value);

    /// @brief Respuesta a un cambio en la escala y en el puerto de transformación
    /// @param value el nuevo valor y
    void slot_scale_y (double value);

    /// @brief Respuesta a un cambio en la escala z en el puerto de transformación
    /// @param value el nuevo valor z
    void slot_scale_z (double value);

    /// @brief Respuesta a un cambio de la proyección en el puerto de vista
    /// @param checked el nuevo valor de la opción de proyección en perspectiva
    void slot_proj (bool checked);

    /// @brief Respuesta a un cambio de la cámara en el puerto de vista
    /// @param checked el nuevo valor de la opción de cámara 1
    void slot_cam (bool checked);

    /// @brief Resouesta a un cambio de la luz seleccionada en el puerto de luces
    /// @param index el nuevo índice de la luz seleccionada
    void slot_light_id (int index);

    /// @brief Respuesta a un cambio del estado encendido en el puerto de luces
    /// @param state el nuevo valor del estado encendido
    void slot_light_state (Qt::CheckState state);

    /// @brief Respuesta a un cambio en la componente de reflexión ambiental roja en el puerto de luces
    /// @param value el nuevo valor rojo
    void slot_light_amb_r (double value);

    /// @brief Respuesta a un cambio en la componente de reflexión ambiental verde en el puerto de luces
    /// @param value el nuevo valor verde
    void slot_light_amb_g (double value);

    /// @brief Respuesta a un cambio en la componente de reflexión ambiental azul en el puerto de luces
    /// @param value el nuevo valor azul
    void slot_light_amb_b (double value);

    /// @brief Respuesta a un cambio en la componente de reflexión difusa roja en el puerto de luces
    /// @param value el nuevo valor rojo
    void slot_light_diff_r (double value);

    /// @brief Respuesta a un cambio en la componente de reflexión difusa verde en el puerto de luces
    /// @param value el nuevo valor verde
    void slot_light_diff_g (double value);

    /// @brief Respuesta a un cambio en la componente de reflexión difusa azul en el puerto de luces
    /// @param value el nuevo valor azul
    void slot_light_diff_b (double value);

    /// @brief Respuesta a un cambio en la componente de reflexión especular roja en el puerto de luces
    /// @param value el nuevo valor rojo
    void slot_light_spec_r (double value);

    /// @brief Respuesta a un cambio en la componente de reflexión especular verde en el puerto de luces
    /// @param value el nuevo valor verde
    void slot_light_spec_g (double value);

    /// @brief Respuesta a un cambio en la componente de reflexión especular azul en el puerto de luces
    /// @param value el nuevo valor azul
    void slot_light_spec_b (double value);

    /// @brief Respuesta a un cambio en el coeficiente de reflexión ambiental rojo en el puerto de material
    /// @param value el nuevo valor rojo
    void slot_material_amb_r (double value);

    /// @brief Respuesta a un cambio en el coeficiente de reflexión ambiental verde en el puerto de material
    /// @param value el nuevo valor verde
    void slot_material_amb_g (double value);

    /// @brief Respuesta a un cambio en el coeficiente de reflexión ambiental azul en el puerto de material
    /// @param value el nuevo valor azul
    void slot_material_amb_b (double value);

    /// @brief Respuesta a un cambio en el coeficiente de reflexión difusa rojo en el puerto de material
    /// @param value el nuevo valor rojo
    void slot_material_diff_r (double value);

    /// @brief  Respuesta a un cambio en el coeficiente de reflexión difusa verde en el puerto de material
    /// @param value el nuevo valor verde
    void slot_material_diff_g (double value);

    /// @brief Respuesta a un cambio en el coeficiente de reflexión difusa azul en el puerto de material
    /// @param value el nuevo valor azul
    void slot_material_diff_b (double value);

    /// @brief Respuesta a un cambio en el coeficiente de reflexión especular rojo en el puerto de material
    /// @param value el nuevo valor rojo
    void slot_material_spec_r (double value);

    /// @brief Respuesta a un cambio en el coeficiente de reflexión especular verde en el puerto de material
    /// @param value el nuevo valor verde
    void slot_material_spec_g (double value);

    /// @brief Respuesta a un cambio en el coeficiente de reflexión especular azul rojo en el puerto de material
    /// @param value el nuevo valor azul
    void slot_material_spec_b (double value);

    /// @brief Respuesta a un cambio en el brillo especular en el puerto de material
    /// @param value el nuevo valor del brillo
    void slot_material_sh (double value);

    /// @brief Respuesta a un cambio en el estado de aplicar filtro en el puerto de posprocesamiento
    /// @param checked el nuevo valor del estado de aplicar filtro
    void slot_post_processing_filter (Qt::CheckState state);

    /// @brief Respuesta a un cambio en el valor del kernel en el puerto de posprocesamiento
    /// @param value el nuevo valor modificado
    void slot_post_processing_kernel_value (double value);

};
