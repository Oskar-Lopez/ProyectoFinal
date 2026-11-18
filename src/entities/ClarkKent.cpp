#include "ClarkKent.h"
#include "../physics/ConstantesFisicas.h"

ClarkKent::ClarkKent(int xInicial, int yInicial, int ancho, int alto)
    : Personaje(static_cast<float>(xInicial), static_cast<float>(yInicial),
                static_cast<float>(ancho), static_cast<float>(alto),
                Fisica::VIDAS_INICIALES_CLARK, 100),
      posX(static_cast<float>(xInicial)),
      posY(static_cast<float>(yInicial)),
      vx(0.0f),
      vy(0.0f),
      velocidadMovimiento(Fisica::VELOCIDAD_CLARK),
      fuerzaSalto(Fisica::IMPULSO_SALTO_CLARK),
      gravedad(Fisica::GRAVEDAD),
      enSuelo(false),
      mirandoDerecha(true),
      vidasMaximas(Fisica::VIDAS_INICIALES_CLARK),
      tiempoInvulnerable(0.0f) {
}

void ClarkKent::moverse() {
    posX += vx * Fisica::DT;
    posY += vy * Fisica::DT;
    x = posX;
    y = posY;
}

void ClarkKent::actualizar() {
    actualizar(1280, 620, {});
}

void ClarkKent::actualizar(int limiteAnchoPantalla, int ySuelo, const QVector<QRect> &plataformas) {
    if (tiempoInvulnerable > 0.0f) {
        tiempoInvulnerable -= Fisica::DT;
        if (tiempoInvulnerable < 0.0f) {
            tiempoInvulnerable = 0.0f;
        }
    }

    // --- FASE 1: MOVIMIENTO (MRU) Y COLISIÓN HORIZONTAL (EJE X) ---
    posX += vx * Fisica::DT;

    if (posX < 0.0f) {
        posX = 0.0f;
    } else if (posX + ancho > static_cast<float>(limiteAnchoPantalla)) {
        posX = static_cast<float>(limiteAnchoPantalla) - ancho;
    }

    x = posX;

    for (const QRect &plat : plataformas) {
        if (getHitbox().intersects(plat)) {
            if (vx > 0.0f) {
                posX = static_cast<float>(plat.x()) - ancho;
            } else if (vx < 0.0f) {
                posX = static_cast<float>(plat.x() + plat.width());
            }
            x = posX;
        }
    }

    // --- FASE 2: GRAVEDAD (MRUA) Y COLISIÓN VERTICAL (EJE Y) ---
    float desplazamientoY = (vy * Fisica::DT) + (0.5f * gravedad * Fisica::DT * Fisica::DT);
    vy += gravedad * Fisica::DT;

    if (vy > Fisica::VELOCIDAD_CAIDA_MAX) {
        vy = Fisica::VELOCIDAD_CAIDA_MAX;
    }

    posY += desplazamientoY;
    y = posY;
    enSuelo = false;

    for (const QRect &plat : plataformas) {
        if (getHitbox().intersects(plat)) {
            if (vy > 0.0f) {
                posY = static_cast<float>(plat.y()) - alto;
                vy = 0.0f;
                enSuelo = true;
            } else if (vy < 0.0f) {
                posY = static_cast<float>(plat.y() + plat.height());
                vy = 0.0f;
            }
            y = posY;
        }
    }

    // Colisión con el suelo base
    if (posY + alto >= static_cast<float>(ySuelo)) {
        posY = static_cast<float>(ySuelo) - alto;
        y = posY;
        vy = 0.0f;
        enSuelo = true;
    }
}

void ClarkKent::setPosicion(int nuevoX, int nuevoY) {
    posX = static_cast<float>(nuevoX);
    posY = static_cast<float>(nuevoY);
    x = posX;
    y = posY;
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

void ClarkKent::recibirDanio(int cantidad) {
    if (tiempoInvulnerable > 0.0f || vida <= 0) {
        return;
    }

    vida -= cantidad;
    if (vida < 0) {
        vida = 0;
    }

    tiempoInvulnerable = Fisica::TIEMPO_INVULNERABILIDAD;
    vy = Fisica::IMPULSO_SALTO_DANIO;
    enSuelo = false;
}

void ClarkKent::reiniciarPosicion(int xInicial, int yInicial) {
    setPosicion(xInicial, yInicial);
    vx = 0.0f;
    vy = 0.0f;
    enSuelo = false;
    mirandoDerecha = true;
    vida = vidasMaximas;
    tiempoInvulnerable = 0.0f;
}

QRect ClarkKent::getHitbox() const {
    return QRect(static_cast<int>(x), static_cast<int>(y),
                 static_cast<int>(ancho), static_cast<int>(alto));
}

bool ClarkKent::estaEnSuelo() const {
    return enSuelo;
}

bool ClarkKent::estaMirandoDerecha() const {
    return mirandoDerecha;
}

bool ClarkKent::estaInvulnerable() const {
    return tiempoInvulnerable > 0.0f;
}

bool ClarkKent::estaVivo() const {
    return vida > 0;
}

int ClarkKent::getVidas() const {
    return vida;
}

int ClarkKent::getVidasMaximas() const {
    return vidasMaximas;
}

int ClarkKent::getX() const {
    return static_cast<int>(x);
}

int ClarkKent::getY() const {
    return static_cast<int>(y);
}
