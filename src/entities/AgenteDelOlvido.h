#ifndef AGENTEDELOLVIDO_H
#define AGENTEDELOLVIDO_H

#include "Entidad.h"
#include "ClarkKent.h"

enum class EstadoIA { PATRULLA, PERSECUCION, BUSQUEDA };

class AgenteDelOlvido : public Entidad {
private:
    float velocidadBase;
    float radioVision;
    int contadorEvasiones;
    EstadoIA estadoActual;

public:
    AgenteDelOlvido(float x = 400, float y = 300);

    void actualizar() override;
    void percibirEntorno(const ClarkKent& jugador);
    void razonar();
    void actuar();
    void aprender();
};

#endif // AGENTEDELOLVIDO_H
