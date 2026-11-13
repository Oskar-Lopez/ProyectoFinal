#ifndef CAJA_H
#define CAJA_H

#include "Obstaculo.h"

class Caja : public Obstaculo {
private:
    float coeficienteFriccion;
    float masa;

public:
    Caja(float x = 0, float y = 0, float mu = 0.4f, float masa = 50.0f);

    void actualizar() override;
    void interactuar() override;
    void recibirEmpuje();
};

#endif // CAJA_H
