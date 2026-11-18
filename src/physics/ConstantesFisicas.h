#ifndef CONSTANTESFISICAS_H
#define CONSTANTESFISICAS_H

namespace Fisica {
    // Paso de tiempo fijo por frame (~60 FPS, sincronizado con QTimer a 16 ms)
    constexpr float DT = 0.016f;

    // Constantes cinemáticas (unidades en píxeles/segundo y píxeles/segundo^2)
    constexpr float GRAVEDAD = 3200.0f;
    constexpr float VELOCIDAD_CLARK = 410.0f;
    constexpr float IMPULSO_SALTO_CLARK = -1050.0f;
    constexpr float VELOCIDAD_CAIDA_MAX = 1400.0f;
}

#endif // CONSTANTESFISICAS_H
