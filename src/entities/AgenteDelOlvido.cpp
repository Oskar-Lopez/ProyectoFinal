#include "AgenteDelOlvido.h"

AgenteDelOlvido::AgenteDelOlvido(float x, float y)
    : Entidad(x, y, 48, 48), velocidadBase(2.5f), radioVision(180.0f),
      contadorEvasiones(0), estadoActual(EstadoIA::PATRULLA) {}

void AgenteDelOlvido::actualizar() {
    razonar();
    actuar();
}

void AgenteDelOlvido::percibirEntorno(const ClarkKent& jugador) {
    (void)jugador;
}

void AgenteDelOlvido::razonar() {}

void AgenteDelOlvido::actuar() {}

void AgenteDelOlvido::aprender() {
    contadorEvasiones++;
    if (contadorEvasiones >= 3) {
        velocidadBase += 0.5f;
    }
}
