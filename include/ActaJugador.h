#ifndef ACTAJUGADOR_H
#define ACTAJUGADOR_H

#include <iostream>
#include "Jugador.h"

// Registra lo que hizo un jugador en un partido específico
// No es dueño del Jugador — solo guarda un puntero
class ActaJugador {
private:
    Jugador* jugador;
    int      goles;
    int      tarjetasAmarillas;
    int      tarjetasRojas;
    int      faltas;
    int      minutosJugados;

public:
    ActaJugador();
    ActaJugador(Jugador* j);
    ActaJugador(const ActaJugador& otra);
    ~ActaJugador();

    Jugador* getJugador()           const;
    int      getGoles()             const;
    int      getTarjetasAmarillas() const;
    int      getTarjetasRojas()     const;
    int      getFaltas()            const;
    int      getMinutosJugados()    const;

    void setGoles            (int v);
    void setTarjetasAmarillas(int v);
    void setTarjetasRojas    (int v);
    void setFaltas           (int v);
    void setMinutosJugados   (int v);

    // Convierte el acta en EstadisticasJugador para acumular al historico
    EstadisticasJugador toStats() const;

    friend std::ostream& operator<<(std::ostream& os, const ActaJugador& aj);
};

#endif
