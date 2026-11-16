#include "Caja.h"

Caja::Caja(float x, float y, float mu, float masa)
    : Obstaculo(x, y, 48, 48, 0), coeficienteFriccion(mu), masa(masa) {}

void Caja::actualizar() {}

void Caja::interactuar() {
    recibirEmpuje();
}

void Caja::recibirEmpuje() {
    // Fricción paramétrica Nivel 2: d = V0^2 / (2 * mu * g)
}
