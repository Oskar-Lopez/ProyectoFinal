#ifndef NIVEL_H
#define NIVEL_H

#include <vector>
#include "../entities/ClarkKent.h"
#include "../entities/AgenteDelOlvido.h"
#include "../entities/Obstaculo.h"

class Nivel {
private:
    int numeroNivel;
    int tiempoRestante;
    ClarkKent* jugador;
    std::vector<AgenteDelOlvido*> enemigos;
    std::vector<Obstaculo*> obstaculos;

public:
    Nivel(int numNivel = 1);
    ~Nivel();

    void cargarNivel();
    bool verificarVictoria();
    void reiniciarNivel();

    int getNumeroNivel() const { return numeroNivel; }
    int getTiempoRestante() const { return tiempoRestante; }
    ClarkKent* getJugador() const { return jugador; }
};

#endif // NIVEL_H
