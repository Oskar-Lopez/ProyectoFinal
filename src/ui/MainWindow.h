#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QVector>
#include <QRect>
#include "../entities/ClarkKent.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    enum class EstadoJuego {
        MENU,
        JUGANDO,
        VICTORIA
    };

    EstadoJuego estadoActual;
    QTimer *timerJuego;
    ClarkKent jugador;

    QVector<QRect> plataformas;
    QVector<QRect> pistas;
    QRect metaNivel;
    int totalPistas;

    bool teclaIzquierda;
    bool teclaDerecha;

    static constexpr int ANCHO_VENTANA = 1280;
    static constexpr int ALTO_VENTANA = 720;
    static constexpr int NIVEL_SUELO = 620;

    void iniciarEscenario();

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
