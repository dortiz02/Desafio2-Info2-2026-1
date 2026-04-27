#include "../include/Etapa.h"
#include <iostream>

Etapa::Etapa() : nombre("") {}
Etapa::Etapa(const std::string& nom) : nombre(nom) {}
Etapa::Etapa(const Etapa& otra)
    : nombre(otra.nombre), partidos(otra.partidos), clasificados(otra.clasificados) {}
Etapa::~Etapa() {}

// Operador de asignacion — necesario para usar Etapa en Lista<T>
Etapa& Etapa::operator=(const Etapa& otra) {
    if (this == &otra) return *this;
    nombre       = otra.nombre;
    partidos     = otra.partidos;
    clasificados = otra.clasificados;
    return *this;
}

std::string           Etapa::getNombre()      const { return nombre;       }
Lista<Partido>&       Etapa::getPartidos()          { return partidos;     }
const Lista<Partido>& Etapa::getPartidos()    const { return partidos;     }
Lista<Equipo*>&       Etapa::getClasificados()      { return clasificados; }

/// Agrega un partido ya configurado con equipos y fecha asignados
void Etapa::agregarPartido(const Partido& p) {
    partidos.agregar(p);
}

/// Simula todos los partidos de la etapa
/// Si hay empate, se resuelve en prorroga usando ranking FIFA como sesgo
void Etapa::simularEtapa(Simulador& sim) {
    for (int i = 0; i < partidos.getTam(); i++) {
        sim.simularPartido(partidos[i], false);

        if (partidos[i].getGanador() == nullptr) {
            // Limpiar actas del intento anterior
            partidos[i].getActaLocal().getConvocados().limpiar();
            partidos[i].getActaVisitante().getConvocados().limpiar();
            partidos[i].getActaLocal().setGolesAFavor(0);
            partidos[i].getActaLocal().setGolesEnContra(0);
            partidos[i].getActaVisitante().setGolesAFavor(0);
            partidos[i].getActaVisitante().setGolesEnContra(0);

            int ganador = sim.romperEmpate(
                *partidos[i].getActaLocal().getEquipo(),
                *partidos[i].getActaVisitante().getEquipo());

            sim.simularPartido(partidos[i], true);

            // Forzar resultado si sigue empatado tras prorroga
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
        partidos[i].actualizarHistoricos();
    }
}

/// Llena clasificados con el ganador de cada partido
void Etapa::determinarClasificados() {
    clasificados.limpiar();
    for (int i = 0; i < partidos.getTam(); i++) {
        Equipo* ganador = partidos[i].getGanador();
        if (ganador != nullptr) clasificados.agregar(ganador);
    }
}

/// Calcula bytes ocupados por esta etapa en memoria
/// Incluye partidos, actas de jugadores y clasificados
size_t Etapa::calcularMemoria() const {
    size_t mem = sizeof(Etapa);
    mem += sizeof(Partido)    * partidos.getTam();
    mem += sizeof(Equipo*)    * clasificados.getTam();
    mem += sizeof(ActaJugador) * 11 * 2 * partidos.getTam();
    return mem;
}

/// Imprime nombre, datos de cada partido y goleadores
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
       << " | Partidos: "    << e.partidos.getTam()
       << " | Clasificados: " << e.clasificados.getTam();
    return os;
}
