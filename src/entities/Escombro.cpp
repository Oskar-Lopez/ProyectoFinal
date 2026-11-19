#include "Escombro.h"
#include "../physics/ConstantesFisicas.h"

Escombro::Escombro(float x, float y, float v0x, float v0y, float g, float ySuelo)
    : Obstaculo(x, y, 28.0f, 28.0f, v0y),
      x0(x),
      y0(y),
      V0x(v0x),
      V0y(v0y),
      gravedad(g),
      tiempoVuelo(0.0f),
      tiempoEspera(0.0f),
      limiteSuelo(ySuelo),
      activo(true) {
}

void Escombro::actualizar() {
    caerParabolico();
}

void Escombro::interactuar() {
    activo = false;
    tiempoEspera = 1.2f;
}

void Escombro::caerParabolico() {
    if (!activo) {
        tiempoEspera -= Fisica::DT;
        if (tiempoEspera <= 0.0f) {
            x = x0;
            y = y0;
            tiempoVuelo = 0.0f;
            activo = true;
        }
        return;
    }

    tiempoVuelo += Fisica::DT;

    // Ecuación paramétrica de tiro parabólico:
    // x(t) = x0 + V0x * t
    // y(t) = y0 + V0y * t + 0.5 * g * t^2
    x = x0 + (V0x * tiempoVuelo);
    y = y0 + (V0y * tiempoVuelo) + (0.5f * gravedad * tiempoVuelo * tiempoVuelo);

    if (y + alto >= limiteSuelo) {
        y = limiteSuelo - alto;
        activo = false;
        tiempoEspera = 1.4f;
    }
}

bool Escombro::causaDanio() const {
    return activo;
}
