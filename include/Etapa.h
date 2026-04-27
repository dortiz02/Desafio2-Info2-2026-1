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
    std::string    nombre;          // "R16", "R8", "QF", "SF", "3er", "Final"
    Lista<Partido> partidos;
    Lista<Equipo*> clasificados;    // equipos que pasan a la siguiente etapa

public:
    // Constructores
    Etapa();
    Etapa(const std::string& nombre);
    Etapa(const Etapa& otra);       // copia — requerido por enunciado
    ~Etapa();

    // Getters
    std::string     getNombre()       const;
    Lista<Partido>& getPartidos();
    Lista<Equipo*>& getClasificados();

    // Agrega un partido ya configurado a la etapa
    void agregarPartido(const Partido& p);

    // Simula todos los partidos de la etapa
    // En eliminatorias, si hay empate se va a prorroga
    // y se llama a romperEmpate del Simulador
    void simularEtapa(Simulador& sim);

    // Llena la lista de clasificados tras simular
    // Para la final tambien determina el 3er puesto
    void determinarClasificados();

    // Calcula el total de bytes que ocupa esta etapa en memoria
    size_t calcularMemoria() const;

    // Imprime nombre de etapa, datos de partidos y goleadores
    void imprimir() const;

    friend std::ostream& operator<<(std::ostream& os, const Etapa& e);
};

#endif
