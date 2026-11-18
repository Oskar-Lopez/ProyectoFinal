#include "AgenteDelOlvido.h"
#include "../physics/ConstantesFisicas.h"
#include <cmath>

AgenteDelOlvido::AgenteDelOlvido(float x, float y, float limiteIzq, float limiteDer)
    : Entidad(x, y, 44.0f, 48.0f),
      velocidadBase(Fisica::VELOCIDAD_AGENTE_PATRULLA),
      radioVision(Fisica::RADIO_VISION_AGENTE),
      contadorEvasiones(0),
      estadoActual(EstadoIA::PATRULLA),
      limiteIzquierdo(limiteIzq),
      limiteDerecho(limiteDer),
      direccion(1.0f) {
}

void AgenteDelOlvido::actualizar() {
    razonar();
    actuar();
}

void AgenteDelOlvido::percibirEntorno(const ClarkKent& jugador) {
    if (jugador.estaInvulnerable() || !jugador.estaVivo()) {
        estadoActual = EstadoIA::PATRULLA;
        return;
    }

    float centroAgenteX = x + (ancho * 0.5f);
    float centroJugadorX = static_cast<float>(jugador.getX()) + (jugador.getHitbox().width() * 0.5f);
    float distanciaX = centroJugadorX - centroAgenteX;

    // Compara altura de los pies para detectar si comparten plataforma/suelo
    float piesAgenteY = y + alto;
    float piesJugadorY = static_cast<float>(jugador.getY() + jugador.getHitbox().height());
    float diferenciaVertical = std::abs(piesJugadorY - piesAgenteY);

    if (diferenciaVertical < 35.0f && std::abs(distanciaX) <= radioVision) {
        if (estadoActual != EstadoIA::PERSECUCION) {
            estadoActual = EstadoIA::PERSECUCION;
        }
        direccion = (distanciaX >= 0.0f) ? 1.0f : -1.0f;
    } else {
        if (estadoActual == EstadoIA::PERSECUCION) {
            aprender();
        }
        estadoActual = EstadoIA::PATRULLA;
    }
}

void AgenteDelOlvido::razonar() {
    // Mantiene al agente dentro de los límites físicos de su plataforma
    if (x <= limiteIzquierdo) {
        x = limiteIzquierdo;
        direccion = 1.0f;
    } else if (x + ancho >= limiteDerecho) {
        x = limiteDerecho - ancho;
        direccion = -1.0f;
    }
}

void AgenteDelOlvido::actuar() {
    float velocidadActual = (estadoActual == EstadoIA::PERSECUCION)
                                ? Fisica::VELOCIDAD_AGENTE_PERSECUCION
                                : velocidadBase;

    // Movimiento Rectilíneo Uniforme (MRU): dx = v * dt
    x += direccion * velocidadActual * Fisica::DT;

    if (x < limiteIzquierdo) {
        x = limiteIzquierdo;
        direccion = 1.0f;
    } else if (x + ancho > limiteDerecho) {
        x = limiteDerecho - ancho;
        direccion = -1.0f;
    }
}

void AgenteDelOlvido::aprender() {
    contadorEvasiones++;
    if (contadorEvasiones >= 3 && velocidadBase < Fisica::VELOCIDAD_AGENTE_PERSECUCION) {
        velocidadBase += 15.0f;
    }
}

QRect AgenteDelOlvido::getHitbox() const {
    return QRect(static_cast<int>(x), static_cast<int>(y),
                 static_cast<int>(ancho), static_cast<int>(alto));
}

bool AgenteDelOlvido::estaEnPersecucion() const {
    return estadoActual == EstadoIA::PERSECUCION;
}

bool AgenteDelOlvido::estaMirandoDerecha() const {
    return direccion >= 0.0f;
}
