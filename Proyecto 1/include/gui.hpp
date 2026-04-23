/// @file gui.hpp

# pragma once

# include "pipeline.hpp"

/// @brief Lienzo donde se muestra la imagen en la ventana
struct Canvas : public QWidget {

    /// @brief Respuesta a un evento de redibujar el lienzo
    /// @param event el evento recibido
    void paintEvent(QPaintEvent * event);

};

/// @brief Puerto para el control de la cámara
struct CameraDock : public QDockWidget {

    QWidget widget;
    QVBoxLayout layout;
    QComboBox camera_dropdown;
    QLabel transform_label;
    QWidget group;
    QGridLayout group_layout;
    QLabel row_labels [2];
    QLabel col_labels [3];
    QDoubleSpinBox pos_boxes [3];
    QDoubleSpinBox rot_boxes [3];
    QPushButton button;
    QLabel proj_label;
    QRadioButton proj_buttons [2];
    QLabel skybox_label;
    QComboBox skybox_dropdown;

    /// @brief Construye un nuevo puerto
    CameraDock ();

    /// @brief Sincroniza los valores del puerto con los valores de las variables de la aplicación
    void syncValues ();

};

/// @brief Puerto para el control del objeto
struct ObjectDock : public QDockWidget {

    QWidget widget;
    QVBoxLayout layout;
    QComboBox dropdown;
    QLabel label;
    QWidget group;
    QGridLayout group_layout;
    QLabel row_labels [3];
    QLabel col_labels [3];
    QDoubleSpinBox pos_boxes [3];
    QDoubleSpinBox rot_boxes [3];
    QDoubleSpinBox scale_boxes [3];
    

    /// @brief Construye un nuevo puerto
    ObjectDock ();

    /// @brief Sincroniza los valores del puerto con los valores de las variables de la aplicación
    void syncValues ();

};

/// @brief Puerto para el control de las luces
struct LightDock : public QDockWidget {

    QWidget widget;
    QVBoxLayout layout;
    QComboBox dropdown;
    QCheckBox checkbox;
    QLabel transform_label;
    QWidget transform_group;
    QGridLayout transform_group_layout;
    QLabel transform_row_label;
    QLabel transform_col_labels [3];
    QDoubleSpinBox pos_boxes [3];
    QLabel component_label;
    QWidget component_group;
    QGridLayout component_group_layout;
    QLabel component_row_labels [3];
    QLabel component_col_labels [4];
    QDoubleSpinBox amb_boxes [4];
    QDoubleSpinBox diff_boxes [4];
    QDoubleSpinBox spec_boxes [4];

    /// @brief Construye un nuevo puerto
    LightDock ();

    /// @brief Sincroniza los valores del puerto con los valores de las variables de la aplicación
    void syncValues ();

};

/// @brief Puerto para el control del material
struct MaterialDock : public QDockWidget {

    QWidget widget;
    QVBoxLayout layout;
    QComboBox material_dropdown;
    QLabel coef_label;
    QWidget group;
    QGridLayout group_layout;
    QLabel row_labels [3];
    QLabel col_labels [4];
    QDoubleSpinBox amb_boxes [4];
    QDoubleSpinBox diff_boxes [4];
    QDoubleSpinBox spec_boxes [4];
    QLabel sh_label;
    QDoubleSpinBox sh_box;
    QLabel texture_label;
    QComboBox texture_dropdown;
    QCheckBox checkbox;

    /// @brief Construye un nuevo puerto
    MaterialDock ();

    /// @brief Sincroniza los valores del puerto con los valores de las variables de la aplicación
    void syncValues ();

};

/// @brief Ventana principal de la aplicación
struct Window : public QMainWindow {

    Canvas canvas;
    CameraDock camera_dock;
    ObjectDock object_dock;
    LightDock light_dock;
    MaterialDock material_dock;

    /// @brief Construye una nueva ventana
    Window ();

    /// @brief Renderiza la escena vista por la cámara y actualiza el lienzo
    void draw ();

