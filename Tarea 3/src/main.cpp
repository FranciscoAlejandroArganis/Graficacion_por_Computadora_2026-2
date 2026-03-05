# include "gui.hpp"

/// @brief Inicia la ejecución del programa
/// @param argc es la cantidad de argumentos de la línea de comandos
/// @param argv son los argumentos de la línea de comandos
/// @return 0 si el programa terminó con éxito
int main (int argc, char * * argv) {
    QApplication app (argc, argv);

    Window window;
    window.canvas.setMinimumSize (WIDTH, HEIGHT);
    window.setWindowTitle ("Tarea 3");

    for (uint64_t i = 0; i < 3; i ++) {
        window.transform.boxes [0] [i].setRange (- 100, 100);
        window.transform.boxes [0] [i].setValue (cube_position [i]);
        window.transform.boxes [1] [i].setRange (- 2 * M_PI, 2 * M_PI);
        window.transform.boxes [1] [i].setSingleStep (0.1);
        window.transform.boxes [1] [i].setValue (cube_rotation [i]);
        window.transform.boxes [2] [i].setRange (- 100, 100);
        window.transform.boxes [2] [i].setValue (cube_scale [i]);
    }
    if (perspective_flag)
        window.projection.perspective.setChecked (true);
    else
        window.projection.orthogonal.setChecked (true);

    window.connect (window.transform.boxes [0], & QDoubleSpinBox::valueChanged, & window, & Window::slot_pos_x);
    window.connect (window.transform.boxes [0] + 1, & QDoubleSpinBox::valueChanged, & window, & Window::slot_pos_y);
    window.connect (window.transform.boxes [0] + 2, & QDoubleSpinBox::valueChanged, & window, & Window::slot_pos_z);
    window.connect (window.transform.boxes [1], & QDoubleSpinBox::valueChanged, & window, & Window::slot_rot_x);
    window.connect (window.transform.boxes [1] + 1, & QDoubleSpinBox::valueChanged, & window, & Window::slot_rot_y);
    window.connect (window.transform.boxes [1] + 2, & QDoubleSpinBox::valueChanged, & window, & Window::slot_rot_z);
    window.connect (window.transform.boxes [2], & QDoubleSpinBox::valueChanged, & window, & Window::slot_scale_x);
    window.connect (window.transform.boxes [2] + 1, & QDoubleSpinBox::valueChanged, & window, & Window::slot_scale_y);
    window.connect (window.transform.boxes [2] + 2, & QDoubleSpinBox::valueChanged, & window, & Window::slot_scale_z);
    window.connect (& window.projection.perspective, & QRadioButton::toggled, & window, & Window::slot_proj);

    window.show ();
    window.draw ();

    return app.exec ();
}
