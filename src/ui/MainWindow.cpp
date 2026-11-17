#include "MainWindow.h"
#include <QPainter>
#include <QFont>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      estadoActual(EstadoJuego::MENU),
      timerJuego(new QTimer(this)),
      jugador(120, 500, 46, 76),
      totalPistas(3),
      teclaIzquierda(false),
      teclaDerecha(false) {

    setFixedSize(ANCHO_VENTANA, ALTO_VENTANA);
    setWindowTitle("Proyecto Final - Prototipo Clark Kent");

    iniciarEscenario();

    connect(timerJuego, &QTimer::timeout, this, &MainWindow::actualizarJuego);
    timerJuego->start(16);
}

void MainWindow::iniciarEscenario() {
    jugador.reiniciarPosicion(120, 500);

    plataformas = {
        QRect(260, 500, 220, 24),
        QRect(560, 400, 220, 24),
        QRect(860, 300, 240, 24)
    };

    pistas = {
        QRect(359, 462, 22, 22),
        QRect(659, 362, 22, 22),
        QRect(920, 262, 22, 22)
    };
    totalPistas = static_cast<int>(pistas.size());

    metaNivel = QRect(1030, 228, 44, 72);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        return;
    }

    if (estadoActual == EstadoJuego::MENU || estadoActual == EstadoJuego::VICTORIA) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            iniciarEscenario();
            estadoActual = EstadoJuego::JUGANDO;
            update();
        }
        return;
    }

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
            iniciarEscenario();
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

void MainWindow::actualizarJuego() {
    if (estadoActual == EstadoJuego::JUGANDO) {
        if (teclaIzquierda && !teclaDerecha) {
            jugador.moverIzquierda();
        } else if (teclaDerecha && !teclaIzquierda) {
            jugador.moverDerecha();
        } else {
            jugador.detenerHorizontal();
        }

        jugador.actualizar(ANCHO_VENTANA, NIVEL_SUELO, plataformas);

        QRect hitboxJugador = jugador.getHitbox();
        for (qsizetype i = 0; i < pistas.size(); ) {
            if (hitboxJugador.intersects(pistas[i])) {
                pistas.removeAt(i);
            } else {
                ++i;
            }
        }

        if (pistas.isEmpty() && hitboxJugador.intersects(metaNivel)) {
            teclaIzquierda = false;
            teclaDerecha = false;
            estadoActual = EstadoJuego::VICTORIA;
        }
    }

    update();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.fillRect(rect(), QColor(24, 28, 38));

    if (estadoActual == EstadoJuego::MENU) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 28, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "PROTOTIPO CLARK KENT\n\nPresiona ENTER para comenzar");
        return;
    }

    if (estadoActual == EstadoJuego::VICTORIA) {
        painter.setPen(QColor(80, 220, 130));
        painter.setFont(QFont("Arial", 30, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "¡OBJETIVO COMPLETADO!\n\nPresiona ENTER para volver a jugar");
        return;
    }

    // Suelo base
    painter.fillRect(0, NIVEL_SUELO, ANCHO_VENTANA, ALTO_VENTANA - NIVEL_SUELO, QColor(65, 72, 85));
    painter.fillRect(0, NIVEL_SUELO, ANCHO_VENTANA, 6, QColor(110, 120, 138));

    // Plataformas elevadas
    for (const QRect &plat : plataformas) {
        painter.fillRect(plat, QColor(85, 95, 115));
        painter.fillRect(plat.x(), plat.y(), plat.width(), 5, QColor(145, 160, 185));
    }

    // Meta del nivel (gris bloqueada, verde cuando se recogen todas las pistas)
    QColor colorMeta = pistas.isEmpty() ? QColor(50, 205, 110) : QColor(110, 110, 120);
    painter.fillRect(metaNivel, colorMeta);

    // Pistas recolectables
    painter.setBrush(QColor(245, 200, 60));
    painter.setPen(Qt::NoPen);
    for (const QRect &pista : pistas) {
        painter.drawEllipse(pista);
    }

    // Clark Kent
    QRect hitbox = jugador.getHitbox();
    painter.fillRect(hitbox, QColor(40, 115, 225));

    int anchoVisor = 14;
    int xVisor = jugador.estaMirandoDerecha()
                     ? (hitbox.right() - anchoVisor)
                     : hitbox.left();
    painter.fillRect(xVisor, hitbox.top() + 12, anchoVisor, 10, QColor(220, 50, 50));

    // HUD
    int pistasRecogidas = totalPistas - static_cast<int>(pistas.size());
    painter.setPen(Qt::white);
    painter.setFont(QFont("Monospace", 11));
    painter.drawText(20, 30, "Controles: [A/D o Flechas] Mover | [W/Espacio] Saltar | [R] Reiniciar | [ESC] Menú");
    painter.drawText(20, 55, QString("Pistas: %1/%2 | Salida: %3 | Pos: (%4, %5)")
                                 .arg(pistasRecogidas)
                                 .arg(totalPistas)
                                 .arg(pistas.isEmpty() ? "ABIERTA" : "BLOQUEADA")
                                 .arg(jugador.getX())
                                 .arg(jugador.getY()));
}
