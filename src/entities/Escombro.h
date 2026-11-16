#ifndef ESCOMBRO_H
#define ESCOMBRO_H

#include "Obstaculo.h"

class Escombro : public Obstaculo {
private:
    float V0x;
    float V0y;
    float gravedad;

public:
    Escombro(float x = 0, float y = 0, float v0x = 0, float v0y = 0, float g = 9.8f);

    void actualizar() override;
    void interactuar() override;
    void caerParabolico();
};

#endif // ESCOMBRO_H
