# include "gui.hpp"

void Canvas::paintEvent(QPaintEvent * event) {
    QPainter painter (this);
    QRect img_rect = image.rect ();
    img_rect.setSize (image.size ().scaled (size (), Qt::KeepAspectRatio));
    img_rect.moveCenter (rect ().center ());
    painter.drawImage (img_rect, image);
}

CameraDock::CameraDock () :
    QDockWidget ("Cámara"),
    transform_label ("Transformación"),
    row_labels { QLabel ("Posición"), QLabel ("Rotación") },
    col_labels { QLabel ("x"), QLabel ("y"), QLabel ("z") },
    button ("Orientar cámara hacia la posición el objeto"),
    proj_label ("Proyección"),
    proj_buttons { QRadioButton ("Perspectiva"), QRadioButton ("Ortogonal") },
    skybox_label ("Skybox")
{
    camera_dropdown.addItem ("Cámara 1");
    camera_dropdown.addItem ("Cámara 2");
    camera_dropdown.addItem ("Cámara 3");
    skybox_dropdown.addItem ("Ninguno");
    skybox_dropdown.addItem ("Cielo");
    skybox_dropdown.addItem ("Espacio");
    skybox_dropdown.addItem ("Yokohama");
    skybox_dropdown.addItem ("Nissi");
    for (uint64_t i = 0; i < 2; i ++) {
        row_labels [i].setAlignment (Qt::AlignRight | Qt::AlignVCenter);
        group_layout.addWidget (row_labels + i, i + 1, 0);
    }
    for (uint64_t i = 0; i < 3; i ++) {
        col_labels [i].setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        group_layout.addWidget (col_labels + i, 0, i + 1);
        group_layout.addWidget (pos_boxes + i, 1, i + 1);
        group_layout.addWidget (rot_boxes + i, 2, i + 1);
    }
    group.setLayout (& group_layout);
    layout.addWidget (& camera_dropdown);
    layout.addWidget (& transform_label);
    layout.addWidget (& group);
    layout.addWidget (& button);
    layout.addWidget (& proj_label);
    for (uint64_t i = 0; i < 2; i ++)
        layout.addWidget (proj_buttons + i);
    layout.addWidget (& skybox_label);
    layout.addWidget (& skybox_dropdown);
    layout.addStretch ();
    widget.setLayout (& layout);
    setWidget (& widget);
}

void CameraDock::syncValues () {
    QSignalBlocker blockers [] {
        QSignalBlocker (pos_boxes [0]), QSignalBlocker (pos_boxes [1]), QSignalBlocker (pos_boxes [2]),
        QSignalBlocker (rot_boxes [0]), QSignalBlocker (rot_boxes [1]), QSignalBlocker (rot_boxes [2]),
        QSignalBlocker (proj_buttons [0]), QSignalBlocker (proj_buttons [1])
    };
    for (uint64_t i = 0; i < 3; i ++) {
        pos_boxes [i].setValue (camera_transforms [camera_id].position [i]);
        rot_boxes [i].setValue (camera_transforms [camera_id].rotation [i]);
    }
    proj_buttons [! perspective_flag].setChecked (true);
}

ObjectDock::ObjectDock () :
    QDockWidget ("Objeto"),
    label ("Transformación"),
    row_labels { QLabel ("Posición"), QLabel ("Rotación"), QLabel ("Escala") },
    col_labels { QLabel ("x"), QLabel ("y"), QLabel ("z") }
{
    dropdown.addItem ("Conejo");
    dropdown.addItem ("Dragón");
    dropdown.addItem ("Buda");
    dropdown.addItem ("Tetera");
    dropdown.addItem ("Cubo");
    dropdown.addItem ("Esfera");
    dropdown.addItem ("Nathan");
    for (uint64_t i = 0; i < 3; i ++) {
        row_labels [i].setAlignment (Qt::AlignRight | Qt::AlignVCenter);
        col_labels [i].setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        group_layout.addWidget (row_labels + i, i + 1, 0);
        group_layout.addWidget (col_labels + i, 0, i + 1);
        group_layout.addWidget (pos_boxes + i, 1, i + 1);
        group_layout.addWidget (rot_boxes + i, 2, i + 1);
        group_layout.addWidget (scale_boxes + i, 3, i + 1);
    }
    group.setLayout (& group_layout);
    layout.addWidget (& dropdown);
    layout.addWidget (& label);
    layout.addWidget (& group);
    layout.addStretch ();
    widget.setLayout (& layout);
    setWidget (& widget);
}

