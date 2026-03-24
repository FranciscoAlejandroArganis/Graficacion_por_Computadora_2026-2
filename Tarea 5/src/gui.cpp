# include "gui.hpp"

void Canvas::paintEvent(QPaintEvent * event) {
    QPainter painter (this);
    QRect img_rect = image.rect ();
    img_rect.setSize (image.size ().scaled (size (), Qt::KeepAspectRatio));
    img_rect.moveCenter (rect ().center ());
    painter.drawImage (img_rect, image);
}

ViewDock::ViewDock () :
    QDockWidget ("Vista"),
    cam_group ("Cámara"),
    cam_group_buttons { QRadioButton ("1"), QRadioButton ("2") },
    proj_group ("Proyección"),
    proj_group_buttons { QRadioButton ("Perspectiva"), QRadioButton ("Ortogonal") }
{
    for (uint64_t i = 0; i < 2; i ++) {
        cam_group_layout.addWidget (cam_group_buttons + i);
        proj_group_layout.addWidget (proj_group_buttons + i);
    }
    cam_group.setLayout (& cam_group_layout);
    proj_group.setLayout (& proj_group_layout);
    layout.addWidget (& cam_group);
    layout.addWidget (& proj_group);
    layout.addStretch ();
    widget.setLayout (& layout);
    setWidget (& widget);
}

void ViewDock::syncValues () {
    cam_group_buttons [camera_id].setChecked (true);
    proj_group_buttons [! perspective_flag].setChecked (true);
}

TransformDock::TransformDock () :
    QDockWidget ("Transformación"),
    row_labels { QLabel ("Posición"), QLabel ("Rotación"), QLabel ("Escala") },
    col_labels { QLabel ("x"), QLabel ("y"), QLabel ("z") }
{
    for (uint64_t i = 0; i < 3; i ++) {
        row_labels [i].setAlignment (Qt::AlignRight | Qt::AlignVCenter);
        col_labels [i].setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        layout.addWidget (row_labels + i, i + 1, 0);
        layout.addWidget (col_labels + i, 0, i + 1);
        layout.addWidget (pos_boxes + i, 1, i + 1);
        layout.addWidget (rot_boxes + i, 2, i + 1);
        layout.addWidget (scale_boxes + i, 3, i + 1);
    }
    widget.setLayout (& layout);
    setWidget (& widget);
}

void TransformDock::syncValues () {
    for (uint64_t i = 0; i < 3; i ++) {
        pos_boxes [i].setValue (cube_transform.position [i]);
        rot_boxes [i].setValue (cube_transform.rotation [i]);
        scale_boxes [i].setValue (cube_transform.scale [i]);
    }
}

LightsDock::LightsDock () :
    QDockWidget ("Luces"),
    checkbox ("Encendida"),
    row_labels { QLabel ("Ambiental"), QLabel ("Difuso"), QLabel ("Especular") },
    col_labels { QLabel ("Rojo"), QLabel ("Verde"), QLabel ("Azul") }
{
    dropdown.addItem ("Luz 1");
    dropdown.addItem ("Luz 2");
    layout.addWidget (& dropdown, 0, 0, 1, 4);
    layout.addWidget (& checkbox, 1, 0);
    for (uint64_t i = 0; i < 3; i ++) {
        row_labels [i].setAlignment (Qt::AlignRight | Qt::AlignVCenter);
        col_labels [i].setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        layout.addWidget (row_labels + i, i + 3, 0);
        layout.addWidget (col_labels + i, 2, i + 1);
        layout.addWidget (amb_boxes + i, 3, i + 1);
        layout.addWidget (diff_boxes + i, 4, i + 1);
        layout.addWidget (spec_boxes + i, 5, i + 1);
    }
    widget.setLayout (& layout);
    setWidget (& widget);
}

void LightsDock::syncValues () {
    checkbox.setChecked (active_lights & 1lu << light_id);
    for (uint64_t i = 0; i < 3; i ++) {
        amb_boxes [i].setValue (lights [light_id].ambient [i]);
        diff_boxes [i].setValue (lights [light_id].diffuse [i]);
        spec_boxes [i].setValue (lights [light_id].specular [i]);
    }
}

