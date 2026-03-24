# include <iostream>

# include "gui.hpp"

/// @brief Inicia la ejecución del programa
/// @param argc es la cantidad de argumentos de la línea de comandos
/// @param argv son los argumentos de la línea de comandos
/// @return 0 si el programa terminó con éxito
int main (int argc, char * * argv) {

    std::string cube_obj = "Cube_Quads.obj";
    if (! load_obj (cube_obj))  {
        std::cerr << "No se puede cargar el modelo del archivo " << cube_obj << '\n';
        return EXIT_FAILURE;
    }

    QApplication app (argc, argv);

    pipeline = (Pipeline) {
        .num_primitives = vertices.size () / 3,
        .width = WIDTH,
        .height = HEIGHT,
        .viewport = { { 0, 0 }, { WIDTH - 1, HEIGHT - 1 } },
        .color_buffer = color_buffer,
        .depth_buffer = depth_buffer,
        .vertex_buffer = vertices.data (),
        .vertex_shader = vertex_shader,
        .fragment_shader = fragment_shader
    };
    image = QImage (reinterpret_cast <uchar *> (color_buffer), WIDTH, HEIGHT, QImage::Format_RGB32);

    Window window;
    window.canvas.setMinimumSize (WIDTH, HEIGHT);
    window.setWindowTitle ("Tarea 5");

    for (uint64_t i = 0; i < 3; i ++) {
        window.transform_dock.pos_boxes [i].setRange (- 100, 100);
        window.transform_dock.rot_boxes [i].setRange (- 2 * M_PI, 2 * M_PI);
        window.transform_dock.rot_boxes [i].setSingleStep (0.1);
        window.transform_dock.scale_boxes [i].setRange (0.1, 3);
        window.transform_dock.scale_boxes [i].setSingleStep (0.1);

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

        for (uint64_t j = 0; j < 3; j ++) {
            window.post_processing_dock.boxes [i] [j].setRange (-255, 255);
            window.post_processing_dock.boxes [i] [j].setDecimals (3);
            window.post_processing_dock.boxes [i] [j].setSingleStep (0.1);
        }
    }

    window.sync_all ();
    window.connect_all ();

    window.show ();
    window.draw ();

    return app.exec ();

}