void ObjectDock::syncValues () {
    QSignalBlocker blockers [] {
        QSignalBlocker (pos_boxes [0]), QSignalBlocker (pos_boxes [1]), QSignalBlocker (pos_boxes [2]),
        QSignalBlocker (rot_boxes [0]), QSignalBlocker (rot_boxes [1]), QSignalBlocker (rot_boxes [2]),
        QSignalBlocker (scale_boxes [0]), QSignalBlocker (scale_boxes [1]), QSignalBlocker (scale_boxes [2])
    };
    for (uint64_t i = 0; i < 3; i ++) {
        pos_boxes [i].setValue (object_transforms [object_id].position [i]);
        rot_boxes [i].setValue (object_transforms [object_id].rotation [i]);
        scale_boxes [i].setValue (object_transforms [object_id].scale [i]);
    }
}

LightDock::LightDock () :
    QDockWidget ("Luces"),
    checkbox ("Encendida"),
    transform_label ("Transformación"),
    transform_row_label ("Posición"),
    transform_col_labels { QLabel ("x"), QLabel ("y"), QLabel ("z") },
    component_label ("Componentes"),
    component_row_labels { QLabel ("Ambiental"), QLabel ("Difuso"), QLabel ("Especular") },
    component_col_labels { QLabel ("Rojo"), QLabel ("Verde"), QLabel ("Azul"), QLabel ("Alfa") }
{
    dropdown.addItem ("Luz 1");
    dropdown.addItem ("Luz 2");
    transform_row_label.setAlignment (Qt::AlignRight | Qt::AlignVCenter);
    transform_group_layout.addWidget (& transform_row_label, 1, 0);
    for (uint64_t i = 0; i < 3; i ++) {
        transform_col_labels [i].setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        transform_group_layout.addWidget (transform_col_labels + i, 0, i + 1);
        transform_group_layout.addWidget (pos_boxes + i, 1, i + 1);
    }
    transform_group.setLayout (& transform_group_layout);
    for (uint64_t i = 0; i < 3; i ++) {
        component_row_labels [i].setAlignment (Qt::AlignRight | Qt::AlignVCenter);
        component_group_layout.addWidget (component_row_labels + i, i + 1, 0);
    }
    for (uint64_t i = 0; i < 4; i ++) {
        component_col_labels [i].setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        component_group_layout.addWidget (component_col_labels + i, 0, i + 1);
        component_group_layout.addWidget (amb_boxes + i, 1, i + 1);
        component_group_layout.addWidget (diff_boxes + i, 2, i + 1);
        component_group_layout.addWidget (spec_boxes + i, 3, i + 1);
    }
    component_group.setLayout (& component_group_layout);
    layout.addWidget (& dropdown);
    layout.addWidget (& checkbox);
    layout.addWidget (& transform_label);
    layout.addWidget (& transform_group);
    layout.addWidget (& component_label);
    layout.addWidget (& component_group);
    layout.addStretch ();
    widget.setLayout (& layout);
    setWidget (& widget);
}

void LightDock::syncValues () {
    QSignalBlocker blockers [] {
        QSignalBlocker (checkbox),
        QSignalBlocker (pos_boxes [0]), QSignalBlocker (pos_boxes [1]), QSignalBlocker (pos_boxes [2]),
        QSignalBlocker (amb_boxes [0]), QSignalBlocker (amb_boxes [1]), QSignalBlocker (amb_boxes [2]), QSignalBlocker (amb_boxes [3]),
        QSignalBlocker (diff_boxes [0]), QSignalBlocker (diff_boxes [1]), QSignalBlocker (diff_boxes [2]), QSignalBlocker (diff_boxes [3]),
        QSignalBlocker (spec_boxes [0]), QSignalBlocker (spec_boxes [1]), QSignalBlocker (spec_boxes [2]), QSignalBlocker (spec_boxes [3])
    };
    checkbox.setChecked (active_lights & 1lu << light_id);
    for (uint64_t i = 0; i < 3; i ++)
        pos_boxes [i].setValue (lights [light_id].position [i]);
    for (uint64_t i = 0; i < 4; i ++) {
        amb_boxes [i].setValue (lights [light_id].ambient [i]);
        diff_boxes [i].setValue (lights [light_id].diffuse [i]);
        spec_boxes [i].setValue (lights [light_id].specular [i]);
    }
}

