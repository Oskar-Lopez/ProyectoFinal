#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "Entidad.h"

class Obstaculo : public Entidad {
protected:
    float velocidadInicial;

public:
    Obstaculo(float x, float y, float ancho, float alto, float velIni = 0)
        : Entidad(x, y, ancho, alto), velocidadInicial(velIni) {}

    virtual void interactuar() = 0;
};

#endif // OBSTACULO_H
