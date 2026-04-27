#include "../include/EstadisticasEquipo.h"

// ── Constructores ─────────────────────────────────────────────
EstadisticasEquipo::EstadisticasEquipo()
    : golesAFavor(0), golesEnContra(0), partidosGanados(0),
      partidosEmpatados(0), partidosPerdidos(0),
      tarjetasAmarillas(0), tarjetasRojas(0), faltas(0) {}

EstadisticasEquipo::EstadisticasEquipo(const EstadisticasEquipo& otra)
    : golesAFavor      (otra.golesAFavor),
      golesEnContra    (otra.golesEnContra),
      partidosGanados  (otra.partidosGanados),
      partidosEmpatados(otra.partidosEmpatados),
      partidosPerdidos (otra.partidosPerdidos),
      tarjetasAmarillas(otra.tarjetasAmarillas),
      tarjetasRojas    (otra.tarjetasRojas),
      faltas           (otra.faltas) {}

// ── Getters ───────────────────────────────────────────────────
int EstadisticasEquipo::getGolesAFavor()       const { return golesAFavor;       }
int EstadisticasEquipo::getGolesEnContra()     const { return golesEnContra;     }
int EstadisticasEquipo::getPartidosGanados()   const { return partidosGanados;   }
int EstadisticasEquipo::getPartidosEmpatados() const { return partidosEmpatados; }
int EstadisticasEquipo::getPartidosPerdidos()  const { return partidosPerdidos;  }
int EstadisticasEquipo::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int EstadisticasEquipo::getTarjetasRojas()     const { return tarjetasRojas;     }
int EstadisticasEquipo::getFaltas()            const { return faltas;            }

// ── Setters ───────────────────────────────────────────────────
void EstadisticasEquipo::setGolesAFavor       (int v) { golesAFavor       = v; }
void EstadisticasEquipo::setGolesEnContra     (int v) { golesEnContra     = v; }
void EstadisticasEquipo::setPartidosGanados   (int v) { partidosGanados   = v; }
void EstadisticasEquipo::setPartidosEmpatados (int v) { partidosEmpatados = v; }
void EstadisticasEquipo::setPartidosPerdidos  (int v) { partidosPerdidos  = v; }
void EstadisticasEquipo::setTarjetasAmarillas (int v) { tarjetasAmarillas = v; }
void EstadisticasEquipo::setTarjetasRojas     (int v) { tarjetasRojas     = v; }
void EstadisticasEquipo::setFaltas            (int v) { faltas            = v; }

// ── Operadores ────────────────────────────────────────────────

// Acumula stats de un partido sobre el histórico del equipo
EstadisticasEquipo& EstadisticasEquipo::operator+=(const EstadisticasEquipo& otra) {
    golesAFavor       += otra.golesAFavor;
    golesEnContra     += otra.golesEnContra;
    partidosGanados   += otra.partidosGanados;
    partidosEmpatados += otra.partidosEmpatados;
    partidosPerdidos  += otra.partidosPerdidos;
    tarjetasAmarillas += otra.tarjetasAmarillas;
    tarjetasRojas     += otra.tarjetasRojas;
    faltas            += otra.faltas;
    return *this;
}

EstadisticasEquipo& EstadisticasEquipo::operator=(const EstadisticasEquipo& otra) {
    if (this == &otra) return *this;
    golesAFavor       = otra.golesAFavor;
    golesEnContra     = otra.golesEnContra;
    partidosGanados   = otra.partidosGanados;
    partidosEmpatados = otra.partidosEmpatados;
    partidosPerdidos  = otra.partidosPerdidos;
    tarjetasAmarillas = otra.tarjetasAmarillas;
    tarjetasRojas     = otra.tarjetasRojas;
    faltas            = otra.faltas;
    return *this;
}

// ── Persistencia ──────────────────────────────────────────────
void EstadisticasEquipo::guardar(std::fstream& archivo) const {
    archivo.write(reinterpret_cast<const char*>(&golesAFavor),       sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&golesEnContra),     sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&partidosGanados),   sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&partidosEmpatados), sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&partidosPerdidos),  sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&tarjetasAmarillas), sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&tarjetasRojas),     sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&faltas),            sizeof(int));
}

void EstadisticasEquipo::cargar(std::fstream& archivo) {
    archivo.read(reinterpret_cast<char*>(&golesAFavor),       sizeof(int));
    archivo.read(reinterpret_cast<char*>(&golesEnContra),     sizeof(int));
    archivo.read(reinterpret_cast<char*>(&partidosGanados),   sizeof(int));
    archivo.read(reinterpret_cast<char*>(&partidosEmpatados), sizeof(int));
    archivo.read(reinterpret_cast<char*>(&partidosPerdidos),  sizeof(int));
    archivo.read(reinterpret_cast<char*>(&tarjetasAmarillas), sizeof(int));
    archivo.read(reinterpret_cast<char*>(&tarjetasRojas),     sizeof(int));
    archivo.read(reinterpret_cast<char*>(&faltas),            sizeof(int));
}
