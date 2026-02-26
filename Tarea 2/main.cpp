# include <QtWidgets>
# include <iostream>

# define WIDTH 457
# define HEIGHT 600

/// @brief Arreglo con los pixeles desde el cual se despliega una imagen en la ventana
QRgb buffer [HEIGHT] [WIDTH];

/// @brief Representa la ventana
struct Window : public QWidget {

    /// @brief Objeto pixmap usado para mostrar una imagen en la ventana
    QPixmap pixmap;

    /// @brief Respuesta a un evento de redibujar la ventana
    /// @param event el evento recibido
    void paintEvent(QPaintEvent * event) {
        QPainter painter (this);
        painter.drawPixmap (rect (), pixmap);
    }

};

/// @brief Sustituye pixeles en una imagen que tienen cierto valor
/// @param image es la imagen
/// @param old_rgb es el valor original que deben tener los pixeles para ser sustituidos
/// @param new_rgb es el nuevo valor que toman los pixeles sustituidos
void image_rgb_subst (QImage * image, QRgb old_rgb, QRgb new_rgb) {
    for (uint64_t y = 0; y < image -> height (); y ++) {
        QRgb * line = reinterpret_cast <QRgb *> (image -> scanLine (y));
        for (uint64_t x = 0; x < image -> width (); x ++) {
            if (line [x] == old_rgb)
                line [x] = new_rgb;
        }
    }
}

/// @brief Llena un arreglo de pixeles con el contenido de un objeto QImage
/// @param image es la imagen
/// @param data es el arreglo de pixeles
void raw_pixels_from_image (QImage * image, void * data) {
    for (uint64_t y = 0; y < image -> height (); y ++)
        memcpy (buffer [y], image -> scanLine (y), image -> width () * sizeof (QRgb));
}

/// @brief Construye un objeto QPixmap a partir de un arreglo de pixeles
/// @param data es el arreglo de pixeles
/// @param width es el ancho de los pixeles
/// @param height es el alto de los pixeles
/// @return el objeto QPixmap con los datos de los pixeles
QPixmap pixmap_from_raw_pixels (void * data, uint64_t width, uint64_t height) {
    return QPixmap::fromImage (QImage (reinterpret_cast <uchar *> (data), width, height, QImage::Format_RGB32));
}

/// @brief Inicia la ejecución del programa
/// @param argc es la cantidad de argumentos de la línea de comandos
/// @param argv son los argumentos de la línea de comandos
/// @return 0 si el programa terminó con éxito
int main (int argc, char * * argv) {
    QApplication app (argc, argv);
    Window window;
    window.resize (WIDTH, HEIGHT);
    window.setWindowTitle ("Tarea 2");
    window.show ();

    QImage image = QImage ("Pixar_UP.png");
    if (image.isNull ()) {
        std::cerr << "Error: No se puede leer la imagen Pixar_UP.png\n";
        return EXIT_FAILURE;
    }
    image_rgb_subst (& image, qRgb (65, 142, 202), qRgb (0, 0, 0));
    image = image.scaled (WIDTH, HEIGHT);
    raw_pixels_from_image (& image, buffer);
    window.pixmap = pixmap_from_raw_pixels (buffer, WIDTH, HEIGHT);
    window.update ();

    return app.exec ();
}