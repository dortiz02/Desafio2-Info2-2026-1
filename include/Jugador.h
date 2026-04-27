#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <fstream>
#include <iostream>
#include "EstadisticasJugador.h"

class Jugador {
private:
    std::string         nombre;
    std::string         apellido;
    int                 numeroCamiseta;
    EstadisticasJugador statsHistoricas;

public:
    Jugador();
    Jugador(const std::string& nombre, const std::string& apellido, int numeroCamiseta);
    Jugador(const Jugador& otro);
    Jugador& operator=(const Jugador& otro);
    ~Jugador();

    std::string          getNombre()          const;
    std::string          getApellido()        const;
    int                  getNumeroCamiseta()  const;
    EstadisticasJugador& getStatsHistoricas();

    void setNombre         (const std::string& v);
    void setApellido       (const std::string& v);
    void setNumeroCamiseta (int v);

    void actualizarStats(const EstadisticasJugador& statsPartido);

    void guardar(std::fstream& archivo) const;
    void cargar (std::fstream& archivo);

    bool operator==(const Jugador& otro) const;
    friend std::ostream& operator<<(std::ostream& os, const Jugador& j);
};

#endif
