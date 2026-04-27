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
    Lista<Jugador>     plantilla;

public:
    Equipo();
    Equipo(const std::string& pais, const std::string& confederacion,
           int ranking, const std::string& dt,
           int golesAFavor, int golesEnContra,
           int ganados, int empatados, int perdidos);
    Equipo(const Equipo& otro);
    ~Equipo();

    std::string         getPais()          const;
    std::string         getConfederacion() const;
    int                 getRankingFIFA()   const;
    std::string         getDT()            const;
    EstadisticasEquipo& getStatsHistoricas();
    const EstadisticasEquipo& getStatsHistoricas() const;
    Lista<Jugador>&     getPlantilla();

    double getPromGF() const;
    double getPromGC() const;

    void setPais          (const std::string& v);
    void setConfederacion (const std::string& v);
    void setRankingFIFA   (int v);
    void setDT            (const std::string& v);

    void generarPlantilla();
    void actualizarStats(const EstadisticasEquipo& statsPartido);

    void guardarPlantilla(std::fstream& archivo) const;
    void cargarPlantilla (std::fstream& archivo);

    bool operator<(const Equipo& otro) const;
    friend std::ostream& operator<<(std::ostream& os, const Equipo& e);
};

#endif
