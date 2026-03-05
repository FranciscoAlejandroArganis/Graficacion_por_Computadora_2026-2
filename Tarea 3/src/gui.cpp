# include "gui.hpp"

void Canvas::paintEvent(QPaintEvent * event) {
    QPainter painter (this);
    painter.drawImage (rect (), image);
}

ProjectionDock::ProjectionDock () :
    QDockWidget ("Proyección"),
    perspective ("Perspectiva"),
    orthogonal ("Ortogonal")
{
    layout.addWidget (& perspective);
    layout.addWidget (& orthogonal);
    layout.addStretch ();
    widget.setLayout (& layout);
    setWidget (& widget);
}

TransformDock::TransformDock () :
    QDockWidget ("Transformación"),
    labels { { QLabel ("Posición"), QLabel ("Rotación"), QLabel ("Escala") }, { QLabel ("x"), QLabel ("y"), QLabel ("z") } }
{
    for (uint64_t i = 0; i < 3; i ++) {
        labels [0] [i].setAlignment (Qt::AlignRight | Qt::AlignVCenter);
        layout.addWidget (labels [0] + i, i + 1, 0);
    }
    for (uint64_t i = 0; i < 3; i ++) {
        labels [1] [i].setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        layout.addWidget (labels [1] + i, 0, i + 1);
    }
    for (uint64_t i = 0; i < 3; i ++) {
        for (uint64_t j = 0; j < 3; j ++)
            layout.addWidget (boxes [i] + j, i + 1, j + 1);
    }
    widget.setLayout (& layout);
    setWidget (& widget);
}

Window::Window () {
    addDockWidget (Qt::TopDockWidgetArea, & projection);
    addDockWidget (Qt::TopDockWidgetArea, & transform);
    setCentralWidget (& canvas);
}

void Window::draw () {
    image.fill (Qt::black);
    transform_euler_zyx_mat4 (cube_position, cube_rotation, cube_scale, model_mat);
    if (perspective_flag)
        perspective_proj_mat4 (ASPECT_RATIO, FOV_V, NEAR, FAR, projection_mat);
    else
        orthogonal_proj_mat4 (ASPECT_RATIO, EXT_V, 0, FAR, projection_mat);
    render_camera (camera_positions [0], viewport_starts [0], viewport_ends [0]);
    render_camera (camera_positions [1], viewport_starts [1], viewport_ends [1]);
    canvas.update ();
}

void Window::slot_pos_x (double value) {
    cube_position [0] = value;
    draw ();
}

void Window::slot_pos_y (double value) {
    cube_position [1] = value;
    draw ();
}

void Window::slot_pos_z (double value) {
    cube_position [2] = value;
    draw ();
}

void Window::slot_rot_x (double value) {
    cube_rotation [0] = value;
    draw ();
}

void Window::slot_rot_y (double value) {
    cube_rotation [1] = value;
    draw ();
}

void Window::slot_rot_z (double value) {
    cube_rotation [2] = value;
    draw ();
}

void Window::slot_scale_x (double value) {
    cube_scale [0] = value;
    draw ();
}

void Window::slot_scale_y (double value) {
    cube_scale [1] = value;
    draw ();
}

void Window::slot_scale_z (double value) {
    cube_scale [2] = value;
    draw ();
}

void Window::slot_proj (bool checked) {
    perspective_flag = checked;
    draw ();
}
