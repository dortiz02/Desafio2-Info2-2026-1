#include "../include/Etapa.h"
#include <iostream>

// -- Constructores ----------------------------------------------------

Etapa::Etapa() : nombre("") {}

Etapa::Etapa(const std::string& nom) : nombre(nom) {}

Etapa::Etapa(const Etapa& otra)
    : nombre      (otra.nombre),
      partidos    (otra.partidos),
      clasificados(otra.clasificados) {}

Etapa::~Etapa() {}

// -- Getters ----------------------------------------------------------

std::string     Etapa::getNombre()        const { return nombre;       }
Lista<Partido>& Etapa::getPartidos()            { return partidos;     }
Lista<Equipo*>& Etapa::getClasificados()        { return clasificados; }

// -- Logica -----------------------------------------------------------

/// Agrega un partido ya configurado (con equipos y fecha asignados)
void Etapa::agregarPartido(const Partido& p) {
    partidos.agregar(p);
}

/// Simula todos los partidos de la etapa
/// Si el resultado es empate, se resuelve en prorroga usando
/// el ranking FIFA como sesgo aleatorio (romperEmpate del Simulador)
void Etapa::simularEtapa(Simulador& sim) {
    for (int i = 0; i < partidos.getTam(); i++) {
        // Primera simulacion sin prorroga
        sim.simularPartido(partidos[i], false);

        // Si empate, resimular en prorroga con ganador forzado
        if (partidos[i].getGanador() == nullptr) {
            // Limpiar actas del intento anterior
            partidos[i].getActaLocal().getConvocados().limpiar();
            partidos[i].getActaVisitante().getConvocados().limpiar();
            partidos[i].getActaLocal().setGolesAFavor(0);
            partidos[i].getActaLocal().setGolesEnContra(0);
            partidos[i].getActaVisitante().setGolesAFavor(0);
            partidos[i].getActaVisitante().setGolesEnContra(0);

            // Determinar ganador por ranking
            int ganador = sim.romperEmpate(
                *partidos[i].getActaLocal().getEquipo(),
                *partidos[i].getActaVisitante().getEquipo());

            // Sumar un gol al equipo ganador para forzar resultado
            // y simular en prorroga (120 minutos)
            sim.simularPartido(partidos[i], true);

            // Forzar resultado si sigue empatado
            if (partidos[i].getGanador() == nullptr) {
                if (ganador == 0) {
                    int gf = partidos[i].getActaLocal().getGolesAFavor();
                    partidos[i].getActaLocal().setGolesAFavor(gf + 1);
                    partidos[i].getActaVisitante().setGolesEnContra(gf + 1);
                } else {
                    int gf = partidos[i].getActaVisitante().getGolesAFavor();
                    partidos[i].getActaVisitante().setGolesAFavor(gf + 1);
                    partidos[i].getActaLocal().setGolesEnContra(gf + 1);
                }
                partidos[i].setResueltoEnProrroga(true);
            }
        }

        // Actualizar historicos al finalizar el partido
        partidos[i].actualizarHistoricos();
    }
}

/// Llena la lista de clasificados con el ganador de cada partido
void Etapa::determinarClasificados() {
    clasificados.limpiar();
    for (int i = 0; i < partidos.getTam(); i++) {
        Equipo* ganador = partidos[i].getGanador();
        if (ganador != nullptr) clasificados.agregar(ganador);
    }
}

/// Calcula memoria ocupada por esta etapa en bytes
/// Incluye partidos, actas y clasificados
size_t Etapa::calcularMemoria() const {
    size_t mem = sizeof(Etapa);
    mem += sizeof(Partido) * partidos.getTam();
    mem += sizeof(Equipo*) * clasificados.getTam();
    // Cada partido tiene 2 actas con 11 ActaJugador cada una
    mem += sizeof(ActaJugador) * 11 * 2 * partidos.getTam();
    return mem;
}

/// Imprime nombre de etapa, datos de cada partido y goleadores
void Etapa::imprimir() const {
    std::cout << "\n========================================\n";
    std::cout << "ETAPA: " << nombre << "\n";
    std::cout << "========================================\n";
    for (int i = 0; i < partidos.getTam(); i++) {
        std::cout << "\nPartido " << (i+1) << ":\n";
        std::cout << partidos[i] << "\n";
        partidos[i].imprimirGoleadores();
    }
}

std::ostream& operator<<(std::ostream& os, const Etapa& e) {
    os << "Etapa: " << e.nombre
       << " | Partidos: " << e.partidos.getTam()
       << " | Clasificados: " << e.clasificados.getTam();
    return os;
}
