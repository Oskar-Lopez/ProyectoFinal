#include "Nivel.h"
#include "../physics/ConstantesFisicas.h"
#include "../entities/Escombro.h"
#include "../entities/Caja.h"

Nivel::Nivel(int numNivel)
    : numeroNivel(numNivel),
      tiempoRestante(Fisica::TIEMPO_NIVEL_1),
      jugador(new ClarkKent(120, 500)),
      totalPistas(3) {
    cargarNivel(numNivel);
}

Nivel::~Nivel() {
    limpiarEntidades();
    delete jugador;
}

void Nivel::limpiarEntidades() {
    for (auto* enemigo : enemigos) {
        delete enemigo;
    }
    enemigos.clear();

    for (auto* obstaculo : obstaculos) {
        delete obstaculo;
    }
    obstaculos.clear();

    estructuras.clear();
    pistas.clear();
}

void Nivel::cargarNivel(int numNivel) {
    numeroNivel = numNivel;
    limpiarEntidades();

    if (numeroNivel == 1) {
        // NIVEL 1: El Último Trayecto (Bogotá - Side-scroller)
        tiempoRestante = Fisica::TIEMPO_NIVEL_1;
        jugador->setModoTopDown(false);
        jugador->reiniciarPosicion(120, 500);

        estructuras = {
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

        enemigos.push_back(new AgenteDelOlvido(360.0f, static_cast<float>(NIVEL_SUELO - 48), 320.0f, 820.0f, false));
        enemigos.push_back(new AgenteDelOlvido(580.0f, 400.0f - 48.0f, 560.0f, 780.0f, false));
        enemigos.push_back(new AgenteDelOlvido(880.0f, 300.0f - 48.0f, 860.0f, 1020.0f, false));

        // Escombros con tiro parabólico desde cornisas de Bogotá
        obstaculos.push_back(new Escombro(520.0f, 110.0f, -85.0f, -60.0f, Fisica::GRAVEDAD_ESCOMBRO, static_cast<float>(NIVEL_SUELO)));
        obstaculos.push_back(new Escombro(840.0f, 95.0f, -95.0f, -40.0f, Fisica::GRAVEDAD_ESCOMBRO, static_cast<float>(NIVEL_SUELO)));

        metaNivel = QRect(1030, 228, 44, 72);
    } else {
        // NIVEL 2: La Firma (Teatro Colón - Top-Down)
        numeroNivel = 2;
        tiempoRestante = Fisica::TIEMPO_NIVEL_2;
        int vidasActuales = jugador->getVidas();
        jugador->setModoTopDown(true);
        jugador->reiniciarPosicion(95, 360);
        // Conserva al menos 2 vidas al iniciar el Nivel 2
        while (jugador->getVidas() > vidasActuales && jugador->getVidas() > 2) {
            jugador->recibirDanio(1);
        }

        // Muros interiores del Teatro Colón
        estructuras = {
            QRect(260, 92, 34, 240),
            QRect(260, 440, 34, 248),
            QRect(610, 220, 34, 330),
            QRect(920, 92, 34, 250),
            QRect(920, 450, 34, 238)
        };

        // Documentos/sellos confidenciales a recuperar en el teatro
        pistas = {
            QRect(160, 155, 22, 22),
            QRect(520, 585, 22, 22),
            QRect(820, 165, 22, 22)
        };
        totalPistas = static_cast<int>(pistas.size());

        // Guardias patrullando en vista aérea
        enemigos.push_back(new AgenteDelOlvido(420.0f, 140.0f, 120.0f, 620.0f, true));
        enemigos.push_back(new AgenteDelOlvido(750.0f, 560.0f, 120.0f, 620.0f, true));
        enemigos.push_back(new AgenteDelOlvido(670.0f, 135.0f, 660.0f, 870.0f, false));

        // Cajas empujables con rozamiento cinético (mu = 0.45)
        obstaculos.push_back(new Caja(251.0f, 362.0f, Fisica::COEFICIENTE_FRICCION_CAJA, 50.0f));
        obstaculos.push_back(new Caja(911.0f, 368.0f, Fisica::COEFICIENTE_FRICCION_CAJA, 55.0f));

        metaNivel = QRect(1135, 345, 56, 76);
    }
}

void Nivel::cargarNivel() {
    cargarNivel(numeroNivel);
}

void Nivel::actualizar() {
    if (tiempoRestante > 0.0f) {
        tiempoRestante -= Fisica::DT;
        if (tiempoRestante < 0.0f) {
            tiempoRestante = 0.0f;
        }
    }

    jugador->actualizar(ANCHO_ESCENARIO, NIVEL_SUELO, estructuras);

    QRect hitboxJugador = jugador->getHitbox();
    QRect areaSala(48, 92, ANCHO_ESCENARIO - 96, 596);

    // Actualizar obstáculos polimórficos (Escombros en N1, Cajas en N2)
    for (Obstaculo* obs : obstaculos) {
        if (obs->esSolido() && hitboxJugador.intersects(obs->getHitbox())) {
            float dirX = 0.0f;
            float dirY = 0.0f;
            if (jugador->getVx() > 0.0f) dirX = 1.0f;
            else if (jugador->getVx() < 0.0f) dirX = -1.0f;
            if (jugador->getVy() > 0.0f) dirY = 1.0f;
            else if (jugador->getVy() < 0.0f) dirY = -1.0f;

            obs->empujar(dirX, dirY);
        }

        obs->actualizar();
        if (obs->esSolido()) {
            obs->aplicarLimites(areaSala, estructuras);
        }

        if (obs->causaDanio() && hitboxJugador.intersects(obs->getHitbox())) {
            jugador->recibirDanio(1);
            obs->interactuar();
        }
    }

    // Actualizar agentes enemigos
    for (AgenteDelOlvido* enemigo : enemigos) {
        enemigo->percibirEntorno(*jugador);
        enemigo->actualizar();

        if (hitboxJugador.intersects(enemigo->getHitbox())) {
            jugador->recibirDanio(1);
        }
    }

    // Recolección de pistas / documentos
    for (int i = 0; i < static_cast<int>(pistas.size()); ) {
        if (hitboxJugador.intersects(pistas[i])) {
            pistas.removeAt(i);
        } else {
            ++i;
        }
    }
}

bool Nivel::verificarVictoria() const {
    return pistas.isEmpty() && jugador->getHitbox().intersects(metaNivel);
}

bool Nivel::verificarDerrota() const {
    return !jugador->estaVivo() || tiempoRestante <= 0.0f;
}

void Nivel::reiniciarNivel() {
    cargarNivel(numeroNivel);
}

bool Nivel::avanzarSiguienteNivel() {
    if (numeroNivel == 1) {
        cargarNivel(2);
        return true;
    }
    return false;
}

QString Nivel::getTituloNivel() const {
    if (numeroNivel == 1) {
        return "Nivel 1: El Último Trayecto (Bogotá)";
    }
    return "Nivel 2: La Firma (Teatro Colón)";
}
