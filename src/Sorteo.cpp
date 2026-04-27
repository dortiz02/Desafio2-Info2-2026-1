#include "../include/Sorteo.h"
#include <cstdlib>
#include <iostream>

Sorteo::Sorteo() {}
Sorteo::Sorteo(const Sorteo& otro) : grupos(otro.grupos) {
    for (int i=0;i<4;i++) bombos[i]=otro.bombos[i];
}
Sorteo::~Sorteo() {}

/// Ordena lista de punteros a Equipo por ranking FIFA ascendente
/// Usa burbuja — maximo 47 elementos
void Sorteo::ordenarPorRanking(Lista<Equipo*>& lista) const {
    int n=lista.getTam();
    for (int i=0;i<n-1;i++)
        for (int j=0;j<n-i-1;j++)
            if (*lista[j+1] < *lista[j]) {
                Equipo* tmp=lista[j]; lista[j]=lista[j+1]; lista[j+1]=tmp;
            }
}

/// Cuenta equipos de una confederacion en el grupo
int Sorteo::contarConfederacion(const Grupo& g, const std::string& conf) const {
    int cuenta=0;
    Lista<Equipo*>& eq=const_cast<Grupo&>(g).getEquipos();
    for (int i=0;i<eq.getTam();i++) if (eq[i]->getConfederacion()==conf) cuenta++;
    return cuenta;
}

/// Verifica si el equipo puede agregarse sin violar restricciones:
/// UEFA max 2, cualquier otra confederacion max 1
bool Sorteo::puedeAgregarseA(const Grupo& g, const Equipo* e) const {
    const std::string& conf=e->getConfederacion();
    int cuenta=contarConfederacion(g,conf);
    if (conf=="UEFA") return cuenta<2;
    return cuenta<1;
}

/// Llena los 4 bombos: EE.UU. directo al bombo 1,
/// resto ordenado por ranking en bloques de 12
void Sorteo::armarBombos(Lista<Equipo*>& equipos) {
    Lista<Equipo*> resto;
    Equipo* eeuu=nullptr;
    for (int i=0;i<equipos.getTam();i++) {
        if (equipos[i]->getPais()=="United States") eeuu=equipos[i];
        else resto.agregar(equipos[i]);
    }
    ordenarPorRanking(resto);
    if (eeuu!=nullptr) bombos[0].agregar(eeuu);
    for (int i=0;i<11&&i<resto.getTam();i++) bombos[0].agregar(resto[i]);
    for (int b=1;b<4;b++)
        for (int i=0;i<12;i++) {
            int idx=11+(b-1)*12+i;
            if (idx<resto.getTam()) bombos[b].agregar(resto[idx]);
        }
}

/// Sorteo: un equipo de cada bombo por grupo
/// Si hay conflicto de confederacion, prueba otro del mismo bombo
/// Fallback: asigna el primero disponible para evitar bloqueo total
void Sorteo::conformarGrupos() {
    char letras[12]={'A','B','C','D','E','F','G','H','I','J','K','L'};
    for (int i=0;i<12;i++) grupos.agregar(Grupo(letras[i]));

    for (int b=0;b<4;b++) {
        int tam=bombos[b].getTam();
        bool asignado[12]={};
        for (int g=0;g<12;g++) {
            bool encontrado=false;
            int intentos=0;
            while (!encontrado && intentos<tam*2) {
                int idx=std::rand()%tam;
                if (asignado[idx]) { intentos++; continue; }
                if (puedeAgregarseA(grupos[g],bombos[b][idx])) {
                    grupos[g].agregarEquipo(bombos[b][idx]);
                    asignado[idx]=true;
                    encontrado=true;
                }
                intentos++;
            }
            // Fallback: primer disponible
            if (!encontrado)
                for (int i=0;i<tam;i++)
                    if (!asignado[i]) { grupos[g].agregarEquipo(bombos[b][i]); asignado[i]=true; break; }
        }
    }
}

Lista<Grupo>&   Sorteo::getGrupos()     { return grupos;    }
Lista<Equipo*>& Sorteo::getBombo(int i) { return bombos[i]; }

void Sorteo::imprimirGrupos() const {
    for (int i=0;i<grupos.getTam();i++) {
        const Grupo& g=grupos[i];
        std::cout << "\n=== GRUPO " << g.getLetra() << " ===\n";
        Lista<Equipo*>& eq=const_cast<Grupo&>(g).getEquipos();
        for (int j=0;j<eq.getTam();j++)
            std::cout << "  " << eq[j]->getPais() << " [" << eq[j]->getConfederacion() << "]\n";
    }
}

long long Sorteo::contarIteraciones() const {
    return 47LL*47 + 12LL*4*10;
}

std::ostream& operator<<(std::ostream& os, const Sorteo& s) {
    os << "Sorteo — " << s.grupos.getTam() << " grupos conformados\n";
    return os;
}
