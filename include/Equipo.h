#ifndef EQUIPO_H
#define EQUIPO_H

#include <string>
#include <fstream>
#include <iostream>
#include "EstadisticasEquipo.h"
#include "Jugador.h"
#include "Lista.h"

class Equipo {
private:
    std::string        pais;
    std::string        confederacion;
    int                rankingFIFA;
    std::string        directorTecnico;
    EstadisticasEquipo statsHistoricas;
    Lista<Jugador>     plantilla;        // 26 jugadores, dueño de los objetos

public:
    // Constructores
    Equipo();
    Equipo(const std::string& pais, const std::string& confederacion,
           int ranking, const std::string& dt,
           int golesAFavor, int golesEnContra,
           int ganados, int empatados, int perdidos);
    Equipo(const Equipo& otro);  // copia — requerido por enunciado

    // Destructor
    ~Equipo();

    // Getters
    std::string         getPais()             const;
    std::string         getConfederacion()    const;
    int                 getRankingFIFA()      const;
    std::string         getDT()               const;
    EstadisticasEquipo& getStatsHistoricas();
    Lista<Jugador>&     getPlantilla();

    // Promedios para la fórmula de goles esperados
    double getPromGF() const;   // golesAFavor / partidosTotales
    double getPromGC() const;   // golesEnContra / partidosTotales

    // Setters
    void setPais          (const std::string& v);
    void setConfederacion (const std::string& v);
    void setRankingFIFA   (int v);
    void setDT            (const std::string& v);

    // Genera la plantilla artificial de 26 jugadores
    // Reparte los goles históricos uniformemente entre ellos
    void generarPlantilla();

    // Acumula las stats de un partido sobre el histórico
    void actualizarStats(const EstadisticasEquipo& statsPartido);

    // Persistencia de la plantilla — solo inicio y fin del programa
    void guardarPlantilla(std::fstream& archivo) const;
    void cargarPlantilla (std::fstream& archivo);

    // Sobrecarga de operadores
    bool operator<(const Equipo& otro) const;  // por rankingFIFA
    friend std::ostream& operator<<(std::ostream& os, const Equipo& e);
};

#endif
