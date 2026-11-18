#include "ClarkKent.h"
#include "../physics/ConstantesFisicas.h"

ClarkKent::ClarkKent(int xInicial, int yInicial, int ancho, int alto)
    : Personaje(xInicial, yInicial, ancho, alto),
      posX(static_cast<float>(xInicial)),
      posY(static_cast<float>(yInicial)),
      vx(0.0f),
      vy(0.0f),
      velocidadMovimiento(Fisica::VELOCIDAD_CLARK),
      fuerzaSalto(Fisica::IMPULSO_SALTO_CLARK),
      gravedad(Fisica::GRAVEDAD),
      enSuelo(false),
      mirandoDerecha(true) {
}

void ClarkKent::moverse() {
    posX += vx * Fisica::DT;
    posY += vy * Fisica::DT;
    x = static_cast<int>(posX);
    y = static_cast<int>(posY);
}

void ClarkKent::actualizar() {
    actualizar(1280, 620, {});
}

void ClarkKent::actualizar(int limiteAnchoPantalla, int ySuelo, const QVector<QRect> &plataformas) {
    // --- FASE 1: MOVIMIENTO (MRU) Y COLISIÓN HORIZONTAL (EJE X) ---
    posX += vx * Fisica::DT;

    if (posX < 0.0f) {
        posX = 0.0f;
    } else if (posX + ancho > static_cast<float>(limiteAnchoPantalla)) {
        posX = static_cast<float>(limiteAnchoPantalla - ancho);
    }

    x = static_cast<int>(posX);

    for (const QRect &plat : plataformas) {
        if (getHitbox().intersects(plat)) {
            if (vx > 0.0f) {
                x = plat.x() - ancho;
            } else if (vx < 0.0f) {
                x = plat.x() + plat.width();
            }
            posX = static_cast<float>(x);
        }
    }

    // --- FASE 2: GRAVEDAD (MRUA) Y COLISIÓN VERTICAL (EJE Y) ---
    float desplazamientoY = (vy * Fisica::DT) + (0.5f * gravedad * Fisica::DT * Fisica::DT);
    vy += gravedad * Fisica::DT;

    if (vy > Fisica::VELOCIDAD_CAIDA_MAX) {
        vy = Fisica::VELOCIDAD_CAIDA_MAX;
    }

    posY += desplazamientoY;
    y = static_cast<int>(posY);
    enSuelo = false;

    for (const QRect &plat : plataformas) {
        if (getHitbox().intersects(plat)) {
            if (vy > 0.0f) {
                y = plat.y() - alto;
                vy = 0.0f;
                enSuelo = true;
            } else if (vy < 0.0f) {
                y = plat.y() + plat.height();
                vy = 0.0f;
            }
            posY = static_cast<float>(y);
        }
    }

    // Colisión con el suelo base
    if (posY + alto >= static_cast<float>(ySuelo)) {
        y = ySuelo - alto;
        posY = static_cast<float>(y);
        vy = 0.0f;
        enSuelo = true;
    }
}

void ClarkKent::setPosicion(int nuevoX, int nuevoY) {
    x = nuevoX;
    y = nuevoY;
    posX = static_cast<float>(nuevoX);
    posY = static_cast<float>(nuevoY);
}

void ClarkKent::moverIzquierda() {
    vx = -velocidadMovimiento;
    mirandoDerecha = false;
}

void ClarkKent::moverDerecha() {
    vx = velocidadMovimiento;
    mirandoDerecha = true;
}

void ClarkKent::detenerHorizontal() {
    vx = 0.0f;
}

void ClarkKent::saltar() {
    if (enSuelo) {
        vy = fuerzaSalto;
        enSuelo = false;
    }
}

void ClarkKent::reiniciarPosicion(int xInicial, int yInicial) {
    setPosicion(xInicial, yInicial);
    vx = 0.0f;
    vy = 0.0f;
    enSuelo = false;
    mirandoDerecha = true;
}

QRect ClarkKent::getHitbox() const {
    return QRect(x, y, ancho, alto);
}

bool ClarkKent::estaEnSuelo() const {
    return enSuelo;
}

bool ClarkKent::estaMirandoDerecha() const {
    return mirandoDerecha;
}

int ClarkKent::getX() const {
    return x;
}

int ClarkKent::getY() const {
    return y;
}
