#include "Nivel.h"

Nivel::Nivel(int numNivel)
    : numeroNivel(numNivel), tiempoRestante(180), jugador(nullptr) {
    jugador = new ClarkKent(100, 500);
}

Nivel::~Nivel() {
    delete jugador;
    for (auto* enemigo : enemigos) delete enemigo;
    for (auto* obstaculo : obstaculos) delete obstaculo;
}

void Nivel::cargarNivel() {
    tiempoRestante = 180;
    if (numeroNivel == 1) {
        // Nivel 1: El Último Trayecto (Bogotá Side-scroller)
        jugador->setPosicion(100, 520);
    } else if (numeroNivel == 2) {
        // Nivel 2: La Firma (Teatro Colón Top-Down)
        jugador->setPosicion(640, 600);
    }
}

bool Nivel::verificarVictoria() {
    return false;
}

void Nivel::reiniciarNivel() {
    cargarNivel();
}
