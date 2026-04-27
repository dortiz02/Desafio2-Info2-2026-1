#ifndef GRUPO_H
#define GRUPO_H

#include <string>
#include <iostream>
#include "Equipo.h"
#include "Partido.h"
#include "Lista.h"

class Simulador;

// Representa un grupo de la fase clasificatoria
// Contiene 4 equipos y genera/almacena sus 6 partidos
// Es dueno de los Partido por valor, pero NO de los Equipo (punteros)
class Grupo {
private:
    char           letra;
    Lista<Equipo*> equipos;
    Lista<Partido> partidos;
    int            puntos[4];  // indexado por posicion en equipos[], no en tabla

    /// Busca el indice de un equipo dentro de la lista interna
    /// Devuelve -1 si no se encuentra
    int indiceEquipo(Equipo* e) const;

public:
    Grupo();
    Grupo(char letra);
    Grupo(const Grupo& otro);
    Grupo& operator=(const Grupo& otro);
    ~Grupo();

    char                  getLetra()             const;
    Lista<Equipo*>&       getEquipos();
    Lista<Partido>&       getPartidos();
    const Lista<Partido>& getPartidos()          const;

    /// Devuelve puntos por indice interno (0..3)
    int getPuntos(int i)                         const;

    /// Devuelve puntos de un equipo especifico por puntero
    /// Uso correcto para obtener puntos de clasificados
    int getPuntosDeEquipo(Equipo* e)             const;

    void agregarEquipo   (Equipo* e);
    void generarPartidos ();
    void asignarFechas   (const std::string& fechaInicio,
                          int* partidosPorDia, int diasDisponibles);
    void actualizarPuntos(int idxLocal, int idxVisitante,
                          int golesL, int golesV);
    void simularPartidos (Simulador& sim);
    void imprimirTabla   () const;

    Lista<Equipo*> getTablaClasificacion() const;

    friend std::ostream& operator<<(std::ostream& os, const Grupo& g);
};

#endif
