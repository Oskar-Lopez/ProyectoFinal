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

// [FIX] Implementa el desplazamiento según las velocidades actuales (heredado de Personaje)
void ClarkKent::moverse() {
    x += static_cast<int>(vx);
    y += static_cast<int>(vy);
}

// [FIX] Implementación sin parámetros (heredado de Entidad) con los límites por defecto
void ClarkKent::actualizar() {
    actualizar(1280, 620);
}

// Actualización física completa con límites dinámicos
void ClarkKent::actualizar(int limiteAnchoPantalla, int ySuelo) {
    // 1. Aplicar gravedad
    vy += gravedad;

    // 2. Aplicar desplazamiento
    moverse();

    // 3. Limitar bordes izquierdo y derecho
    if (x < 0) {
        x = 0;
    } else if (x + ancho > limiteAnchoPantalla) {
        x = limiteAnchoPantalla - ancho;
    }

    // 4. Colisión con el suelo
    if (y + alto >= ySuelo) {
        y = ySuelo - alto;
        vy = 0.0f;
        enSuelo = true;
    }
}

// [FIX] Permite a Nivel.cpp ubicar a Clark Kent en el mapa
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