    /// Sincroniza todos los puertos
    void sync_all ();

    /// @brief Conecta todas las señales de los puertos
    void connect_all ();

    /// @brief Respuesta a un cambio de la cámara seleccioinada en el puerto de cámara
    /// @param index el nuevo índice de la luz seleccionada
    void slot_camera_id (int index);

    /// @brief Respuesta a un cambio en la posición de la cámara
    /// @param value el nuevo valor de la posición
    void slot_camera_pos (double value);

    /// @brief Respuesta a un cambio en la rotación de la cámara
    /// @param value el nuevo valor de la rotación
    void slot_camera_rot (double value);

    /// @brief Respuesta a presionar el botón de orientar la cámara
    /// @param checked el estado del botón si es activable
    void slot_camera_center (bool checked);

    /// @brief Respuesta a un cambio de la proyección en el puerto de vista
    /// @param checked el nuevo valor de la opción de proyección en perspectiva
    void slot_camera_proj (bool checked);

    /// @brief Respuesta a un cambio en el skybox seleccionado
    /// @param index el nuevo índice del objeto seleccionado
    void slot_camera_skybox_id (int index);
    
    /// @brief Respuesta a un cambio en el objeto seleccionado
    /// @param index el nuevo índice del objeto seleccionado
    void slot_object_id (int index);

    /// @brief Respuesta a un cambio en la posición del objeto
    /// @param value el nuevo valor de la posición
    void slot_object_pos (double value);

    /// @brief Respuesta a un cambio en la rotación del objeto
    /// @param value el nuevo valor de la rotación
    void slot_object_rot (double value);

    /// @brief Respuesta a un cambio en el escalamiento del objeto
    /// @param value el nuevo valor del escalamiento
    void slot_object_scale (double value);

    /// @brief Resouesta a un cambio de la luz seleccionada en el puerto de luces
    /// @param index el nuevo índice de la luz seleccionada
    void slot_light_id (int index);

    /// @brief Respuesta a un cambio del estado encendido en el puerto de luces
    /// @param state el nuevo valor del estado encendido
    void slot_light_state (Qt::CheckState state);

    /// @brief Respuesta a un cambio en la posición de la luz seleccionada
    /// @param value el nuevo valor de la posición
    void slot_light_pos (double value);

    /// @brief Respuesta a un cambio en la componente ambiental de la luz seleccionada
    /// @param value el nuevo valor de la componente ambiental
    void slot_light_amb (double value);

    /// @brief Respuesta a un cambio en la componente difusa de la luz seleccionada
    /// @param value el nuevo valor de la componente difusa
    void slot_light_diff (double value);

    /// @brief Respuesta a un cambio en la componente especular de la luz seleccionada
    /// @param value el nuevo valor de la componente especular
    void slot_light_spec (double value);

    /// @brief Respuesta a un cambio del material seleccionado
    /// @param index el nuevo índice de l material seleccionado
    void slot_material_id (int index);

    /// @brief Respuesta a un cambio del coeficiente ambiental del material seleccionado
    /// @param value el nuevo del coeficiente ambiental
    void slot_material_amb (double value);

    /// @brief Respuesta a un cambio del coeficiente difuso del material seleccionado
    /// @param value el nuevo coeficiente difuso
    void slot_material_diff (double value);

    /// @brief Respuesta a un cambio del coeficiente especular del material seleccionado
    /// @param value el nuevo coeficiente especular
    void slot_material_spec (double value);

    /// @brief Respuesta a un cambio en el brillo especular del material seleccionado
    /// @param value el nuevo valor del brillo
    void slot_material_sh (double value);

    /// @brief Respuesta a un cambio de la textura seleccionada
    /// @param index el nuevo índice de la textura seleccionada
    void slot_material_texture_id (int index);

    /// @brief Respuesta a un cambio del estado de la propiedad reflexiva del material
    /// @param state el nuevo valor del estado de de la propiedad reflexiva
    void slot_material_state (Qt::CheckState state);

};
