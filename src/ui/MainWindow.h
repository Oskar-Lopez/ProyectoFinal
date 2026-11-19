#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QPaintEvent>
#include "../levels/Nivel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    enum class EstadoJuego {
        MENU,
        JUGANDO,
        TRANSICION_NIVEL,
        VICTORIA,
        GAME_OVER
    };

    EstadoJuego estadoActual;
    QTimer *timerJuego;
    Nivel nivelActual;

    bool teclaIzquierda;
    bool teclaDerecha;
    bool teclaArriba;
    bool teclaAbajo;

    void limpiarTeclas();

private slots:
    void actualizarJuego();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;
};

#endif // MAINWINDOW_H
