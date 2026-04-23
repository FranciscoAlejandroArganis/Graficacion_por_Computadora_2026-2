# include "gui.hpp"

# include "sampler.hpp"

/// @brief Inicia la ejecución del programa
/// @param argc es la cantidad de argumentos de la línea de comandos
/// @param argv son los argumentos de la línea de comandos
/// @return 0 si el programa terminó con éxito
int main (int argc, char * * argv) {

    if (! load_obj (model_file_names [object_id]))  {
        std::cerr << "No se puede cargar el modelo del archivo " << model_file_names [object_id] << '\n';
        return EXIT_FAILURE;
    }

    QApplication app (argc, argv);

    pipeline = (Pipeline) {
        .num_primitives = 2,
        .width = WIDTH,
        .height = HEIGHT,
        .viewport = { { 0, 0 }, { WIDTH - 1, HEIGHT - 1 } },
        .color_buffer = color_buffer,
        .depth_buffer = depth_buffer,
        .invert_y_viewport = true
    };
    image = QImage (reinterpret_cast <uchar *> (color_buffer), WIDTH, HEIGHT, QImage::Format_RGB32);

    Window window;
    window.canvas.setMinimumSize (WIDTH, HEIGHT);
    window.setWindowTitle ("Proyecto 1");

    for (uint64_t i = 0; i < 3; i ++) {
        window.camera_dock.pos_boxes [i].setRange (- 1000, 1000);
        window.camera_dock.rot_boxes [i].setRange (- 2 * M_PI, 2 * M_PI);
        window.camera_dock.rot_boxes [i].setSingleStep (0.1);
        window.object_dock.pos_boxes [i].setRange (- 1000, 1000);
        window.object_dock.rot_boxes [i].setRange (- 2 * M_PI, 2 * M_PI);
        window.object_dock.rot_boxes [i].setSingleStep (0.1);
        window.object_dock.scale_boxes [i].setRange (0, 1000);
        window.object_dock.scale_boxes [i].setSingleStep (0.1);
        window.light_dock.pos_boxes [i].setRange (- 1000, 1000);
    }
    for (uint64_t i = 0; i < 4; i ++) {
        window.light_dock.amb_boxes [i].setRange (0, 1);
        window.light_dock.amb_boxes [i].setSingleStep (0.01);
        window.light_dock.diff_boxes [i].setRange (0, 1);
        window.light_dock.diff_boxes [i].setSingleStep (0.01);
        window.light_dock.spec_boxes [i].setRange (0, 1);
        window.light_dock.spec_boxes [i].setSingleStep (0.01);
        window.material_dock.amb_boxes [i].setRange (0, 1);
        window.material_dock.amb_boxes [i].setSingleStep (0.01);
        window.material_dock.amb_boxes [i].setDecimals (6);
        window.material_dock.diff_boxes [i].setRange (0, 1);
        window.material_dock.diff_boxes [i].setSingleStep (0.01);
        window.material_dock.diff_boxes [i].setDecimals (6);
        window.material_dock.spec_boxes [i].setRange (0, 1);
        window.material_dock.spec_boxes [i].setSingleStep (0.01);
        window.material_dock.spec_boxes [i].setDecimals (6);
    }
    window.material_dock.sh_box.setRange (1, 1000);

    window.sync_all ();
    window.connect_all ();

    window.show ();
    window.draw ();

    return app.exec ();

}
