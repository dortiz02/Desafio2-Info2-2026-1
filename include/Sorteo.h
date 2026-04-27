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
    Lista<Equipo*> bombos[4];   // 4 bombos de 12 equipos cada uno
    Lista<Grupo>   grupos;      // 12 grupos resultado del sorteo

    // Ordena los equipos por ranking FIFA (burbuja — max 47 elementos)
    void ordenarPorRanking(Lista<Equipo*>& lista) const;

    // Verifica si un equipo puede entrar a un grupo sin violar restricciones
    // - No dos equipos de la misma confederacion salvo UEFA (max 2)
    bool puedeAgregarseA(const Grupo& g, const Equipo* e) const;

    // Cuenta cuantos equipos de una confederacion hay en el grupo
    int contarConfederacion(const Grupo& g, const std::string& conf) const;

public:
    Sorteo();
    Sorteo(const Sorteo& otro);  // copia — requerido por enunciado
    ~Sorteo();

    // Llena los 4 bombos a partir de la lista de 48 equipos
    // EE.UU. va directo al bombo 1 como pais sede
    void armarBombos(Lista<Equipo*>& equipos);

    // Realiza el sorteo: extrae un equipo de cada bombo por grupo
    // respetando las restricciones de confederacion
    void conformarGrupos();

    // Getters
    Lista<Grupo>&   getGrupos();
    Lista<Equipo*>& getBombo(int i);

    // Imprime los grupos conformados con confederacion de cada equipo
    void imprimirGrupos() const;

    long long contarIteraciones() const;

    friend std::ostream& operator<<(std::ostream& os, const Sorteo& s);
};

#endif
