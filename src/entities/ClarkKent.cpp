#include "ClarkKent.h"

ClarkKent::ClarkKent(int xInicial, int yInicial, int ancho, int alto)
    : Personaje(xInicial, yInicial, ancho, alto),
      vx(0.0f),
      vy(0.0f),
      velocidadMovimiento(6.5f),
      fuerzaSalto(-17.0f),
      gravedad(0.85f),
      enSuelo(false),
      mirandoDerecha(true) {
}

void ClarkKent::moverse() {
    x += static_cast<int>(vx);
    y += static_cast<int>(vy);
}

void ClarkKent::actualizar() {
    actualizar(1280, 620, {});
}

void ClarkKent::actualizar(int limiteAnchoPantalla, int ySuelo, const QVector<QRect> &plataformas) {
    // --- FASE 1: MOVIMIENTO Y COLISIÓN HORIZONTAL (EJE X) ---
    x += static_cast<int>(vx);

    if (x < 0) {
        x = 0;
    } else if (x + ancho > limiteAnchoPantalla) {
        x = limiteAnchoPantalla - ancho;
    }

    for (const QRect &plat : plataformas) {
        if (getHitbox().intersects(plat)) {
            if (vx > 0.0f) {
                x = plat.x() - ancho;
            } else if (vx < 0.0f) {
                x = plat.x() + plat.width();
            }
        }
    }

    // --- FASE 2: GRAVEDAD Y COLISIÓN VERTICAL (EJE Y) ---
    vy += gravedad;
    y += static_cast<int>(vy);
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
        }
    }

    // Colisión con el suelo base
    if (y + alto >= ySuelo) {
        y = ySuelo - alto;
        vy = 0.0f;
        enSuelo = true;
    }
}

void ClarkKent::setPosicion(int nuevoX, int nuevoY) {
    x = nuevoX;
    y = nuevoY;
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