MaterialDock::MaterialDock () :
    QDockWidget ("Material"),
    row_labels { QLabel ("Ambiental"), QLabel ("Difuso"), QLabel ("Especular") },
    col_labels { QLabel ("Rojo"), QLabel ("Verde"), QLabel ("Azul") },
    sh_label ("Brillo")
{
    for (uint64_t i = 0; i < 3; i ++) {
        row_labels [i].setAlignment (Qt::AlignRight | Qt::AlignVCenter);
        col_labels [i].setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        layout.addWidget (row_labels + i, i + 1, 0);
        layout.addWidget (col_labels + i, 0, i + 1);
        layout.addWidget (amb_boxes + i, 1, i + 1);
        layout.addWidget (diff_boxes + i, 2, i + 1);
        layout.addWidget (spec_boxes + i, 3, i + 1);
    }
    sh_label.setAlignment (Qt::AlignRight | Qt::AlignVCenter);
    layout.addWidget (& sh_label, 5, 0);
    layout.addWidget (& sh_box, 5, 1);
    layout.setRowStretch (4, 1);
    widget.setLayout (& layout);
    setWidget (& widget);
}

void MaterialDock::syncValues () {
    for (uint64_t i = 0; i < 3; i ++) {
        amb_boxes [i].setValue (cube_material.ambient [i]);
        diff_boxes [i].setValue (cube_material.diffuse [i]);
        spec_boxes [i].setValue (cube_material.specular [i]);
    }
    sh_box.setValue (cube_material.shininess);
}

PostProcessingDock::PostProcessingDock () :
    QDockWidget ("Posprocesamiento"),
    checkbox ("Aplicar filtro de convolución"),
    label ("Kernel")
{
    layout.addWidget (& checkbox, 0, 0, 1, 4);
    layout.addWidget (& label, 1, 0);
    for (uint64_t i = 0; i < 3; i ++) {
        for (uint64_t j = 0; j < 3; j ++)
            layout.addWidget (boxes [i] + j, i + 2, j);
    }
    widget.setLayout (& layout);
    setWidget (& widget);
}

void PostProcessingDock::syncValues () {
    double (* data) [3] = reinterpret_cast <double (*) [3]> (kernel.data);
    checkbox.setChecked (filter);
    for (uint64_t i = 0; i < 3; i ++) {
        for (uint64_t j = 0; j < 3; j ++)
            boxes [i] [j].setValue (data [i] [j]);
    }
}

Window::Window () {
    addDockWidget (Qt::TopDockWidgetArea, & view_dock);
    addDockWidget (Qt::TopDockWidgetArea, & transform_dock);
    addDockWidget (Qt::TopDockWidgetArea, & lights_dock);
    addDockWidget (Qt::TopDockWidgetArea, & material_dock);
    addDockWidget (Qt::TopDockWidgetArea, & post_processing_dock);
    setCentralWidget (& canvas);
}

void Window::draw () {
    pipeline.clear_buffers ();
    transform_euler_zyx_mat4 (cube_transform.position, cube_transform.rotation, cube_transform.scale, model_mat);
    camera_look_at_mat4 (camera_positions [camera_id], camera_target, camera_ref, view_mat);
    if (perspective_flag)
        perspective_proj_mat4 (ASPECT_RATIO, FOV_V, NEAR, FAR, projection_mat);
    else
        orthogonal_proj_mat4 (ASPECT_RATIO, EXT_V, 0, FAR, projection_mat);

    double temp [4] [4];
    memcpy (normal_mat, model_mat, 16 * sizeof (double));
    mat_inverse (normal_mat, temp, 4);
    mat_transpose (temp, normal_mat, 4, 4);
    mat4_chain_mul(mvp_chain, 3, mvp_mat);

    pipeline.draw ();
    if (filter) {
        Image image = {
            .extent = { .width = WIDTH, .height = HEIGHT },
            .data = color_buffer
        };
        Rect rect = { .start = { 0, 0 }, .end = { WIDTH - 1, HEIGHT - 1 } };
        convolution_filter (image, rect, kernel, queue);
    }
    canvas.update ();
}

