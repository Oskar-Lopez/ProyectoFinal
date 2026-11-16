#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), enPresentacion(true), juegoCompletado(false) {
    setFixedSize(1280, 720);
    setWindowTitle("Clark Kent: Acuerdo de Paz 2016 - UdeA");

    nivelActual = new Nivel(1);

    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &MainWindow::actualizarFrame);
    timerJuego->start(16);
}

MainWindow::~MainWindow() {
    delete nivelActual;
}

void MainWindow::actualizarFrame() {
    if (!enPresentacion && !juegoCompletado && nivelActual != nullptr) {
        if (nivelActual->verificarVictoria()) {
            if (nivelActual->getNumeroNivel() == 1) {
                delete nivelActual;
                nivelActual = new Nivel(2);
                nivelActual->cargarNivel();
            } else {
                juegoCompletado = true;
            }
        }
    }
    update();
}

void MainWindow::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setFont(QFont("Courier", 16, QFont::Bold));

    if (enPresentacion) {
        painter.fillRect(rect(), QColor(24, 34, 48));
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter,
                         "CLARK KENT: EL UNIVERSAL DE CARTAGENA\n\nPresiona ENTER para iniciar");
        return;
    }

    if (juegoCompletado) {
        painter.fillRect(rect(), QColor(20, 60, 40));
        painter.setPen(Qt::yellow);
        painter.drawText(rect(), Qt::AlignCenter,
                         "¡ACUERDO FIRMADO!\n\nPresiona R para volver al inicio");
        return;
    }

    if (nivelActual->getNumeroNivel() == 1) {
        // Escena provisional Nivel 1: Side-scroller Bogotá
        painter.fillRect(rect(), QColor(65, 40, 35));
        painter.setPen(Qt::white);
        painter.drawText(20, 40, "NIVEL 1: EL ULTIMO TRAYECTO (Bogota - Side-Scroller) | Tiempo: 180s");
        painter.drawText(20, 70, "[Tecla N: Simular paso a Nivel 2]");
    } else {
        // Escena provisional Nivel 2: Top-Down Teatro Colón
        painter.fillRect(rect(), QColor(35, 35, 65));
        painter.setPen(Qt::white);
        painter.drawText(20, 40, "NIVEL 2: LA FIRMA (Teatro Colon - Top-Down) | Tiempo: 180s");
        painter.drawText(20, 70, "[Tecla N: Simular victoria final]");
    }

    // Dibujo provisional del jugador usando sus coordenadas reales de Entidad
    ClarkKent* jugador = nivelActual->getJugador();
    if (jugador != nullptr) {
        painter.setBrush(Qt::cyan);
        painter.drawRect(jugador->getX(), jugador->getY(),
                         jugador->getAncho(), jugador->getAlto());
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (enPresentacion && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)) {
        enPresentacion = false;
        nivelActual->cargarNivel();
        return;
    }

    if (juegoCompletado && event->key() == Qt::Key_R) {
        delete nivelActual;
        nivelActual = new Nivel(1);
        juegoCompletado = false;
        enPresentacion = true;
        return;
    }

    // Tecla temporal para probar hoy la transición entre Nivel 1 -> Nivel 2 -> Victoria
    if (!enPresentacion && !juegoCompletado && event->key() == Qt::Key_N) {
        if (nivelActual->getNumeroNivel() == 1) {
            delete nivelActual;
            nivelActual = new Nivel(2);
            nivelActual->cargarNivel();
        } else {
            juegoCompletado = true;
        }
    }

    QMainWindow::keyPressEvent(event);
}
