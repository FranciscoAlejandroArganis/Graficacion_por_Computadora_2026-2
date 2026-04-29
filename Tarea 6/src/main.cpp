# include <iostream>

# include "gui.hpp"

/// @brief Inicia la ejecución del programa
/// @param argc es la cantidad de argumentos de la línea de comandos
/// @param argv son los argumentos de la línea de comandos
/// @return 0 si el programa terminó con éxito
int main (int argc, char * * argv) {

    std::filesystem::path obj_prefix = "obj";
    std::string model_file_names [] = { "bunny.obj", "dragon.obj", "Happy_Buddha.obj", "Teapot.obj", "Cube_Quads.obj", "Sphere_Patches.obj", "Nathan.obj" };
    for (uint64_t i = 0; i < NUM_OBJECTS; i ++) {
        object_id = i;
        if (! load_obj (obj_prefix / model_file_names [i]))  {
            std::cerr << "No se puede cargar el modelo del archivo " << model_file_names [i] << '\n';
            return EXIT_FAILURE;
        }
    }
    std::filesystem::path texture_prefix = "textures";
    std::string texture_file_names [] = { "", "Madera.png", "Ladrillos.png", "Baldosas.png", "Placas.png", "Nathan.png" };
    for (uint64_t i = 1; i < NUM_TEXTURES; i ++) {
        texture_id = i;
        if (! load_texture (texture_prefix / texture_file_names [i])) {
            std::cerr << "No se puede cargar la textura en el archivo " << texture_file_names [i] << '\n';
            return EXIT_FAILURE;
        }
    }
    object_id = 0;
    texture_id = 0;

    QApplication app (argc, argv);

    QSurfaceFormat format;
    format.setRedBufferSize (8);
    format.setGreenBufferSize (8);
    format.setBlueBufferSize (8);
    format.setAlphaBufferSize (8);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setOptions (QSurfaceFormat::DeprecatedFunctions);
    format.setSwapBehavior (QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(format);

    Window window;
    window.canvas.setMinimumSize (400, 400);
    window.setWindowTitle ("Tarea 6");

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
