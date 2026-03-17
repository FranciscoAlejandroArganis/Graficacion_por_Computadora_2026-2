# include "gui.hpp"

/// @brief Buffer de color que forma la imagen mostrada en el lienzo
uint32_t color_buffer [HEIGHT] [WIDTH];

/// @brief Buffer usado en la prueba de profundidad
double depth_buffer [HEIGHT] [WIDTH];

/// @brief Inicia la ejecución del programa
/// @param argc es la cantidad de argumentos de la línea de comandos
/// @param argv son los argumentos de la línea de comandos
/// @return 0 si el programa terminó con éxito
int main (int argc, char * * argv) {

    QApplication app (argc, argv);

    pipeline = (Pipeline) {
        .num_primitives = NUM_TRIANGLES,
        .width = WIDTH,
        .height = HEIGHT,
        .viewport = { { 0, 0 }, { WIDTH - 1, HEIGHT - 1 } },
        .color_buffer = color_buffer,
        .depth_buffer = depth_buffer,
        .vertex_buffer = vertex_buffer,
        .vertex_shader = vertex_shader,
        .fragment_shader = fragment_shader
    };
    image = QImage (reinterpret_cast <uchar *> (color_buffer), WIDTH, HEIGHT, QImage::Format_RGB32);

    Window window;
    window.canvas.setMinimumSize (WIDTH, HEIGHT);
    window.setWindowTitle ("Tarea 4");

    for (uint64_t i = 0; i < 3; i ++) {
        window.transform_dock.pos_boxes [i].setRange (- 100, 100);
        window.transform_dock.rot_boxes [i].setRange (- 2 * M_PI, 2 * M_PI);
        window.transform_dock.rot_boxes [i].setSingleStep (0.1);
        window.transform_dock.scale_boxes [i].setRange (0, 50);

        window.lights_dock.amb_boxes [i].setRange (0, 1);
        window.lights_dock.amb_boxes [i].setSingleStep (0.01);
        window.lights_dock.diff_boxes [i].setRange (0, 1);
        window.lights_dock.diff_boxes [i].setSingleStep (0.01);
        window.lights_dock.spec_boxes [i].setRange (0, 1);
        window.lights_dock.spec_boxes [i].setSingleStep (0.01);

        window.material_dock.amb_boxes [i].setRange (0, 1);
        window.material_dock.amb_boxes [i].setSingleStep (0.01);
        window.material_dock.amb_boxes [i].setDecimals (6);
        window.material_dock.diff_boxes [i].setRange (0, 1);
        window.material_dock.diff_boxes [i].setSingleStep (0.01);
        window.material_dock.diff_boxes [i].setDecimals (6);
        window.material_dock.spec_boxes [i].setRange (0, 1);
        window.material_dock.spec_boxes [i].setSingleStep (0.01);
        window.material_dock.spec_boxes [i].setDecimals (6);
        window.material_dock.sh_box.setRange (1, 1000);
        window.material_dock.sh_box.setSingleStep (10);
    }

    window.view_dock.syncValues ();
    window.transform_dock.syncValues ();
    window.lights_dock.syncValues ();
    window.material_dock.syncValues ();
    window.connect_all ();

    window.show ();
    window.draw ();

    return app.exec ();

}
