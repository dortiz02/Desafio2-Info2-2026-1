#include "../include/EstadisticasJugador.h"

// ── Constructores ─────────────────────────────────────────────
EstadisticasJugador::EstadisticasJugador()
    : partidosJugados(0), goles(0), minutosJugados(0),
      asistencias(0), tarjetasAmarillas(0), tarjetasRojas(0), faltas(0) {}

EstadisticasJugador::EstadisticasJugador(const EstadisticasJugador& otra)
    : partidosJugados  (otra.partidosJugados),
      goles            (otra.goles),
      minutosJugados   (otra.minutosJugados),
      asistencias      (otra.asistencias),
      tarjetasAmarillas(otra.tarjetasAmarillas),
      tarjetasRojas    (otra.tarjetasRojas),
      faltas           (otra.faltas) {}

// ── Getters ───────────────────────────────────────────────────
int EstadisticasJugador::getPartidosJugados()   const { return partidosJugados;   }
int EstadisticasJugador::getGoles()             const { return goles;             }
int EstadisticasJugador::getMinutosJugados()    const { return minutosJugados;    }
int EstadisticasJugador::getAsistencias()       const { return asistencias;       }
int EstadisticasJugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int EstadisticasJugador::getTarjetasRojas()     const { return tarjetasRojas;     }
int EstadisticasJugador::getFaltas()            const { return faltas;            }

// ── Setters ───────────────────────────────────────────────────
void EstadisticasJugador::setPartidosJugados   (int v) { partidosJugados   = v; }
void EstadisticasJugador::setGoles             (int v) { goles             = v; }
void EstadisticasJugador::setMinutosJugados    (int v) { minutosJugados    = v; }
void EstadisticasJugador::setAsistencias       (int v) { asistencias       = v; }
void EstadisticasJugador::setTarjetasAmarillas (int v) { tarjetasAmarillas = v; }
void EstadisticasJugador::setTarjetasRojas     (int v) { tarjetasRojas     = v; }
void EstadisticasJugador::setFaltas            (int v) { faltas            = v; }

// ── Operadores ────────────────────────────────────────────────

// Acumula stats de un partido sobre el histórico
EstadisticasJugador& EstadisticasJugador::operator+=(const EstadisticasJugador& otra) {
    partidosJugados   += otra.partidosJugados;
    goles             += otra.goles;
    minutosJugados    += otra.minutosJugados;
    asistencias       += otra.asistencias;
    tarjetasAmarillas += otra.tarjetasAmarillas;
    tarjetasRojas     += otra.tarjetasRojas;
    faltas            += otra.faltas;
    return *this;
}

EstadisticasJugador& EstadisticasJugador::operator=(const EstadisticasJugador& otra) {
    if (this == &otra) return *this;
    partidosJugados   = otra.partidosJugados;
    goles             = otra.goles;
    minutosJugados    = otra.minutosJugados;
    asistencias       = otra.asistencias;
    tarjetasAmarillas = otra.tarjetasAmarillas;
    tarjetasRojas     = otra.tarjetasRojas;
    faltas            = otra.faltas;
    return *this;
}

// ── Persistencia ──────────────────────────────────────────────

// Guarda en formato binario para eficiencia
void EstadisticasJugador::guardar(std::fstream& archivo) const {
    archivo.write(reinterpret_cast<const char*>(&partidosJugados),   sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&goles),             sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&minutosJugados),    sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&asistencias),       sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&tarjetasAmarillas), sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&tarjetasRojas),     sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&faltas),            sizeof(int));
}

void EstadisticasJugador::cargar(std::fstream& archivo) {
    archivo.read(reinterpret_cast<char*>(&partidosJugados),   sizeof(int));
    archivo.read(reinterpret_cast<char*>(&goles),             sizeof(int));
    archivo.read(reinterpret_cast<char*>(&minutosJugados),    sizeof(int));
    archivo.read(reinterpret_cast<char*>(&asistencias),       sizeof(int));
    archivo.read(reinterpret_cast<char*>(&tarjetasAmarillas), sizeof(int));
    archivo.read(reinterpret_cast<char*>(&tarjetasRojas),     sizeof(int));
    archivo.read(reinterpret_cast<char*>(&faltas),            sizeof(int));
}
