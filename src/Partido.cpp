#include "../include/Partido.h"

// ── Constructores ─────────────────────────────────────────────

Partido::Partido()
    : fecha(""), hora("00:00"), sede("nombreSede"),
      resueltoEnProrroga(false), simulado(false) {
    arbitros[0] = "codArbitro1";
    arbitros[1] = "codArbitro2";
    arbitros[2] = "codArbitro3";
}

Partido::Partido(Equipo* local, Equipo* visitante,
                 const std::string& fec, const std::string& sed)
    : fecha(fec), hora("00:00"), sede(sed),
      actaLocal(local), actaVisitante(visitante),
      resueltoEnProrroga(false), simulado(false) {
    arbitros[0] = "codArbitro1";
    arbitros[1] = "codArbitro2";
    arbitros[2] = "codArbitro3";
}

Partido::Partido(const Partido& otro)
    : fecha              (otro.fecha),
      hora               (otro.hora),
      sede               (otro.sede),
      actaLocal          (otro.actaLocal),
      actaVisitante      (otro.actaVisitante),
      resueltoEnProrroga (otro.resueltoEnProrroga),
      simulado           (otro.simulado) {
    arbitros[0] = otro.arbitros[0];
    arbitros[1] = otro.arbitros[1];
    arbitros[2] = otro.arbitros[2];
}

Partido::~Partido() {}

// ── Getters ───────────────────────────────────────────────────

std::string Partido::getFecha()             const { return fecha;              }
std::string Partido::getHora()              const { return hora;               }
std::string Partido::getSede()              const { return sede;               }
bool        Partido::isResueltoEnProrroga() const { return resueltoEnProrroga; }
bool        Partido::isSimulado()           const { return simulado;           }

std::string Partido::getArbitro(int i) const {
    if (i >= 0 && i < 3) return arbitros[i];
    return "";
}

ActaEquipo& Partido::getActaLocal()     { return actaLocal;     }
ActaEquipo& Partido::getActaVisitante() { return actaVisitante; }

// ── Setters ───────────────────────────────────────────────────

void Partido::setFecha             (const std::string& v) { fecha = v; }
void Partido::setHora              (const std::string& v) { hora  = v; }
void Partido::setSede              (const std::string& v) { sede  = v; }
void Partido::setResueltoEnProrroga(bool v) { resueltoEnProrroga = v;  }

void Partido::setArbitro(int i, const std::string& v) {
    if (i >= 0 && i < 3) arbitros[i] = v;
}

// ── Lógica ────────────────────────────────────────────────────

// Devuelve puntero al ganador, nullptr si empate
Equipo* Partido::getGanador() const {
    if (actaLocal.getGolesAFavor() > actaVisitante.getGolesAFavor())
        return actaLocal.getEquipo();
    if (actaVisitante.getGolesAFavor() > actaLocal.getGolesAFavor())
        return actaVisitante.getEquipo();
    return nullptr;  // empate
}

// Actualiza historicos de equipos y jugadores al terminar el partido
// Se llama una sola vez por partido, al finalizar
void Partido::actualizarHistoricos() {
    bool ganoLocal = actaLocal.getGolesAFavor() > actaVisitante.getGolesAFavor();
    bool empate    = actaLocal.getGolesAFavor() == actaVisitante.getGolesAFavor();

    // Actualizar equipo local
    actaLocal.getEquipo()->actualizarStats(
        actaLocal.toStats(ganoLocal, empate));

    // Actualizar equipo visitante
    actaVisitante.getEquipo()->actualizarStats(
        actaVisitante.toStats(!ganoLocal && !empate, empate));

    // Actualizar jugadores convocados del local
    Lista<ActaJugador>& convLocal = actaLocal.getConvocados();
    for (int i = 0; i < convLocal.getTam(); i++)
        convLocal[i].getJugador()->actualizarStats(convLocal[i].toStats());

    // Actualizar jugadores convocados del visitante
    Lista<ActaJugador>& convVisit = actaVisitante.getConvocados();
    for (int i = 0; i < convVisit.getTam(); i++)
        convVisit[i].getJugador()->actualizarStats(convVisit[i].toStats());

    simulado = true;
}

// Imprime número de camiseta de los goleadores de cada equipo
void Partido::imprimirGoleadores() const {
    std::cout << "  Goleadores " << actaLocal.getEquipo()->getPais() << ": ";
    const Lista<ActaJugador>& convLocal = actaLocal.getConvocados();
    for (int i = 0; i < convLocal.getTam(); i++)
        if (convLocal[i].getGoles() > 0)
            std::cout << "#" << convLocal[i].getJugador()->getNumeroCamiseta()
                      << "(" << convLocal[i].getGoles() << ") ";

    std::cout << "\n  Goleadores " << actaVisitante.getEquipo()->getPais() << ": ";
    const Lista<ActaJugador>& convVisit = actaVisitante.getConvocados();
    for (int i = 0; i < convVisit.getTam(); i++)
        if (convVisit[i].getGoles() > 0)
            std::cout << "#" << convVisit[i].getJugador()->getNumeroCamiseta()
                      << "(" << convVisit[i].getGoles() << ") ";
    std::cout << "\n";
}

// ── Operadores ────────────────────────────────────────────────

std::ostream& operator<<(std::ostream& os, const Partido& p) {
    os << "Fecha: " << p.fecha << " " << p.hora
       << " | Sede: " << p.sede
       << " | Arbitros: " << p.arbitros[0] << ", "
                          << p.arbitros[1] << ", "
                          << p.arbitros[2]
       << (p.resueltoEnProrroga ? " | PRORROGA" : "")
       << "\n  " << p.actaLocal
       << "\n  " << p.actaVisitante;
    return os;
}
