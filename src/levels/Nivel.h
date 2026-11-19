#ifndef NIVEL_H
#define NIVEL_H

#include <vector>
#include <QVector>
#include <QRect>
#include <QString>
#include "../entities/ClarkKent.h"
#include "../entities/AgenteDelOlvido.h"
#include "../entities/Obstaculo.h"

class Nivel {
private:
    int numeroNivel;
    float tiempoRestante;
    ClarkKent* jugador;
    std::vector<AgenteDelOlvido*> enemigos;
    std::vector<Obstaculo*> obstaculos;

    QVector<QRect> estructuras; // Plataformas en Nivel 1, muros en Nivel 2
    QVector<QRect> pistas;
    QRect metaNivel;
    int totalPistas;

    void limpiarEntidades();

public:
    static constexpr int ANCHO_ESCENARIO = 1280;
    static constexpr int ALTO_ESCENARIO = 720;
    static constexpr int NIVEL_SUELO = 620;

    explicit Nivel(int numNivel = 1);
    ~Nivel();

    void cargarNivel(int numNivel);
    void cargarNivel();
    void actualizar();
    bool verificarVictoria() const;
    bool verificarDerrota() const;
    void reiniciarNivel();
    bool avanzarSiguienteNivel();

    int getNumeroNivel() const { return numeroNivel; }
    QString getTituloNivel() const;
    int getTiempoRestante() const { return static_cast<int>(tiempoRestante); }
    int getTotalPistas() const { return totalPistas; }
    int getPistasRecogidas() const { return totalPistas - static_cast<int>(pistas.size()); }
    bool salidaAbierta() const { return pistas.isEmpty(); }

    ClarkKent* getJugador() const { return jugador; }
    const std::vector<AgenteDelOlvido*>& getEnemigos() const { return enemigos; }
    const std::vector<Obstaculo*>& getObstaculos() const { return obstaculos; }
    const QVector<QRect>& getEstructuras() const { return estructuras; }
    const QVector<QRect>& getPistas() const { return pistas; }
    QRect getMetaNivel() const { return metaNivel; }
};

#endif // NIVEL_H
