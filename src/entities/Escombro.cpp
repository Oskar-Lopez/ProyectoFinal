#include "Escombro.h"

Escombro::Escombro(float x, float y, float v0x, float v0y, float g)
    : Obstaculo(x, y, 32, 32, v0y), V0x(v0x), V0y(v0y), gravedad(g) {}

void Escombro::actualizar() {
    caerParabolico();
}

void Escombro::interactuar() {}

void Escombro::caerParabolico() {
    // Ecuación parabólica Nivel 1: y(t) = y0 + V0y*t - 0.5*g*t^2
}