MaterialDock::MaterialDock () :
    QDockWidget ("Materiales"),
    coef_label ("Coeficientes"),
    row_labels { QLabel ("Ambiental"), QLabel ("Difuso"), QLabel ("Especular") },
    col_labels { QLabel ("Rojo"), QLabel ("Verde"), QLabel ("Azul"), QLabel ("Alfa") },
    sh_label ("Brillo"),
    texture_label ("Textura"),
    checkbox ("Reflexivo")
{
    material_dropdown.addItem ("Material 1");
    material_dropdown.addItem ("Material 2");
    texture_dropdown.addItem ("Ninguna");
    texture_dropdown.addItem ("Madera");
    texture_dropdown.addItem ("Ladrillo");
    texture_dropdown.addItem ("Baldosas de piedra");
    texture_dropdown.addItem ("Placas de metal");
    texture_dropdown.addItem ("Nathan");
    for (uint64_t i = 0; i < 3; i ++) {
        row_labels [i].setAlignment (Qt::AlignRight | Qt::AlignVCenter);
        group_layout.addWidget (row_labels + i, i + 1, 0);
    }
    for (uint64_t i = 0; i < 4; i ++) {
        col_labels [i].setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        group_layout.addWidget (col_labels + i, 0, i + 1);
        group_layout.addWidget (amb_boxes + i, 1, i + 1);
        group_layout.addWidget (diff_boxes + i, 2, i + 1);
        group_layout.addWidget (spec_boxes + i, 3, i + 1);
    }
    sh_label.setAlignment (Qt::AlignRight | Qt::AlignVCenter);
    group_layout.addWidget (& sh_label, 4, 0);
    group_layout.addWidget (& sh_box, 4, 1);
    group.setLayout (& group_layout);
    layout.addWidget (& material_dropdown);
    layout.addWidget (& coef_label);
    layout.addWidget (& group);
    layout.addWidget (& texture_label);
    layout.addWidget (& texture_dropdown);
    layout.addWidget (& checkbox);
    layout.addStretch ();
    widget.setLayout (& layout);
    setWidget (& widget);
}

void MaterialDock::syncValues () {
    QSignalBlocker blockers [] {
        QSignalBlocker (amb_boxes [0]), QSignalBlocker (amb_boxes [1]), QSignalBlocker (amb_boxes [2]), QSignalBlocker (amb_boxes [3]),
        QSignalBlocker (diff_boxes [0]), QSignalBlocker (diff_boxes [1]), QSignalBlocker (diff_boxes [2]), QSignalBlocker (diff_boxes [3]),
        QSignalBlocker (spec_boxes [0]), QSignalBlocker (spec_boxes [1]), QSignalBlocker (spec_boxes [2]), QSignalBlocker (spec_boxes [3]),
        QSignalBlocker (sh_box),
        QSignalBlocker (checkbox)
    };
    for (uint64_t i = 0; i < 4; i ++) {
        amb_boxes [i].setValue (materials [material_id].ambient [i]);
        diff_boxes [i].setValue (materials [material_id].diffuse [i]);
        spec_boxes [i].setValue (materials [material_id].specular [i]);
    }
    sh_box.setValue (materials [material_id].shininess);
    checkbox.setChecked (reflective_material_flag);
}

Window::Window () {
    addDockWidget (Qt::TopDockWidgetArea, & camera_dock);
    addDockWidget (Qt::TopDockWidgetArea, & object_dock);
    addDockWidget (Qt::TopDockWidgetArea, & light_dock);
    addDockWidget (Qt::TopDockWidgetArea, & material_dock);
    setCentralWidget (& canvas);
}

void Window::draw () {
    render_scene ();
    canvas.update ();
}

void Window::sync_all () {
    camera_dock.syncValues ();
    object_dock.syncValues ();
    light_dock.syncValues ();
    material_dock.syncValues ();
}

void Window::connect_all () {
    connect (& camera_dock.camera_dropdown, & QComboBox::activated, this, & Window::slot_camera_id);
    connect (& camera_dock.button, & QPushButton::clicked, this, & Window::slot_camera_center);
    connect (camera_dock.proj_buttons, & QRadioButton::toggled, this, & Window::slot_camera_proj);
    connect (& camera_dock.skybox_dropdown, & QComboBox::activated, this, & Window::slot_camera_skybox_id);
    connect (& object_dock.dropdown, & QComboBox::activated, this, & Window::slot_object_id);
    connect (& light_dock.dropdown, & QComboBox::activated, this, & Window::slot_light_id);
    connect (& light_dock.checkbox, & QCheckBox::checkStateChanged, this, & Window::slot_light_state);
    connect (& material_dock.material_dropdown, & QComboBox::activated, this, & Window::slot_material_id);
    connect (& material_dock.sh_box, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_sh);
    connect (& material_dock.texture_dropdown, & QComboBox::activated, this, & Window::slot_material_texture_id);
    connect (& material_dock.checkbox, & QCheckBox::checkStateChanged, this, & Window::slot_material_state);
    for (uint64_t i = 0; i < 3; i ++) {
        connect (camera_dock.pos_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_camera_pos);
        connect (camera_dock.rot_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_camera_rot);
        connect (object_dock.pos_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_object_pos);
        connect (object_dock.rot_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_object_rot);
        connect (object_dock.scale_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_object_scale);
        connect (light_dock.pos_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_pos);
    }
    for (uint64_t i = 0; i < 4; i ++) {
        connect (light_dock.amb_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_amb);
        connect (light_dock.diff_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_diff);
        connect (light_dock.spec_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_spec);
        connect (material_dock.amb_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_amb);
        connect (material_dock.diff_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_diff);
        connect (material_dock.spec_boxes + i, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_spec);
    }
}

