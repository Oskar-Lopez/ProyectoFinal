#include "MainWindow.h"
#include <QPainter>
#include <QFont>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      estadoActual(EstadoJuego::MENU),
      timerJuego(new QTimer(this)),
      nivelActual(1),
      teclaIzquierda(false),
      teclaDerecha(false),
      teclaArriba(false),
      teclaAbajo(false) {

    setFixedSize(Nivel::ANCHO_ESCENARIO, Nivel::ALTO_ESCENARIO);
    setWindowTitle("Proyecto Final - Clark Kent: El Último Trayecto");

    connect(timerJuego, &QTimer::timeout, this, &MainWindow::actualizarJuego);
    timerJuego->start(16);
}

void MainWindow::limpiarTeclas() {
    teclaIzquierda = false;
    teclaDerecha = false;
    teclaArriba = false;
    teclaAbajo = false;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        return;
    }

    if (estadoActual == EstadoJuego::MENU ||
        estadoActual == EstadoJuego::VICTORIA ||
        estadoActual == EstadoJuego::GAME_OVER) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            limpiarTeclas();
            nivelActual.cargarNivel(1);
            estadoActual = EstadoJuego::JUGANDO;
            update();
        }
        return;
    }

    if (estadoActual == EstadoJuego::TRANSICION_NIVEL) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            limpiarTeclas();
            nivelActual.avanzarSiguienteNivel();
            estadoActual = EstadoJuego::JUGANDO;
            update();
        }
        return;
    }

    ClarkKent *jugador = nivelActual.getJugador();

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
            teclaArriba = true;
            if (!jugador->esModoTopDown()) {
                jugador->saltar();
            }
            break;
        case Qt::Key_Space:
            if (!jugador->esModoTopDown()) {
                jugador->saltar();
            }
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            teclaAbajo = true;
            break;
        case Qt::Key_R:
            limpiarTeclas();
            nivelActual.reiniciarNivel();
            break;
        case Qt::Key_Escape:
            limpiarTeclas();
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
        case Qt::Key_W:
        case Qt::Key_Up:
            teclaArriba = false;
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            teclaAbajo = false;
            break;
        default:
            QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::actualizarJuego() {
    if (estadoActual == EstadoJuego::JUGANDO) {
        ClarkKent *jugador = nivelActual.getJugador();

        if (teclaIzquierda && !teclaDerecha) {
            jugador->moverIzquierda();
        } else if (teclaDerecha && !teclaIzquierda) {
            jugador->moverDerecha();
        } else {
            jugador->detenerHorizontal();
        }

        if (jugador->esModoTopDown()) {
            if (teclaArriba && !teclaAbajo) {
                jugador->moverArriba();
            } else if (teclaAbajo && !teclaArriba) {
                jugador->moverAbajo();
            } else {
                jugador->detenerVertical();
            }
        }

        nivelActual.actualizar();

        if (nivelActual.verificarDerrota()) {
            limpiarTeclas();
            estadoActual = EstadoJuego::GAME_OVER;
        } else if (nivelActual.verificarVictoria()) {
            limpiarTeclas();
            if (nivelActual.getNumeroNivel() == 1) {
                estadoActual = EstadoJuego::TRANSICION_NIVEL;
            } else {
                estadoActual = EstadoJuego::VICTORIA;
            }
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

    if (estadoActual == EstadoJuego::TRANSICION_NIVEL) {
        painter.setPen(QColor(245, 200, 60));
        painter.setFont(QFont("Arial", 26, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "NIVEL 1 COMPLETADO\n\nSiguiente: Nivel 2 - La Firma (Teatro Colón)\nUsa W/A/S/D para infiltrarte y empujar cajas\n\nPresiona ENTER para continuar");
        return;
    }

    if (estadoActual == EstadoJuego::VICTORIA) {
        painter.setPen(QColor(80, 220, 130));
        painter.setFont(QFont("Arial", 30, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "¡MISIÓN CUMPLIDA EN EL TEATRO COLÓN!\n\nPresiona ENTER para volver a jugar");
        return;
    }

    if (estadoActual == EstadoJuego::GAME_OVER) {
        painter.setPen(QColor(235, 75, 75));
        painter.setFont(QFont("Arial", 30, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "¡MISIÓN FALLIDA!\n\nPresiona ENTER para reintentar desde el Nivel 1");
        return;
    }

    const int numNivel = nivelActual.getNumeroNivel();

    if (numNivel == 1) {
        // Escenario Nivel 1: Side-scroller Bogotá
        painter.fillRect(0, Nivel::NIVEL_SUELO, Nivel::ANCHO_ESCENARIO,
                         Nivel::ALTO_ESCENARIO - Nivel::NIVEL_SUELO, QColor(65, 72, 85));
        painter.fillRect(0, Nivel::NIVEL_SUELO, Nivel::ANCHO_ESCENARIO, 6, QColor(110, 120, 138));

        for (const QRect &plat : nivelActual.getEstructuras()) {
            painter.fillRect(plat, QColor(85, 95, 115));
            painter.fillRect(plat.x(), plat.y(), plat.width(), 5, QColor(145, 160, 185));
        }
    } else {
        // Escenario Nivel 2: Top-Down Teatro Colón
        painter.fillRect(48, 92, Nivel::ANCHO_ESCENARIO - 96, 596, QColor(58, 24, 32));
        painter.setPen(QPen(QColor(180, 145, 75), 4));
        painter.drawRect(48, 92, Nivel::ANCHO_ESCENARIO - 96, 596);
        painter.setPen(Qt::NoPen);

        for (const QRect &muro : nivelActual.getEstructuras()) {
            painter.fillRect(muro, QColor(125, 100, 62));
        }
    }

    // Meta del nivel
    QColor colorMeta = nivelActual.salidaAbierta() ? QColor(50, 205, 110) : QColor(110, 110, 120);
    painter.fillRect(nivelActual.getMetaNivel(), colorMeta);

    // Pistas / Documentos recolectables
    painter.setBrush(QColor(245, 200, 60));
    painter.setPen(Qt::NoPen);
    for (const QRect &pista : nivelActual.getPistas()) {
        painter.drawEllipse(pista);
    }

    // Obstáculos polimórficos (Escombros en Nivel 1, Cajas en Nivel 2)
    for (const Obstaculo *obs : nivelActual.getObstaculos()) {
        if (obs->causaDanio()) {
            painter.setBrush(QColor(235, 130, 45));
            painter.drawEllipse(obs->getHitbox());
        } else if (obs->esSolido()) {
            painter.fillRect(obs->getHitbox(), QColor(165, 115, 65));
            painter.setPen(QPen(QColor(95, 60, 30), 2));
            painter.drawRect(obs->getHitbox());
            painter.setPen(Qt::NoPen);
        }
    }

    // Agentes del Olvido
    for (const AgenteDelOlvido *agente : nivelActual.getEnemigos()) {
        QRect hitboxAgente = agente->getHitbox();
        QColor colorCuerpo = agente->estaEnPersecucion()
                                 ? QColor(215, 55, 65)
                                 : QColor(135, 65, 175);
        painter.fillRect(hitboxAgente, colorCuerpo);

        int anchoVisorAgente = 12;
        int xVisorAgente = agente->estaMirandoDerecha()
                               ? (hitboxAgente.right() - anchoVisorAgente)
                               : hitboxAgente.left();
        painter.fillRect(xVisorAgente, hitboxAgente.top() + 10,
                         anchoVisorAgente, 8, QColor(255, 220, 90));
    }

    // Clark Kent
    ClarkKent *jugador = nivelActual.getJugador();
    QRect hitbox = jugador->getHitbox();
    QColor colorClark = jugador->estaInvulnerable()
                            ? QColor(120, 180, 255, 145)
                            : QColor(40, 115, 225);
    painter.fillRect(hitbox, colorClark);

    int anchoVisor = 14;
    int xVisor = jugador->estaMirandoDerecha()
                     ? (hitbox.right() - anchoVisor)
                     : hitbox.left();
    painter.fillRect(xVisor, hitbox.top() + 10, anchoVisor, 9, QColor(220, 50, 50));

    // HUD Superior
    painter.setPen(Qt::white);
    painter.setFont(QFont("Monospace", 11));
    QString textoControles = (numNivel == 1)
                                 ? "Controles: [A/D] Mover | [W/Espacio] Saltar | [R] Reiniciar | [ESC] Menú"
                                 : "Controles: [W/A/S/D] Mover e impulsar cajas | [R] Reiniciar | [ESC] Menú";
    painter.drawText(20, 28, QString("%1  |  %2").arg(nivelActual.getTituloNivel(), textoControles));
    painter.drawText(20, 54, QString("Vidas: %1/%2 | Pistas: %3/%4 | Tiempo: %5s | Salida: %6")
                                 .arg(jugador->getVidas())
                                 .arg(jugador->getVidasMaximas())
                                 .arg(nivelActual.getPistasRecogidas())
                                 .arg(nivelActual.getTotalPistas())
                                 .arg(nivelActual.getTiempoRestante())
                                 .arg(nivelActual.salidaAbierta() ? "ABIERTA" : "BLOQUEADA"));
}
