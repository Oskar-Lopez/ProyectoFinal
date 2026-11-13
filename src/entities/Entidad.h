#ifndef ENTIDAD_H
#define ENTIDAD_H

class Entidad {
protected:
    float x;
    float y;
    float ancho;
    float alto;

public:
    Entidad(float x = 0, float y = 0, float ancho = 32, float alto = 32)
        : x(x), y(y), ancho(ancho), alto(alto) {}
    virtual ~Entidad() = default;

    virtual void actualizar() = 0;

    float getX() const { return x; }
    float getY() const { return y; }
    float getAncho() const { return ancho; }
    float getAlto() const { return alto; }
};

#endif // ENTIDAD_H
