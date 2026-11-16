#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>       // [NUEVO] Temporizador para el bucle de 60 FPS
#include <QKeyEvent>
#include <QPaintEvent>
#include "../entities/ClarkKent.h" // [NUEVO] Inclusión del protagonista

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    enum class EstadoJuego {
        MENU,
        JUGANDO
    };

    EstadoJuego estadoActual;
    QTimer *timerJuego;         // [NUEVO] Bucle principal de actualización
    ClarkKent jugador;          // [NUEVO] Instancia de Clark Kent

    // [NUEVO] Banderas para movimiento fluido al mantener teclas presionadas
    bool teclaIzquierda;
    bool teclaDerecha;

    static constexpr int ANCHO_VENTANA = 1280;
    static constexpr int ALTO_VENTANA = 720;
    static constexpr int NIVEL_SUELO = 620; // [NUEVO] Altura Y donde apoya el personaje

private slots:
    void actualizarJuego();     // [NUEVO] Slot ejecutado por el QTimer cada 16 ms

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override; // [NUEVO] Detecta cuando se suelta una tecla
    void paintEvent(QPaintEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;
};

#endif // MAINWINDOW_H
