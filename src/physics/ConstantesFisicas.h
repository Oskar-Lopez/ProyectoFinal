#ifndef CONSTANTESFISICAS_H
#define CONSTANTESFISICAS_H

namespace Fisica {
    // Paso de tiempo fijo por frame (~60 FPS, sincronizado con QTimer a 16 ms)
    constexpr float DT = 0.016f;

    // Constantes cinemáticas de Clark Kent (píxeles/s y píxeles/s^2)
    constexpr float GRAVEDAD = 3200.0f;
    constexpr float VELOCIDAD_CLARK = 410.0f;
    constexpr float IMPULSO_SALTO_CLARK = -1050.0f;
    constexpr float VELOCIDAD_CAIDA_MAX = 1400.0f;

    // Sistema de vidas y daño
    constexpr int VIDAS_INICIALES_CLARK = 3;
    constexpr float TIEMPO_INVULNERABILIDAD = 1.5f;
    constexpr float IMPULSO_SALTO_DANIO = -420.0f;

    // Constantes cinemáticas de Agente del Olvido (MRU)
    constexpr float VELOCIDAD_AGENTE_PATRULLA = 155.0f;
    constexpr float VELOCIDAD_AGENTE_PERSECUCION = 245.0f;
    constexpr float RADIO_VISION_AGENTE = 210.0f;
}

#endif // CONSTANTESFISICAS_H
