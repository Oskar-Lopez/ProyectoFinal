#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include "../levels/Nivel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QTimer* timerJuego;
    Nivel* nivelActual;
    bool enPresentacion;
    bool juegoCompletado;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void actualizarFrame();
};

#endif // MAINWINDOW_H
