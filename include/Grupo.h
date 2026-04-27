#ifndef GRUPO_H
#define GRUPO_H

#include <string>
#include <iostream>
#include "Equipo.h"
#include "Partido.h"
#include "Lista.h"

// Representa un grupo de la fase clasificatoria
// Contiene 4 equipos y genera/almacena sus 6 partidos
// Es dueño de los Partido por valor, pero NO de los Equipo (punteros)
class Grupo {
private:
    char           letra;
    Lista<Equipo*> equipos;    // 4 punteros — no es dueño
    Lista<Partido> partidos;   // 6 partidos — dueño por valor
    int            puntos[4];  // puntos acumulados por equipo en el grupo

    // indice del equipo en la lista interna, -1 si no está
    int indiceEquipo(Equipo* e) const;

public:
    // Constructores
    Grupo();
    Grupo(char letra);
    Grupo(const Grupo& otro);  // copia — requerido por enunciado
    ~Grupo();

    // Getters
    char            getLetra()    const;
    Lista<Equipo*>& getEquipos();
    Lista<Partido>& getPartidos();
    int             getPuntos(int i) const;

    // Agrega un equipo al grupo (máx 4)
    void agregarEquipo(Equipo* e);

    // Genera los 6 partidos round-robin entre los 4 equipos
    // Debe llamarse después de tener los 4 equipos asignados
    void generarPartidos();

    // Asigna fechas a los partidos respetando:
    // - Máx 4 partidos por día en todo el torneo (se controla externamente)
    // - Ningún equipo juega dos veces en menos de 3 días
    // fechaInicio: "DD-MM-YYYY"
    void asignarFechas(const std::string& fechaInicio, int* partidosPorDia, int diasDisponibles);

    // Devuelve los equipos ordenados por puntos (criterios de desempate incluidos)
    Lista<Equipo*> getTablaClasificacion() const;

    // Simula todos los partidos del grupo y actualiza históricos
    void simularPartidos(Simulador& sim);

    // Actualiza puntos internos según resultado de un partido
    void actualizarPuntos(int idxLocal, int idxVisitante, int golesL, int golesV);

    // Imprime la tabla de clasificacion del grupo
    void imprimirTabla() const;

    friend std::ostream& operator<<(std::ostream& os, const Grupo& g);
};

#endif
