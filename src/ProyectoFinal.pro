QT += core gui widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    ui/MainWindow.cpp \
    levels/Nivel.cpp \
    entities/ClarkKent.cpp \
    entities/AgenteDelOlvido.cpp \
    entities/Escombro.cpp \
    entities/Caja.cpp

HEADERS += \
    ui/MainWindow.h \
    levels/Nivel.h \
    entities/Entidad.h \
    entities/Personaje.h \
    entities/ClarkKent.h \
    entities/AgenteDelOlvido.h \
    entities/Obstaculo.h \
    entities/Escombro.h \
    entities/Caja.h