void Window::sync_all () {
    view_dock.syncValues ();
    transform_dock.syncValues ();
    lights_dock.syncValues ();
    material_dock.syncValues ();
    post_processing_dock.syncValues ();
}

void Window::connect_all () {
    connect (view_dock.cam_group_buttons, & QRadioButton::toggled, this, & Window::slot_cam);
    connect (view_dock.proj_group_buttons, & QRadioButton::toggled, this, & Window::slot_proj);

    connect (transform_dock.pos_boxes, & QDoubleSpinBox::valueChanged, this, & Window::slot_pos_x);
    connect (transform_dock.pos_boxes + 1, & QDoubleSpinBox::valueChanged, this, & Window::slot_pos_y);
    connect (transform_dock.pos_boxes + 2, & QDoubleSpinBox::valueChanged, this, & Window::slot_pos_z);
    connect (transform_dock.rot_boxes, & QDoubleSpinBox::valueChanged, this, & Window::slot_rot_x);
    connect (transform_dock.rot_boxes + 1, & QDoubleSpinBox::valueChanged, this, & Window::slot_rot_y);
    connect (transform_dock.rot_boxes + 2, & QDoubleSpinBox::valueChanged, this, & Window::slot_rot_z);
    connect (transform_dock.scale_boxes, & QDoubleSpinBox::valueChanged, this, & Window::slot_scale_x);
    connect (transform_dock.scale_boxes + 1, & QDoubleSpinBox::valueChanged, this, & Window::slot_scale_y);
    connect (transform_dock.scale_boxes + 2, & QDoubleSpinBox::valueChanged, this, & Window::slot_scale_z);

    connect (& lights_dock.dropdown, & QComboBox::activated, this, & Window::slot_light_id);
    connect (& lights_dock.checkbox, & QCheckBox::checkStateChanged, this, & Window::slot_light_state);
    connect (lights_dock.amb_boxes, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_amb_r);
    connect (lights_dock.amb_boxes + 1, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_amb_g);
    connect (lights_dock.amb_boxes + 2, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_amb_b);
    connect (lights_dock.diff_boxes, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_diff_r);
    connect (lights_dock.diff_boxes + 1, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_diff_g);
    connect (lights_dock.diff_boxes + 2, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_diff_b);
    connect (lights_dock.spec_boxes, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_spec_r);;
    connect (lights_dock.spec_boxes + 1, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_spec_g);
    connect (lights_dock.spec_boxes + 2, & QDoubleSpinBox::valueChanged, this, & Window::slot_light_spec_b);

    connect (material_dock.amb_boxes, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_amb_r);
    connect (material_dock.amb_boxes + 1, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_amb_g);
    connect (material_dock.amb_boxes + 2, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_amb_b);
    connect (material_dock.diff_boxes, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_diff_r);
    connect (material_dock.diff_boxes + 1, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_diff_g);
    connect (material_dock.diff_boxes + 2, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_diff_b);
    connect (material_dock.spec_boxes, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_spec_r);
    connect (material_dock.spec_boxes + 1, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_spec_g);
    connect (material_dock.spec_boxes + 2, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_spec_b);
    connect (& material_dock.sh_box, & QDoubleSpinBox::valueChanged, this, & Window::slot_material_sh);

    connect (& post_processing_dock.checkbox, & QCheckBox::checkStateChanged, this, & Window::slot_post_processing_filter);
    for (uint64_t i = 0; i < 3; i ++) {
        for (uint64_t j = 0; j < 3; j ++)
            connect (post_processing_dock.boxes [i] + j, & QDoubleSpinBox::valueChanged, this, & Window::slot_post_processing_kernel_value);
    }
}

void Window::slot_pos_x (double value) {
    cube_transform.position [0] = value;
    draw ();
}

void Window::slot_pos_y (double value) {
    cube_transform.position [1] = value;
    draw ();
}

