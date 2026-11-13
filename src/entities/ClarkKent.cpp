#include "ClarkKent.h"

ClarkKent::ClarkKent(float x, float y)
    : Personaje(x, y, 48, 64, 100, 100), esSuperman(false), velocidadBase(5.0f) {}

void ClarkKent::actualizar() {
    moverse();
}

void ClarkKent::moverse() {
    // Lógica de movimiento según Nivel 1 (Side-scroller) o Nivel 2 (Top-Down)
}

void ClarkKent::transformarse() {
    esSuperman = !esSuperman;
}

void ClarkKent::usarSuperFuerza() {}

void ClarkKent::usarRayosX() {}

void ClarkKent::usarSupervelocidad() {}
