#include "../include/ActaEquipo.h"

// ── Constructores ─────────────────────────────────────────────

ActaEquipo::ActaEquipo()
    : equipo(nullptr), golesAFavor(0), golesEnContra(0), posesion(0.0) {}

ActaEquipo::ActaEquipo(Equipo* e)
    : equipo(e), golesAFavor(0), golesEnContra(0), posesion(0.0) {}

// Copia el puntero — no duplica el Equipo
ActaEquipo::ActaEquipo(const ActaEquipo& otra)
    : equipo      (otra.equipo),
      golesAFavor (otra.golesAFavor),
      golesEnContra(otra.golesEnContra),
      posesion    (otra.posesion),
      convocados  (otra.convocados) {}   // Lista<T> tiene su propio copy

ActaEquipo::~ActaEquipo() {}

// ── Getters ───────────────────────────────────────────────────

Equipo* ActaEquipo::getEquipo()        const { return equipo;        }
int     ActaEquipo::getGolesAFavor()   const { return golesAFavor;   }
int     ActaEquipo::getGolesEnContra() const { return golesEnContra; }
double  ActaEquipo::getPosesion()      const { return posesion;      }

Lista<ActaJugador>& ActaEquipo::getConvocados() { return convocados; }

// ── Setters ───────────────────────────────────────────────────

void ActaEquipo::setGolesAFavor  (int v)    { golesAFavor   = v; }
void ActaEquipo::setGolesEnContra(int v)    { golesEnContra = v; }
void ActaEquipo::setPosesion     (double v) { posesion      = v; }

void ActaEquipo::agregarConvocado(const ActaJugador& aj) {
    convocados.agregar(aj);
}

// ── Logica ────────────────────────────────────────────────────

// Empaqueta los datos del acta en EstadisticasEquipo
// para acumular sobre el histórico con operator+=
EstadisticasEquipo ActaEquipo::toStats(bool gano, bool empato) const {
    EstadisticasEquipo s;
    s.setGolesAFavor      (golesAFavor);
    s.setGolesEnContra    (golesEnContra);
    s.setPartidosGanados  (gano   ? 1 : 0);
    s.setPartidosEmpatados(empato ? 1 : 0);
    s.setPartidosPerdidos ((!gano && !empato) ? 1 : 0);

    // Acumula tarjetas y faltas de los convocados
    int amarillas = 0, rojas = 0, faltas = 0;
    for (int i = 0; i < convocados.getTam(); i++) {
        amarillas += convocados[i].getTarjetasAmarillas();
        rojas     += convocados[i].getTarjetasRojas();
        faltas    += convocados[i].getFaltas();
    }
    s.setTarjetasAmarillas(amarillas);
    s.setTarjetasRojas    (rojas);
    s.setFaltas           (faltas);
    return s;
}

// ── Operadores ────────────────────────────────────────────────

std::ostream& operator<<(std::ostream& os, const ActaEquipo& ae) {
    if (ae.equipo != nullptr)
        os << ae.equipo->getPais();
    os << " | GF: "       << ae.golesAFavor
       << " | GC: "       << ae.golesEnContra
       << " | Posesion: " << ae.posesion << "%"
       << " | Convocados: " << ae.convocados.getTam();
    for (int i = 0; i < ae.convocados.getTam(); i++)
        os << "\n    " << ae.convocados[i];
    return os;
}
