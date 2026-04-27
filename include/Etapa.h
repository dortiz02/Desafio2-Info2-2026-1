#ifndef ETAPA_H
#define ETAPA_H
#include <string>
#include <iostream>
#include "Partido.h"
#include "Simulador.h"
#include "Lista.h"

// Representa una etapa eliminatoria del torneo (R16, R8, QF, SF, F, 3er)
// Es duena de los partidos por valor
// No es duena de los equipos — solo guarda punteros
class Etapa {
private:
    std::string    nombre;
    Lista<Partido> partidos;
    Lista<Equipo*> clasificados;

public:
    Etapa();
    Etapa(const std::string& nombre);
    Etapa(const Etapa& otra);
    Etapa& operator=(const Etapa& otra);
    ~Etapa();

    std::string     getNombre()      const;
    Lista<Partido>& getPartidos();
    const Lista<Partido>& getPartidos() const;
    Lista<Equipo*>& getClasificados();

    void   agregarPartido(const Partido& p);
    void   simularEtapa(Simulador& sim);
    void   determinarClasificados();
    size_t calcularMemoria() const;
    void   imprimir() const;

    friend std::ostream& operator<<(std::ostream& os, const Etapa& e);
};
#endif
