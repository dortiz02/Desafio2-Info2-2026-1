#include "../include/ActaJugador.h"

// ── Constructores ─────────────────────────────────────────────

ActaJugador::ActaJugador()
    : jugador(nullptr), goles(0), tarjetasAmarillas(0),
      tarjetasRojas(0), faltas(0), minutosJugados(0) {}

ActaJugador::ActaJugador(Jugador* j)
    : jugador(j), goles(0), tarjetasAmarillas(0),
      tarjetasRojas(0), faltas(0), minutosJugados(90) {}

// Copia el puntero — no duplica el Jugado
ActaJugador::ActaJugador(const ActaJugador& otra)
    : jugador         (otra.jugador),
      goles           (otra.goles),
      tarjetasAmarillas(otra.tarjetasAmarillas),
      tarjetasRojas   (otra.tarjetasRojas),
      faltas          (otra.faltas),
      minutosJugados  (otra.minutosJugados) {}

ActaJugador::~ActaJugador() {}

// ── Getters ───────────────────────────────────────────────────

Jugador* ActaJugador::getJugador()           const { return jugador;          }
int      ActaJugador::getGoles()             const { return goles;            }
int      ActaJugador::getTarjetasAmarillas() const { return tarjetasAmarillas;}
int      ActaJugador::getTarjetasRojas()     const { return tarjetasRojas;    }
int      ActaJugador::getFaltas()            const { return faltas;           }
int      ActaJugador::getMinutosJugados()    const { return minutosJugados;   }

// ── Setters ───────────────────────────────────────────────────

void ActaJugador::setGoles            (int v) { goles            = v; }
void ActaJugador::setTarjetasAmarillas(int v) { tarjetasAmarillas = v; }
void ActaJugador::setTarjetasRojas    (int v) { tarjetasRojas    = v; }
void ActaJugador::setFaltas           (int v) { faltas           = v; }
void ActaJugador::setMinutosJugados   (int v) { minutosJugados   = v; }

// ── Logica ────────────────────────────────────────────────────

// Empaqueta los datos del acta en un objeto de estadsticas
// para poder acumular sobre el histórico con operator+=
EstadisticasJugador ActaJugador::toStats() const {
    EstadisticasJugador s;
    s.setPartidosJugados  (1);
    s.setGoles            (goles);
    s.setMinutosJugados   (minutosJugados);
    s.setAsistencias      (0);   // asistencias en 0 según enunciado
    s.setTarjetasAmarillas(tarjetasAmarillas);
    s.setTarjetasRojas    (tarjetasRojas);
    s.setFaltas           (faltas);
    return s;
}

// ── Operadores ────────────────────────────────────────────────

std::ostream& operator<<(std::ostream& os, const ActaJugador& aj) {
    if (aj.jugador != nullptr)
        os << "Camiseta #" << aj.jugador->getNumeroCamiseta()
           << " | Goles: "      << aj.goles
           << " | Amarillas: "  << aj.tarjetasAmarillas
           << " | Rojas: "      << aj.tarjetasRojas
           << " | Faltas: "     << aj.faltas
           << " | Minutos: "    << aj.minutosJugados;
    return os;
}
