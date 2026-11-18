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
    setWindowTitle("Proyecto Final - Clark Kent: El Último Trayecto");

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

    // Despliegue de Agentes del Olvido patrullando suelo y plataformas elevadas
    agentes = {
        AgenteDelOlvido(360.0f, static_cast<float>(NIVEL_SUELO - 48), 320.0f, 820.0f),
        AgenteDelOlvido(580.0f, 400.0f - 48.0f, 560.0f, 780.0f),
        AgenteDelOlvido(880.0f, 300.0f - 48.0f, 860.0f, 1020.0f)
    };

    metaNivel = QRect(1030, 228, 44, 72);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        return;
    }

    if (estadoActual == EstadoJuego::MENU ||
        estadoActual == EstadoJuego::VICTORIA ||
        estadoActual == EstadoJuego::GAME_OVER) {
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

        // Actualizar agentes enemigos y evaluar daño por contacto
        for (AgenteDelOlvido &agente : agentes) {
            agente.percibirEntorno(jugador);
            agente.actualizar();

            if (hitboxJugador.intersects(agente.getHitbox())) {
                jugador.recibirDanio(1);
            }
        }

        if (!jugador.estaVivo()) {
            teclaIzquierda = false;
            teclaDerecha = false;
            estadoActual = EstadoJuego::GAME_OVER;
            update();
            return;
        }

        // Recolección de pistas
        for (int i = 0; i < static_cast<int>(pistas.size()); ) {
            if (hitboxJugador.intersects(pistas[i])) {
                pistas.removeAt(i);
            } else {
                ++i;
            }
        }

        // Condición de victoria
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
                         "CLARK KENT: EL ÚLTIMO TRAYECTO\n\nPresiona ENTER para comenzar");
        return;
    }

    if (estadoActual == EstadoJuego::VICTORIA) {
        painter.setPen(QColor(80, 220, 130));
        painter.setFont(QFont("Arial", 30, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "¡OBJETIVO COMPLETADO!\n\nPresiona ENTER para volver a jugar");
        return;
    }

    if (estadoActual == EstadoJuego::GAME_OVER) {
        painter.setPen(QColor(235, 75, 75));
        painter.setFont(QFont("Arial", 30, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "¡TE HAN DESCUBIERTO!\n\nPresiona ENTER para reintentar");
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

    // Agentes del Olvido
    for (const AgenteDelOlvido &agente : agentes) {
        QRect hitboxAgente = agente.getHitbox();
        QColor colorCuerpo = agente.estaEnPersecucion()
                                 ? QColor(215, 55, 65)
                                 : QColor(135, 65, 175);
        painter.fillRect(hitboxAgente, colorCuerpo);

        int anchoVisorAgente = 12;
        int xVisorAgente = agente.estaMirandoDerecha()
                               ? (hitboxAgente.right() - anchoVisorAgente)
                               : hitboxAgente.left();
        painter.fillRect(xVisorAgente, hitboxAgente.top() + 10,
                         anchoVisorAgente, 8, QColor(255, 220, 90));
    }

    // Clark Kent (parpadeo visual durante invulnerabilidad)
    QRect hitbox = jugador.getHitbox();
    QColor colorClark = jugador.estaInvulnerable()
                            ? QColor(120, 180, 255, 145)
                            : QColor(40, 115, 225);
    painter.fillRect(hitbox, colorClark);

    int anchoVisor = 14;
    int xVisor = jugador.estaMirandoDerecha()
                     ? (hitbox.right() - anchoVisor)
                     : hitbox.left();
    painter.fillRect(xVisor, hitbox.top() + 12, anchoVisor, 10, QColor(220, 50, 50));

    // HUD Superior
    int pistasRecogidas = totalPistas - static_cast<int>(pistas.size());
    painter.setPen(Qt::white);
    painter.setFont(QFont("Monospace", 11));
    painter.drawText(20, 30, "Controles: [A/D o Flechas] Mover | [W/Espacio] Saltar | [R] Reiniciar | [ESC] Menú");
    painter.drawText(20, 55, QString("Vidas: %1/%2 | Pistas: %3/%4 | Salida: %5 | Pos: (%6, %7)")
                                 .arg(jugador.getVidas())
                                 .arg(jugador.getVidasMaximas())
                                 .arg(pistasRecogidas)
                                 .arg(totalPistas)
                                 .arg(pistas.isEmpty() ? "ABIERTA" : "BLOQUEADA")
                                 .arg(jugador.getX())
                                 .arg(jugador.getY()));
}
