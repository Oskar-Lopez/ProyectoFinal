#include "MainWindow.h"
#include <QPainter>
#include <QFont>
#include <QString> // [NUEVO] Para mostrar coordenadas en pantalla

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      estadoActual(EstadoJuego::MENU),
      timerJuego(new QTimer(this)),          // [NUEVO] Inicializa el timer
      jugador(120, 500, 46, 76),             // [NUEVO] Instancia a Clark Kent
      teclaIzquierda(false),                 // [NUEVO]
      teclaDerecha(false) {                  // [NUEVO]

    setFixedSize(ANCHO_VENTANA, ALTO_VENTANA);
    setWindowTitle("Proyecto Final - Prototipo Clark Kent");

    // [NUEVO] Conecta el bucle principal (~60 FPS)
    connect(timerJuego, &QTimer::timeout, this, &MainWindow::actualizarJuego);
    timerJuego->start(16);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // [NUEVO] Ignora repeticiones automáticas del sistema operativo al mantener pulsado
    if (event->isAutoRepeat()) {
        return;
    }

    if (estadoActual == EstadoJuego::MENU) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            estadoActual = EstadoJuego::JUGANDO;
            update();
        }
        return;
    }

    // [MODIFICADO] Controles de movimiento, salto y reinicio en estado JUGANDO
    switch (event->key()) {
        case Qt::Key_A:
        case Qt::Key_Left:
            teclaIzquierda = true;
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            teclaDerecha = true;
            break;
        case Qt::Key_W:
        case Qt::Key_Up:
        case Qt::Key_Space:
            jugador.saltar();
            break;
        case Qt::Key_R:
            jugador.reiniciarPosicion(120, 500);
            break;
        case Qt::Key_Escape:
            teclaIzquierda = false;
            teclaDerecha = false;
            estadoActual = EstadoJuego::MENU;
            break;
        default:
            QMainWindow::keyPressEvent(event);
    }
}

// [NUEVO] Libera las banderas de movimiento cuando el usuario suelta A/D o Flechas
void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        return;
    }

    switch (event->key()) {
        case Qt::Key_A:
        case Qt::Key_Left:
            teclaIzquierda = false;
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            teclaDerecha = false;
            break;
        default:
            QMainWindow::keyReleaseEvent(event);
    }
}

// [NUEVO] Actualiza la velocidad y posición de Clark Kent en cada frame
void MainWindow::actualizarJuego() {
    if (estadoActual == EstadoJuego::JUGANDO) {
        if (teclaIzquierda && !teclaDerecha) {
            jugador.moverIzquierda();
        } else if (teclaDerecha && !teclaIzquierda) {
            jugador.moverDerecha();
        } else {
            jugador.detenerHorizontal();
        }

        jugador.actualizar(ANCHO_VENTANA, NIVEL_SUELO);
    }

    update();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // Fondo general
    painter.fillRect(rect(), QColor(24, 28, 38));

    if (estadoActual == EstadoJuego::MENU) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 28, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "PROTOTIPO DE MOVIMIENTO\n\nPresiona ENTER para comenzar");
        return;
    }

    // [NUEVO] Renderizado del suelo de prueba
    painter.fillRect(0, NIVEL_SUELO, ANCHO_VENTANA, ALTO_VENTANA - NIVEL_SUELO, QColor(65, 72, 85));
    painter.fillRect(0, NIVEL_SUELO, ANCHO_VENTANA, 6, QColor(110, 120, 138));

    // [NUEVO] Renderizado de Clark Kent como rectángulo azul
    QRect hitbox = jugador.getHitbox();
    painter.fillRect(hitbox, QColor(40, 115, 225));

    // [NUEVO] Indicador rojo para visualizar hacia dónde mira Clark Kent
    int anchoVisor = 14;
    int xVisor = jugador.estaMirandoDerecha()
                     ? (hitbox.right() - anchoVisor)
                     : hitbox.left();
    painter.fillRect(xVisor, hitbox.top() + 12, anchoVisor, 10, QColor(220, 50, 50));

    // [NUEVO] HUD de controles y coordenadas en tiempo real
    painter.setPen(Qt::white);
    painter.setFont(QFont("Monospace", 11));
    painter.drawText(20, 30, "Controles: [A/D o Flechas] Mover | [W/Espacio] Saltar | [R] Reiniciar | [ESC] Menú");
    painter.drawText(20, 55, QString("ClarkKent -> X: %1 | Y: %2 | En suelo: %3")
                                 .arg(jugador.getX())
                                 .arg(jugador.getY())
                                 .arg(jugador.estaEnSuelo() ? "Sí" : "No"));
}