void Window::slot_camera_id (int index) {
    camera_id = index;
    camera_dock.syncValues ();
    draw ();
}

void Window::slot_camera_pos (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 3; i ++) {
        if (obj == camera_dock.pos_boxes + i) {
            camera_transforms [camera_id].position [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_camera_rot (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 3; i ++) {
        if (obj == camera_dock.rot_boxes + i) {
            camera_transforms [camera_id].rotation [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_camera_center (bool checked) {
    center_view ();
    camera_dock.syncValues ();
    draw ();
}

void Window::slot_camera_proj (bool checked) {
    perspective_flag = ! perspective_flag;
    draw ();
}

void Window::slot_camera_skybox_id (int index) {
    skybox_id = index;
    if (index > 0 && ! load_cube_map (skybox_dir_names [index])) {
        std::cerr << "No se puede cargar el skybox en la carpeta " << skybox_dir_names [index] << '\n';
        QCoreApplication::exit (EXIT_FAILURE);
        return;
    }
    draw ();
}

void Window::slot_object_id (int index) {
    object_id = index;
    if (! load_obj (model_file_names [index])) {
        std::cerr << "No se puede cargar el modelo en el archivo " << model_file_names [index] << '\n';
        QCoreApplication::exit (EXIT_FAILURE);
        return;
    }
    object_dock.syncValues ();
    draw ();
}

void Window::slot_object_pos (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 3; i ++) {
        if (obj == object_dock.pos_boxes + i) {
            object_transforms [object_id].position [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_object_rot (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 3; i ++) {
        if (obj == object_dock.rot_boxes + i) {
            object_transforms [object_id].rotation [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_object_scale (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 3; i ++) {
        if (obj == object_dock.scale_boxes + i) {
            object_transforms [object_id].scale [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_light_id (int index) {
    light_id = index;
    light_dock.syncValues ();
}

void Window::slot_light_state (Qt::CheckState state) {
    if (state == Qt::Checked)
        active_lights |= 1lu << light_id;
    else
        active_lights &= ~ (1lu << light_id);
    draw ();
}

void Window::slot_light_pos (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 3; i ++) {
        if (obj == light_dock.pos_boxes + i) {
            lights [light_id].position [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_light_amb (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 4; i ++) {
        if (obj == light_dock.amb_boxes + i) {
            lights [light_id].ambient [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_light_diff (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 4; i ++) {
        if (obj == light_dock.diff_boxes + i) {
            lights [light_id].diffuse [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_light_spec (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 4; i ++) {
        if (obj == light_dock.spec_boxes + i) {
            lights [light_id].specular [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_material_id (int index) {
    material_id = index;
    material_dock.syncValues ();
    draw ();
}

void Window::slot_material_amb (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 4; i ++) {
        if (obj == material_dock.amb_boxes + i) {
            materials [material_id].ambient [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_material_diff (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 4; i ++) {
        if (obj == material_dock.diff_boxes + i) {
            materials [material_id].diffuse [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_material_spec (double value) {
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 4; i ++) {
        if (obj == material_dock.spec_boxes + i) {
            materials [material_id].specular [i] = value;
            draw ();
            return;
        }
    }
}

void Window::slot_material_sh (double value) {
    materials [material_id].shininess = value;
    draw ();
}

void Window::slot_material_texture_id (int index) {
    texture_id = index;
    if (texture_id > 0 && ! load_texture (texture_file_names [index])) {
        std::cerr << "No se puede cargar la textura en el archivo " << texture_file_names [index] << '\n';
        QCoreApplication::exit (EXIT_FAILURE);
        return;
    }
    draw ();
}

void Window::slot_material_state (Qt::CheckState state) {
    reflective_material_flag = state == Qt::Checked;
    draw ();
}