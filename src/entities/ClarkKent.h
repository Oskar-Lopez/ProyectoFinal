#ifndef CLARKKENT_H
#define CLARKKENT_H

#include "Personaje.h"
#include <QRect>
#include <QVector>

class ClarkKent : public Personaje {
private:
    float posX;
    float posY;
    float vx;
    float vy;
    float velocidadMovimiento;
    float fuerzaSalto;
    float gravedad;
    bool enSuelo;
    bool mirandoDerecha;

    int vidasMaximas;
    float tiempoInvulnerable;

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
    void recibirDanio(int cantidad = 1);
    void reiniciarPosicion(int xInicial = 120, int yInicial = 500);

    QRect getHitbox() const;
    bool estaEnSuelo() const;
    bool estaMirandoDerecha() const;
    bool estaInvulnerable() const;
    bool estaVivo() const;
    int getVidas() const;
    int getVidasMaximas() const;
    int getX() const;
    int getY() const;
};

#endif // CLARKKENT_H
