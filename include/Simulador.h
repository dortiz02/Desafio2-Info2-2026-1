#ifndef SIMULADOR_H
#define SIMULADOR_H
#include <iostream>
#include <random>
#include "Partido.h"
#include "Lista.h"
class Simulador {
private:
    double alpha;
    double beta;
    double mu;
    double randProb() const;
public:
    Simulador();
    Simulador(const Simulador& otro);
    ~Simulador();
    double     calcularLambda    (const Equipo& a, const Equipo& b) const;
    double     calcularPosesion  (int rankingA, int rankingB)        const;
    Lista<int> elegirConvocados  (const Equipo& e)                   const;
    void       simularJugador    (ActaJugador& aj, int& golesRestantes) const;
    void       simularPartido    (Partido& p, bool prorroga = false);
    int        romperEmpate      (const Equipo& local, const Equipo& visitante) const;
    friend std::ostream& operator<<(std::ostream& os, const Simulador& s);
};
#endif