void Window::slot_pos_z (double value) {
    cube_transform.position [2] = value;
    draw ();
}

void Window::slot_rot_x (double value) {
    cube_transform.rotation [0] = value;
    draw ();
}

void Window::slot_rot_y (double value) {
    cube_transform.rotation [1] = value;
    draw ();
}

void Window::slot_rot_z (double value) {
    cube_transform.rotation [2] = value;
    draw ();
}

void Window::slot_scale_x (double value) {
    cube_transform.scale [0] = value;
    draw ();
}

void Window::slot_scale_y (double value) {
    cube_transform.scale [1] = value;
    draw ();
}

void Window::slot_scale_z (double value) {
    cube_transform.scale [2] = value;
    draw ();
}

void Window::slot_proj (bool checked) {
    perspective_flag = ! perspective_flag;
    draw ();
}

void Window::slot_cam (bool checked) {
    camera_id = 1 - camera_id;
    draw ();
}

void Window::slot_light_id (int index) {
    light_id = index;
    lights_dock.blockSignals (true);
    lights_dock.syncValues ();
    lights_dock.blockSignals (false);
}

void Window::slot_light_state (Qt::CheckState state) {
    if (state == Qt::Checked)
        active_lights |= 1lu << light_id;
    else
        active_lights &= ~ (1lu << light_id);
    draw ();
}

void Window::slot_light_amb_r (double value) {
    lights [light_id].ambient [0] = value;
    draw ();
}

void Window::slot_light_amb_g (double value) {
    lights [light_id].ambient [1] = value;
    draw ();
}

void Window::slot_light_amb_b (double value) {
    lights [light_id].ambient [2] = value;
    draw ();
}

void Window::slot_light_diff_r (double value) {
    lights [light_id].diffuse [0] = value;
    draw ();
}

void Window::slot_light_diff_g (double value) {
    lights [light_id].diffuse [1] = value;
    draw ();
}

void Window::slot_light_diff_b (double value) {
    lights [light_id].diffuse [2] = value;
    draw ();
}

void Window::slot_light_spec_r (double value) {
    lights [light_id].specular [0] = value;
    draw ();
}

void Window::slot_light_spec_g (double value) {
    lights [light_id].specular [1] = value;
    draw ();
}

void Window::slot_light_spec_b (double value) {
    lights [light_id].specular [2] = value;
    draw ();
}

void Window::slot_material_amb_r (double value) {
    cube_material.ambient [0] = value;
    draw ();
}

void Window::slot_material_amb_g (double value) {
    cube_material.ambient [1] = value;
    draw ();
}

void Window::slot_material_amb_b (double value) {
    cube_material.ambient [2] = value;
    draw ();
}

void Window::slot_material_diff_r (double value) {
    cube_material.diffuse [0] = value;
    draw ();
}

void Window::slot_material_diff_g (double value) {
    cube_material.diffuse [1] = value;
    draw ();
}

void Window::slot_material_diff_b (double value) {
    cube_material.diffuse [2] = value;
    draw ();
}

void Window::slot_material_spec_r (double value) {
    cube_material.specular [0] = value;
    draw ();
}

void Window::slot_material_spec_g (double value) {
    cube_material.specular [1] = value;
    draw ();
}

void Window::slot_material_spec_b (double value) {
    cube_material.specular [2] = value;
    draw ();
}

void Window::slot_material_sh (double value) {
    cube_material.shininess = value;
    draw ();
}

void Window::slot_post_processing_filter (Qt::CheckState state) {
    filter = state == Qt::Checked;
    draw ();
}

void Window::slot_post_processing_kernel_value (double value) {
    double (* kernel_data) [3] = reinterpret_cast <double (*) [3]> (kernel.data);
    QObject * obj = sender ();
    for (uint64_t i = 0; i < 3; i ++) {
        for (uint64_t j = 0; j < 3; j ++) {
            if (post_processing_dock.boxes [i] + j == obj) {
                kernel_data [i] [j] = value;
                draw ();
                return;
            }
        }
    }
}
