#include "Caja.h"
#include "../physics/ConstantesFisicas.h"
#include <cmath>

Caja::Caja(float x, float y, float mu, float masa)
    : Obstaculo(x, y, 52.0f, 52.0f, Fisica::VELOCIDAD_EMPUJE_CAJA),
      coeficienteFriccion(mu),
      masa(masa),
      vx(0.0f),
      vy(0.0f) {
}

void Caja::actualizar() {
    if (std::abs(vx) < 0.1f && std::abs(vy) < 0.1f) {
        vx = 0.0f;
        vy = 0.0f;
        return;
    }

    // Desaceleración por fricción cinética: a_f = mu * g (relacionada con d = V0^2 / (2 * mu * g))
    float desaceleracion = coeficienteFriccion * Fisica::GRAVEDAD_PLANA * Fisica::DT;

    if (vx > 0.0f) {
        vx = std::max(0.0f, vx - desaceleracion);
    } else if (vx < 0.0f) {
        vx = std::min(0.0f, vx + desaceleracion);
    }

    if (vy > 0.0f) {
        vy = std::max(0.0f, vy - desaceleracion);
    } else if (vy < 0.0f) {
        vy = std::min(0.0f, vy + desaceleracion);
    }

    x += vx * Fisica::DT;
    y += vy * Fisica::DT;
}

void Caja::interactuar() {
    recibirEmpuje();
}

void Caja::recibirEmpuje() {
    float factorMasa = (masa > 0.0f) ? (50.0f / masa) : 1.0f;
    velocidadInicial = Fisica::VELOCIDAD_EMPUJE_CAJA * factorMasa;
}

bool Caja::esSolido() const {
    return true;
}

void Caja::empujar(float dirX, float dirY) {
    recibirEmpuje();
    if (dirX != 0.0f) {
        vx = dirX * velocidadInicial;
    }
    if (dirY != 0.0f) {
        vy = dirY * velocidadInicial;
    }
}

void Caja::aplicarLimites(const QRect &areaLimite, const QVector<QRect> &muros) {
    if (x < static_cast<float>(areaLimite.left())) {
        x = static_cast<float>(areaLimite.left());
        vx = 0.0f;
    } else if (x + ancho > static_cast<float>(areaLimite.right())) {
        x = static_cast<float>(areaLimite.right()) - ancho;
        vx = 0.0f;
    }

    if (y < static_cast<float>(areaLimite.top())) {
        y = static_cast<float>(areaLimite.top());
        vy = 0.0f;
    } else if (y + alto > static_cast<float>(areaLimite.bottom())) {
        y = static_cast<float>(areaLimite.bottom()) - alto;
        vy = 0.0f;
    }

    for (const QRect &muro : muros) {
        if (getHitbox().intersects(muro)) {
            if (vx > 0.0f) x = static_cast<float>(muro.left()) - ancho;
            else if (vx < 0.0f) x = static_cast<float>(muro.right());
            if (vy > 0.0f) y = static_cast<float>(muro.top()) - alto;
            else if (vy < 0.0f) y = static_cast<float>(muro.bottom());
            vx = 0.0f;
            vy = 0.0f;
        }
    }
}
