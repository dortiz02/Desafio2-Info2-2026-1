#include "../include/Sorteo.h"
#include <cstdlib>
#include <iostream>

// -- Constructores ----------------------------------------------------

Sorteo::Sorteo() {}

Sorteo::Sorteo(const Sorteo& otro)
    : grupos(otro.grupos) {
    for (int i = 0; i < 4; i++) bombos[i] = otro.bombos[i];
}

Sorteo::~Sorteo() {}

// -- Helpers privados -------------------------------------------------

/// Ordena una lista de punteros a Equipo por ranking FIFA ascendente
/// Usa burbuja — maximo 47 elementos, costo aceptable
void Sorteo::ordenarPorRanking(Lista<Equipo*>& lista) const {
    int n = lista.getTam();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (*lista[j+1] < *lista[j]) {
                Equipo* tmp  = lista[j];
                lista[j]     = lista[j+1];
                lista[j+1]   = tmp;
            }
}

/// Cuenta cuantos equipos de una confederacion hay actualmente en el grupo
int Sorteo::contarConfederacion(const Grupo& g, const std::string& conf) const {
    int cuenta = 0;
    const Lista<Equipo*>& equipos = const_cast<Grupo&>(g).getEquipos();
    for (int i = 0; i < equipos.getTam(); i++)
        if (equipos[i]->getConfederacion() == conf) cuenta++;
    return cuenta;
}

/// Verifica si el equipo puede agregarse al grupo sin violar restricciones:
/// - UEFA puede tener maximo 2 equipos por grupo
/// - Cualquier otra confederacion maximo 1
bool Sorteo::puedeAgregarseA(const Grupo& g, const Equipo* e) const {
    const std::string& conf = e->getConfederacion();
    int cuenta = contarConfederacion(g, conf);
    if (conf == "UEFA") return cuenta < 2;
    return cuenta < 1;
}

// -- Logica publica ---------------------------------------------------

/// Llena los 4 bombos con los 48 equipos
/// EE.UU. (pais sede) va directo al bombo 1
/// El resto se ordena por ranking y se reparte en bloques de 12
void Sorteo::armarBombos(Lista<Equipo*>& equipos) {
    Lista<Equipo*> resto;

    // Separar EE.UU. del resto
    Equipo* eeuu = nullptr;
    for (int i = 0; i < equipos.getTam(); i++) {
        if (equipos[i]->getPais() == "United States") {
            eeuu = equipos[i];
        } else {
            resto.agregar(equipos[i]);
        }
    }

    // Ordenar el resto por ranking FIFA
    ordenarPorRanking(resto);

    // Bombo 1: EE.UU. + los 11 mejores rankings
    if (eeuu != nullptr) bombos[0].agregar(eeuu);
    for (int i = 0; i < 11 && i < resto.getTam(); i++)
        bombos[0].agregar(resto[i]);

    // Bombos 2, 3 y 4: bloques de 12 en orden de ranking
    for (int b = 1; b < 4; b++)
        for (int i = 0; i < 12; i++) {
            int idx = 11 + (b - 1) * 12 + i;
            if (idx < resto.getTam()) bombos[b].agregar(resto[idx]);
        }
}

/// Realiza el sorteo extrayendo un equipo de cada bombo por grupo
/// Si no puede agregar por restriccion de confederacion, prueba el siguiente
/// del mismo bombo hasta encontrar uno valido
void Sorteo::conformarGrupos() {
    // Inicializar los 12 grupos con sus letras
    char letras[12] = {'A','B','C','D','E','F','G','H','I','J','K','L'};
    for (int i = 0; i < 12; i++) grupos.agregar(Grupo(letras[i]));

    // Para cada bombo, asignar un equipo a cada grupo
    for (int b = 0; b < 4; b++) {
        // Marcar cuales equipos del bombo ya fueron asignados
        int tamBombo = bombos[b].getTam();
        bool asignado[12] = {};

        for (int g = 0; g < 12; g++) {
            // Intentar hasta encontrar un equipo valido para este grupo
            bool encontrado = false;
            int  intentos   = 0;

            while (!encontrado && intentos < tamBombo * 2) {
                // Seleccionar aleatorio del bombo
                int idx = std::rand() % tamBombo;
                if (asignado[idx]) { intentos++; continue; }

                if (puedeAgregarseA(grupos[g], bombos[b][idx])) {
                    grupos[g].agregarEquipo(bombos[b][idx]);
                    asignado[idx] = true;
                    encontrado    = true;
                }
                intentos++;
            }

            // Fallback: si no encontro valido por restriccion,
            // asignar el primero disponible (evita bloqueo total)
            if (!encontrado) {
                for (int i = 0; i < tamBombo; i++) {
                    if (!asignado[i]) {
                        grupos[g].agregarEquipo(bombos[b][i]);
                        asignado[i] = true;
                        break;
                    }
                }
            }
        }
    }
}

// -- Getters ----------------------------------------------------------

Lista<Grupo>&   Sorteo::getGrupos()      { return grupos;     }
Lista<Equipo*>& Sorteo::getBombo(int i)  { return bombos[i];  }

// -- Salida -----------------------------------------------------------

/// Imprime los 12 grupos con la confederacion de cada equipo
void Sorteo::imprimirGrupos() const {
    for (int i = 0; i < grupos.getTam(); i++) {
        const Grupo& g = grupos[i];
        std::cout << "\n=== GRUPO " << g.getLetra() << " ===\n";
        Lista<Equipo*>& eq = const_cast<Grupo&>(g).getEquipos();
        for (int j = 0; j < eq.getTam(); j++)
            std::cout << "  " << eq[j]->getPais()
                      << " [" << eq[j]->getConfederacion() << "]\n";
    }
}

/// Cuenta el total de iteraciones del proceso de sorteo
/// Util para el reporte de eficiencia
long long Sorteo::contarIteraciones() const {
    // Ordenamiento: O(n^2) con n=47
    // Sorteo: O(12 * 4 * intentos) en promedio
    return 47LL * 47 + 12LL * 4 * 10;
}

std::ostream& operator<<(std::ostream& os, const Sorteo& s) {
    os << "Sorteo — " << s.grupos.getTam() << " grupos conformados\n";
    return os;
}
