#include "../include/ActaEquipo.h"
ActaEquipo::ActaEquipo() : equipo(nullptr), golesAFavor(0), golesEnContra(0), posesion(0.0) {}
ActaEquipo::ActaEquipo(Equipo* e) : equipo(e), golesAFavor(0), golesEnContra(0), posesion(0.0) {}
ActaEquipo::ActaEquipo(const ActaEquipo& otra) : equipo(otra.equipo), golesAFavor(otra.golesAFavor), golesEnContra(otra.golesEnContra), posesion(otra.posesion), convocados(otra.convocados) {}
ActaEquipo::~ActaEquipo() {}
Equipo* ActaEquipo::getEquipo()        const { return equipo;        }
int     ActaEquipo::getGolesAFavor()   const { return golesAFavor;   }
int     ActaEquipo::getGolesEnContra() const { return golesEnContra; }
double  ActaEquipo::getPosesion()      const { return posesion;      }
Lista<ActaJugador>& ActaEquipo::getConvocados() { return convocados; }
const Lista<ActaJugador>& ActaEquipo::getConvocados() const { return convocados; }
void ActaEquipo::setGolesAFavor  (int v)    { golesAFavor   = v; }
void ActaEquipo::setGolesEnContra(int v)    { golesEnContra = v; }
void ActaEquipo::setPosesion     (double v) { posesion      = v; }
void ActaEquipo::agregarConvocado(const ActaJugador& aj) { convocados.agregar(aj); }
EstadisticasEquipo ActaEquipo::toStats(bool gano, bool empato) const {
    EstadisticasEquipo s;
    s.setGolesAFavor(golesAFavor); s.setGolesEnContra(golesEnContra);
    s.setPartidosGanados(gano ? 1 : 0); s.setPartidosEmpatados(empato ? 1 : 0);
    s.setPartidosPerdidos((!gano && !empato) ? 1 : 0);
    int am=0, ro=0, fa=0;
    for (int i=0; i<convocados.getTam(); i++) {
        am += convocados[i].getTarjetasAmarillas();
        ro += convocados[i].getTarjetasRojas();
        fa += convocados[i].getFaltas();
    }
    s.setTarjetasAmarillas(am); s.setTarjetasRojas(ro); s.setFaltas(fa);
    return s;
}
std::ostream& operator<<(std::ostream& os, const ActaEquipo& ae) {
    if (ae.equipo != nullptr) os << ae.equipo->getPais();
    os << " | GF: " << ae.golesAFavor << " | GC: " << ae.golesEnContra
       << " | Posesion: " << ae.posesion << "%";
    for (int i=0; i<ae.convocados.getTam(); i++) os << "\n    " << ae.convocados[i];
    return os;
}
