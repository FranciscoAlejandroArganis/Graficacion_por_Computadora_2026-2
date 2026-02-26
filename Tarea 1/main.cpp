# include <QtWidgets>

# define WIDTH 512
# define HEIGHT 512
# define NUM_PELLETS 300
# define UPDATE_TIME_MS 10

/// @brief Limita un valor objetivo dentro de un intervalo
/// @param value es el valor objetivo
/// @param min es el mínimo del intervalo
/// @param max es el máximo del intervalo
/// @return el valor si se encuentra en el intervalo o, si está fuera, el resultado de considerar que salir por un extremo del intervalo es entrar por el otro extremo
double circ (double value, double min, double max) {
    if (value < min)
        return value + (max - min) * (floor ((min - value) / (max - min)) + 1);
    if (value > max)
        return value - (max - min) * (floor ((value - max) / (max - min)) + 1);
    return value;
}

/// @brief Representa un punto con color que se mueve
struct Pellet {

    double x, y, red, green, blue, direction, speed;

    /// @brief Asigna valores aleatorios para la posición, color, velocidad y dirección del punto
    void randomize () {
        x = rand () % WIDTH;
        y = rand () % HEIGHT;
        red = (double) rand () / RAND_MAX;
        green = (double) rand () / RAND_MAX;
        blue = (double) rand () / RAND_MAX;
        speed = (double) rand () / RAND_MAX;
        direction = 2 * M_PI * (double) rand () / RAND_MAX;
    }

    /// @brief Actualiza la posición del punto
    void update () {
        x += speed * cos (direction);
        x = circ (x, 0, WIDTH - 1);
        y += speed * sin (direction);
        y = circ (y, 0, HEIGHT - 1);
    }

};

QImage image;
QTimer timer;
Pellet pellets [NUM_PELLETS];

/// @brief Representa la ventana
struct Window : public QWidget {

    /// @brief Actualiza los puntos y dibuja la escena en la imagen
    void draw () {
        image.fill (Qt::black);
        for (uint64_t i = 0; i < NUM_PELLETS; i ++) {
            pellets [i].update ();
            QColor color (
                round (pellets [i].red * 255),
                round (pellets [i].green * 255),
                round (pellets [i].blue * 255)
            );
            image.setPixelColor (
                round (pellets [i].x),
                round (pellets [i].y),
                color
            );
        }
        update ();
    }

    /// @brief Respuesta a un evento de redibujar la ventana
    /// @param event el evento recibido
    void paintEvent (QPaintEvent * event) {
        QPainter painter (this);
        painter.drawImage (rect (), image);
    }

};

/// @brief Inicia la ejecución del programa
/// @param argc es la cantidad de argumentos de la línea de comandos
/// @param argv son los argumentos de la línea de comandos
/// @return 0 si el programa terminó con éxito
int main (int argc, char * * argv) {
    srand (time (NULL));

    QApplication app (argc, argv);
    Window window;
    window.resize (WIDTH, HEIGHT);
    window.setWindowTitle ("Tarea 1");
    window.show ();

    image = QImage (WIDTH, HEIGHT, QImage::Format_RGB32);
    image.fill (Qt::black);
    for (uint64_t i = 0; i < NUM_PELLETS; i ++)
        pellets [i].randomize ();
    window.connect (& timer, & QTimer::timeout, & window, & Window::draw);
    timer.start (UPDATE_TIME_MS);

    return app.exec ();
}
