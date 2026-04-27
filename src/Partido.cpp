#include "../include/Partido.h"
Partido::Partido()
    : fecha(""), hora("00:00"), sede("nombreSede"),
      resueltoEnProrroga(false), simulado(false) {
    arbitros[0]="codArbitro1"; arbitros[1]="codArbitro2"; arbitros[2]="codArbitro3";
}
Partido::Partido(Equipo* local, Equipo* visitante, const std::string& fec, const std::string& sed)
    : fecha(fec), hora("00:00"), sede(sed), actaLocal(local), actaVisitante(visitante),
      resueltoEnProrroga(false), simulado(false) {
    arbitros[0]="codArbitro1"; arbitros[1]="codArbitro2"; arbitros[2]="codArbitro3";
}
Partido::Partido(const Partido& otro)
    : fecha(otro.fecha), hora(otro.hora), sede(otro.sede),
      actaLocal(otro.actaLocal), actaVisitante(otro.actaVisitante),
      resueltoEnProrroga(otro.resueltoEnProrroga), simulado(otro.simulado) {
    arbitros[0]=otro.arbitros[0]; arbitros[1]=otro.arbitros[1]; arbitros[2]=otro.arbitros[2];
}
Partido::~Partido() {}

// Operador de asignacion — necesario para usar Partido en Lista<T>
Partido& Partido::operator=(const Partido& otro) {
    if (this == &otro) return *this;
    fecha               = otro.fecha;
    hora                = otro.hora;
    sede                = otro.sede;
    actaLocal           = otro.actaLocal;
    actaVisitante       = otro.actaVisitante;
    resueltoEnProrroga  = otro.resueltoEnProrroga;
    simulado            = otro.simulado;
    arbitros[0]         = otro.arbitros[0];
    arbitros[1]         = otro.arbitros[1];
    arbitros[2]         = otro.arbitros[2];
    return *this;
}
std::string Partido::getFecha()             const { return fecha;              }
std::string Partido::getHora()              const { return hora;               }
std::string Partido::getSede()              const { return sede;               }
bool        Partido::isResueltoEnProrroga() const { return resueltoEnProrroga; }
bool        Partido::isSimulado()           const { return simulado;           }
std::string Partido::getArbitro(int i)      const { return (i>=0&&i<3)?arbitros[i]:""; }
ActaEquipo&       Partido::getActaLocal()           { return actaLocal;     }
ActaEquipo&       Partido::getActaVisitante()        { return actaVisitante; }
const ActaEquipo& Partido::getActaLocal()     const  { return actaLocal;     }
const ActaEquipo& Partido::getActaVisitante() const  { return actaVisitante; }
void Partido::setFecha(const std::string& v)              { fecha = v; }
void Partido::setHora(const std::string& v)               { hora  = v; }
void Partido::setSede(const std::string& v)               { sede  = v; }
void Partido::setResueltoEnProrroga(bool v)               { resueltoEnProrroga = v; }
void Partido::setArbitro(int i, const std::string& v)     { if(i>=0&&i<3) arbitros[i]=v; }
Equipo* Partido::getGanador() const {
    if (actaLocal.getGolesAFavor() > actaVisitante.getGolesAFavor()) return actaLocal.getEquipo();
    if (actaVisitante.getGolesAFavor() > actaLocal.getGolesAFavor()) return actaVisitante.getEquipo();
    return nullptr;
}
void Partido::actualizarHistoricos() {
    bool ganoLocal = actaLocal.getGolesAFavor() > actaVisitante.getGolesAFavor();
    bool empate    = actaLocal.getGolesAFavor() == actaVisitante.getGolesAFavor();
    actaLocal.getEquipo()->actualizarStats(actaLocal.toStats(ganoLocal, empate));
    actaVisitante.getEquipo()->actualizarStats(actaVisitante.toStats(!ganoLocal && !empate, empate));
    Lista<ActaJugador>& convL = actaLocal.getConvocados();
    for (int i=0; i<convL.getTam(); i++) convL[i].getJugador()->actualizarStats(convL[i].toStats());
    Lista<ActaJugador>& convV = actaVisitante.getConvocados();
    for (int i=0; i<convV.getTam(); i++) convV[i].getJugador()->actualizarStats(convV[i].toStats());
    simulado = true;
}
void Partido::imprimirGoleadores() const {
    std::cout << "  Goleadores " << actaLocal.getEquipo()->getPais() << ": ";
    const Lista<ActaJugador>& cL = actaLocal.getConvocados();
    for (int i=0; i<cL.getTam(); i++)
        if (cL[i].getGoles()>0) std::cout << "#" << cL[i].getJugador()->getNumeroCamiseta() << "(" << cL[i].getGoles() << ") ";
    std::cout << "\n  Goleadores " << actaVisitante.getEquipo()->getPais() << ": ";
    const Lista<ActaJugador>& cV = actaVisitante.getConvocados();
    for (int i=0; i<cV.getTam(); i++)
        if (cV[i].getGoles()>0) std::cout << "#" << cV[i].getJugador()->getNumeroCamiseta() << "(" << cV[i].getGoles() << ") ";
    std::cout << "\n";
}
std::ostream& operator<<(std::ostream& os, const Partido& p) {
    os << "Fecha: " << p.fecha << " " << p.hora << " | Sede: " << p.sede
       << " | Arbitros: " << p.arbitros[0] << ", " << p.arbitros[1] << ", " << p.arbitros[2]
       << (p.resueltoEnProrroga ? " | PRORROGA" : "")
       << "\n  " << p.actaLocal << "\n  " << p.actaVisitante;
    return os;
}
