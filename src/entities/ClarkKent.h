#ifndef CLARKKENT_H
#define CLARKKENT_H

#include "Personaje.h"

class ClarkKent : public Personaje {
private:
    bool esSuperman;
    float velocidadBase;

public:
    ClarkKent(float x = 100, float y = 500);

    void actualizar() override;
    void moverse() override;

    void transformarse();
    void usarSuperFuerza();
    void usarRayosX();
    void usarSupervelocidad();

    bool getEsSuperman() const { return esSuperman; }
    void setPosicion(float nx, float ny) { x = nx; y = ny; }
};

#endif // CLARKKENT_H
