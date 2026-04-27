#ifndef SORTEO_H
#define SORTEO_H
#include <iostream>
#include "Equipo.h"
#include "Grupo.h"
#include "Lista.h"

// Maneja la conformacion de los 12 grupos del mundial
// Arma los 4 bombos y realiza el sorteo respetando
// las restricciones de confederacion
class Sorteo {
private:
    Lista<Equipo*> bombos[4];
    Lista<Grupo>   grupos;

    void ordenarPorRanking(Lista<Equipo*>& lista) const;
    bool puedeAgregarseA(const Grupo& g, const Equipo* e) const;
    int  contarConfederacion(const Grupo& g, const std::string& conf) const;

public:
    Sorteo();
    Sorteo(const Sorteo& otro);
    ~Sorteo();

    void armarBombos(Lista<Equipo*>& equipos);
    void conformarGrupos();

    Lista<Grupo>&   getGrupos();
    Lista<Equipo*>& getBombo(int i);

    void      imprimirGrupos()       const;
    long long contarIteraciones()    const;

    friend std::ostream& operator<<(std::ostream& os, const Sorteo& s);
};
#endif
