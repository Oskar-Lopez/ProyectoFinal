#ifndef AGENTEDELOLVIDO_H
#define AGENTEDELOLVIDO_H

#include <QRect>
#include "Entidad.h"
#include "ClarkKent.h"

enum class EstadoIA { PATRULLA, PERSECUCION, BUSQUEDA };

class AgenteDelOlvido : public Entidad {
private:
    float velocidadBase;
    float radioVision;
    int contadorEvasiones;
    EstadoIA estadoActual;

    float limiteMin;
    float limiteMax;
    float direccion;
    bool patrullaVertical;

public:
    AgenteDelOlvido(float x = 400.0f, float y = 300.0f,
                    float limMin = 260.0f, float limMax = 540.0f,
                    bool vertical = false);

    void actualizar() override;
    void percibirEntorno(const ClarkKent& jugador);
    void razonar();
    void actuar();
    void aprender();

    QRect getHitbox() const;
    bool estaEnPersecucion() const;
    bool estaMirandoDerecha() const;
};

#endif // AGENTEDELOLVIDO_H
