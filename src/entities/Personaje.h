#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Entidad.h"

class Personaje : public Entidad {
protected:
    int vida;
    int discrecion;

public:
    Personaje(float x, float y, float ancho, float alto, int vida = 100, int discrecion = 100)
        : Entidad(x, y, ancho, alto), vida(vida), discrecion(discrecion) {}

    virtual void moverse() = 0;
};

#endif // PERSONAJE_H
