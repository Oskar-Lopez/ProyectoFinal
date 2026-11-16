#ifndef CLARKKENT_H
#define CLARKKENT_H

#include "Personaje.h"
#include <QRect>

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

    // [FIX] Implementación obligatoria de los métodos virtuales puros de Entidad y Personaje
    void actualizar() override;
    void moverse() override;

    // [FIX] Sobrecarga con límites de pantalla usada por MainWindow
    void actualizar(int limiteAnchoPantalla, int ySuelo);

    // [FIX] Método requerido por Nivel.cpp
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
