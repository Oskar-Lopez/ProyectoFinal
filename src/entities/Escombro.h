#ifndef ESCOMBRO_H
#define ESCOMBRO_H

#include "Obstaculo.h"

class Escombro : public Obstaculo {
private:
    float x0;
    float y0;
    float V0x;
    float V0y;
    float gravedad;
    float tiempoVuelo;
    float tiempoEspera;
    float limiteSuelo;
    bool activo;

public:
    Escombro(float x = 0.0f, float y = 0.0f, float v0x = -90.0f, float v0y = -120.0f,
             float g = 680.0f, float ySuelo = 620.0f);

    void actualizar() override;
    void interactuar() override;
    void caerParabolico();
    bool causaDanio() const override;
};

#endif // ESCOMBRO_H
