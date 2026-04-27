#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>
#include <iostream>
#include "ActaEquipo.h"

// Representa un partido con toda su informacion y resultdo
// Es dueño de las dos ActaEquipo por valor
class Partido {
private:
    std::string fecha;
    std::string hora;
    std::string sede;
    std::string arbitros[3];
    ActaEquipo  actaLocal;
    ActaEquipo  actaVisitante;
    bool        resueltoEnProrroga;
    bool        simulado;

public:
    Partido();
    Partido(Equipo* local, Equipo* visitante,
            const std::string& fecha, const std::string& sede);
    Partido(const Partido& otro);  // copia — requerido por enunciado
    ~Partido();

    std::string  getFecha()             const;
    std::string  getHora()              const;
    std::string  getSede()              const;
    std::string  getArbitro(int i)      const;
    ActaEquipo&  getActaLocal();
    ActaEquipo&  getActaVisitante();
    bool         isResueltoEnProrroga() const;
    bool         isSimulado()           const;

    void setFecha             (const std::string& v);
    void setHora              (const std::string& v);
    void setSede              (const std::string& v);
    void setArbitro           (int i, const std::string& v);
    void setResueltoEnProrroga(bool v);

    // Devuelve puntero al equipo ganador, nullptr si empate
    Equipo* getGanador() const;

    // Actualiza historicos de equipos y jugadores al finalizar
    void actualizarHistoricos();

    // Imprime goleadores del partido
    void imprimirGoleadores() const;

    friend std::ostream& operator<<(std::ostream& os, const Partido& p);
};

#endif
