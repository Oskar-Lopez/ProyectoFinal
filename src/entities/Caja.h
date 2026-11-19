#ifndef CAJA_H
#define CAJA_H

#include "Obstaculo.h"

class Caja : public Obstaculo {
private:
    float coeficienteFriccion;
    float masa;
    float vx;
    float vy;

public:
    Caja(float x = 0.0f, float y = 0.0f, float mu = 0.45f, float masa = 50.0f);

    void actualizar() override;
    void interactuar() override;
    void recibirEmpuje();

    bool esSolido() const override;
    void empujar(float dirX, float dirY) override;
    void aplicarLimites(const QRect &areaLimite, const QVector<QRect> &muros) override;
};

#endif // CAJA_H
