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
    // Constructores
    Jugador();
    Jugador(const std::string& nombre, const std::string& apellido,
            int numeroCamiseta);
    Jugador(const Jugador& otro);  // copia — requerido por enunciado

    // Destructor
    ~Jugador();

    // Getters
    std::string          getNombre()          const;
    std::string          getApellido()        const;
    int                  getNumeroCamiseta()  const;
    EstadisticasJugador& getStatsHistoricas();

    // Setters
    void setNombre         (const std::string& v);
    void setApellido       (const std::string& v);
    void setNumeroCamiseta (int v);

    // Acumula las stats de un partido sobre el histórico
    void actualizarStats(const EstadisticasJugador& statsPartido);

    // Persistencia — solo al inicio y al final del programa
    void guardar(std::fstream& archivo) const;
    void cargar (std::fstream& archivo);

    // Sobrecarga de operadores
    bool operator==(const Jugador& otro) const;

    friend std::ostream& operator<<(std::ostream& os, const Jugador& j);
};

#endif
