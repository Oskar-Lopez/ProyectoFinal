#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QRect>
#include <QVector>
#include "Entidad.h"

class Obstaculo : public Entidad {
protected:
    float velocidadInicial;

public:
    Obstaculo(float x, float y, float ancho, float alto, float velIni = 0.0f)
        : Entidad(x, y, ancho, alto), velocidadInicial(velIni) {}
    virtual ~Obstaculo() = default;

    virtual void interactuar() = 0;

    virtual bool causaDanio() const { return false; }
    virtual bool esSolido() const { return false; }
    virtual void empujar(float dirX, float dirY) {
        (void)dirX;
        (void)dirY;
    }
    virtual void aplicarLimites(const QRect &areaLimite, const QVector<QRect> &muros) {
        (void)areaLimite;
        (void)muros;
    }

    QRect getHitbox() const {
        return QRect(static_cast<int>(x), static_cast<int>(y),
                     static_cast<int>(ancho), static_cast<int>(alto));
    }
};

#endif // OBSTACULO_H
