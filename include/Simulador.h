#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <iostream>
#include <random>
#include "Partido.h"
#include "Lista.h"

// Contiene toda la logica probabilistica de simulacion
// Separado de Partido para no mezclar responsabilidades
class Simulador {
private:
    double alpha;   // 0.6 — peso del promedio de goles a favor
    double beta;    // 0.4 — peso del promedio de goles en contra
    double mu;      // 1.35 — factor de escala global

    // Genera numero aleatorio uniforme [0.0, 1.0)
    // Usa mt19937 de C++ — sin rand() ni srand()
    double randProb() const;

public:
    Simulador();
    Simulador(const Simulador& otro);  // copia — requerido por enunciado
    ~Simulador();

    // Calcula goles esperados del equipo A contra B
    double calcularLambda(const Equipo& a, const Equipo& b) const;

    // Calcula posesion proporcional inversa al ranking FIFA
    double calcularPosesion(int rankingA, int rankingB) const;

    // Selecciona aleatoriamente 11 indices de la plantilla
    Lista<int> elegirConvocados(const Equipo& e) const;

    // Simula las metricas de un jugador convocado
    void simularJugador(ActaJugador& aj, int& golesRestantes) const;

    // Simula un partido completo — llena las dos actas
    // Si prorroga=true, minutos se contabilizan en 120
    void simularPartido(Partido& p, bool prorroga = false);

    // Resuelve empate usando ranking FIFA como sesgo aleatorio
    // Devuelve 0 si gana local, 1 si gana visitante
    int romperEmpate(const Equipo& local, const Equipo& visitante) const;

    friend std::ostream& operator<<(std::ostream& os, const Simulador& s);
};

#endif
