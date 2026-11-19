#include "AgenteDelOlvido.h"
#include "../physics/ConstantesFisicas.h"
#include <cmath>

AgenteDelOlvido::AgenteDelOlvido(float x, float y, float limMin, float limMax, bool vertical)
    : Entidad(x, y, 44.0f, 48.0f),
      velocidadBase(Fisica::VELOCIDAD_AGENTE_PATRULLA),
      radioVision(Fisica::RADIO_VISION_AGENTE),
      contadorEvasiones(0),
      estadoActual(EstadoIA::PATRULLA),
      limiteMin(limMin),
      limiteMax(limMax),
      direccion(1.0f),
      patrullaVertical(vertical) {
    if (patrullaVertical) {
        alto = 44.0f;
    }
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
    float centroAgenteY = y + (alto * 0.5f);
    float centroJugadorX = static_cast<float>(jugador.getX()) + (jugador.getHitbox().width() * 0.5f);
    float centroJugadorY = static_cast<float>(jugador.getY()) + (jugador.getHitbox().height() * 0.5f);

    float distanciaX = centroJugadorX - centroAgenteX;
    float distanciaY = centroJugadorY - centroAgenteY;

    if (jugador.esModoTopDown()) {
        float distanciaEuclidiana = std::sqrt((distanciaX * distanciaX) + (distanciaY * distanciaY));
        if (distanciaEuclidiana <= radioVision) {
            estadoActual = EstadoIA::PERSECUCION;
            if (patrullaVertical) {
                direccion = (distanciaY >= 0.0f) ? 1.0f : -1.0f;
            } else {
                direccion = (distanciaX >= 0.0f) ? 1.0f : -1.0f;
            }
        } else {
            if (estadoActual == EstadoIA::PERSECUCION) aprender();
            estadoActual = EstadoIA::PATRULLA;
        }
        return;
    }

    float piesAgenteY = y + alto;
    float piesJugadorY = static_cast<float>(jugador.getY() + jugador.getHitbox().height());
    float diferenciaVertical = std::abs(piesJugadorY - piesAgenteY);

    if (diferenciaVertical < 35.0f && std::abs(distanciaX) <= radioVision) {
        estadoActual = EstadoIA::PERSECUCION;
        direccion = (distanciaX >= 0.0f) ? 1.0f : -1.0f;
    } else {
        if (estadoActual == EstadoIA::PERSECUCION) aprender();
        estadoActual = EstadoIA::PATRULLA;
    }
}

void AgenteDelOlvido::razonar() {
    if (patrullaVertical) {
        if (y <= limiteMin) {
            y = limiteMin;
            direccion = 1.0f;
        } else if (y + alto >= limiteMax) {
            y = limiteMax - alto;
            direccion = -1.0f;
        }
    } else {
        if (x <= limiteMin) {
            x = limiteMin;
            direccion = 1.0f;
        } else if (x + ancho >= limiteMax) {
            x = limiteMax - ancho;
            direccion = -1.0f;
        }
    }
}

void AgenteDelOlvido::actuar() {
    float velocidadActual = (estadoActual == EstadoIA::PERSECUCION)
                                ? Fisica::VELOCIDAD_AGENTE_PERSECUCION
                                : velocidadBase;

    if (patrullaVertical) {
        y += direccion * velocidadActual * Fisica::DT;
        if (y < limiteMin) {
            y = limiteMin;
            direccion = 1.0f;
        } else if (y + alto > limiteMax) {
            y = limiteMax - alto;
            direccion = -1.0f;
        }
    } else {
        x += direccion * velocidadActual * Fisica::DT;
        if (x < limiteMin) {
            x = limiteMin;
            direccion = 1.0f;
        } else if (x + ancho > limiteMax) {
            x = limiteMax - ancho;
            direccion = -1.0f;
        }
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
