#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow ventana;
    ventana.show();
    return app.exec();
}
