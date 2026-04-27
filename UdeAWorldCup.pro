# ================================================================
# UdeAWorldCup.pro — Proyecto Qt Creator
# Informatica II 2026-1
# ================================================================

TEMPLATE = app
CONFIG  += console c++17
CONFIG  -= app_bundle
CONFIG  -= qt

TARGET = UdeAWorldCup

INCLUDEPATH += include

HEADERS += \
    include/Lista.h \
    include/EstadisticasJugador.h \
    include/EstadisticasEquipo.h \
    include/Jugador.h \
    include/ActaJugador.h \
    include/ActaEquipo.h \
    include/Equipo.h \
    include/Partido.h \
    include/Simulador.h \
    include/Grupo.h \
    include/Sorteo.h \
    include/Etapa.h \
    include/MedidorRecursos.h \
    include/Torneo.h \
    include/Menu.h

SOURCES += \
    src/EstadisticasJugador.cpp \
    src/EstadisticasEquipo.cpp \
    src/Jugador.cpp \
    src/ActaJugador.cpp \
    src/ActaEquipo.cpp \
    src/Equipo.cpp \
    src/Partido.cpp \
    src/Simulador.cpp \
    src/Grupo.cpp \
    src/Sorteo.cpp \
    src/Etapa.cpp \
    src/MedidorRecursos.cpp \
    src/Torneo.cpp \
    src/Menu.cpp \
    src/main.cpp

# Copiar el CSV al directorio de build para que el ejecutable lo encuentre
# En Qt Creator el ejecutable corre desde el directorio build,
# asi que el CSV debe estar en data/ relativo al .pro
DISTFILES += \
    data/selecciones_clasificadas_mundial.csv
