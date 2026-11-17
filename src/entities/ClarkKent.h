#ifndef CLARKKENT_H
#define CLARKKENT_H

#include "Personaje.h"
#include <QRect>
#include <QVector>

class ClarkKent : public Personaje {
private:
    float vx;
    float vy;
    float velocidadMovimiento;
    float fuerzaSalto;
    float gravedad;
    bool enSuelo;
    bool mirandoDerecha;

public:
    ClarkKent(int xInicial = 120, int yInicial = 500, int ancho = 46, int alto = 76);

    void actualizar() override;
    void moverse() override;
    void actualizar(int limiteAnchoPantalla, int ySuelo, const QVector<QRect> &plataformas = {});

    void setPosicion(int nuevoX, int nuevoY);

    void moverIzquierda();
    void moverDerecha();
    void detenerHorizontal();
    void saltar();
    void reiniciarPosicion(int xInicial = 120, int yInicial = 500);

    QRect getHitbox() const;
    bool estaEnSuelo() const;
    bool estaMirandoDerecha() const;
    int getX() const;
    int getY() const;
};

#endif // CLARKKENT_H
