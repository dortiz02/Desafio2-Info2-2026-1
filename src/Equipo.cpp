#include "../include/Equipo.h"

Equipo::Equipo()
    : pais(""), confederacion(""), rankingFIFA(0), directorTecnico("") {}

Equipo::Equipo(const std::string& p, const std::string& conf,
               int ranking, const std::string& dt,
               int golesAF, int golesEC,
               int ganados, int empatados, int perdidos)
    : pais(p), confederacion(conf), rankingFIFA(ranking), directorTecnico(dt) {
    statsHistoricas.setGolesAFavor      (golesAF);
    statsHistoricas.setGolesEnContra    (golesEC);
    statsHistoricas.setPartidosGanados  (ganados);
    statsHistoricas.setPartidosEmpatados(empatados);
    statsHistoricas.setPartidosPerdidos (perdidos);
}

Equipo::Equipo(const Equipo& otro)
    : pais           (otro.pais),
      confederacion  (otro.confederacion),
      rankingFIFA    (otro.rankingFIFA),
      directorTecnico(otro.directorTecnico),
      statsHistoricas(otro.statsHistoricas),
      plantilla      (otro.plantilla) {}

Equipo::~Equipo() {}

std::string Equipo::getPais()          const { return pais;            }
std::string Equipo::getConfederacion() const { return confederacion;   }
int         Equipo::getRankingFIFA()   const { return rankingFIFA;     }
std::string Equipo::getDT()            const { return directorTecnico; }

EstadisticasEquipo&       Equipo::getStatsHistoricas()       { return statsHistoricas; }
const EstadisticasEquipo& Equipo::getStatsHistoricas() const { return statsHistoricas; }
Lista<Jugador>&     Equipo::getPlantilla()        { return plantilla;       }

double Equipo::getPromGF() const {
    int total = statsHistoricas.getPartidosGanados()
              + statsHistoricas.getPartidosEmpatados()
              + statsHistoricas.getPartidosPerdidos();
    if (total == 0) return 1.0;
    return static_cast<double>(statsHistoricas.getGolesAFavor()) / total;
}

double Equipo::getPromGC() const {
    int total = statsHistoricas.getPartidosGanados()
              + statsHistoricas.getPartidosEmpatados()
              + statsHistoricas.getPartidosPerdidos();
    if (total == 0) return 1.0;
    return static_cast<double>(statsHistoricas.getGolesEnContra()) / total;
}

void Equipo::setPais          (const std::string& v) { pais            = v; }
void Equipo::setConfederacion (const std::string& v) { confederacion   = v; }
void Equipo::setRankingFIFA   (int v)                { rankingFIFA     = v; }
void Equipo::setDT            (const std::string& v) { directorTecnico = v; }

void Equipo::generarPlantilla() {
    int golesTotal    = statsHistoricas.getGolesAFavor();
    int golesPorJug   = golesTotal / 26;
    int golesRestantes = golesTotal % 26;

    for (int i = 1; i <= 26; i++) {
        std::string nom = "nombre"   + std::to_string(i);
        std::string ape = "apellido" + std::to_string(i);
        Jugador j(nom, ape, i);
        int golesJugador = golesPorJug + (i <= golesRestantes ? 1 : 0);
        j.getStatsHistoricas().setGoles(golesJugador);
        plantilla.agregar(j);
    }
}

void Equipo::actualizarStats(const EstadisticasEquipo& statsPartido) {
    statsHistoricas += statsPartido;
}

void Equipo::guardarPlantilla(std::fstream& archivo) const {
    int n = plantilla.getTam();
    archivo.write(reinterpret_cast<const char*>(&n), sizeof(int));
    for (int i = 0; i < n; i++)
        plantilla[i].guardar(archivo);
}

void Equipo::cargarPlantilla(std::fstream& archivo) {
    int n = 0;
    archivo.read(reinterpret_cast<char*>(&n), sizeof(int));
    for (int i = 0; i < n; i++) {
        Jugador j;
        j.cargar(archivo);
        plantilla.agregar(j);
    }
}

bool Equipo::operator<(const Equipo& otro) const {
    return rankingFIFA < otro.rankingFIFA;
}

std::ostream& operator<<(std::ostream& os, const Equipo& e) {
    os << e.pais
       << " | " << e.confederacion
       << " | Ranking: "  << e.rankingFIFA
       << " | DT: "       << e.directorTecnico
       << " | GF: "       << e.statsHistoricas.getGolesAFavor()
       << " | GC: "       << e.statsHistoricas.getGolesEnContra()
       << " | PG: "       << e.statsHistoricas.getPartidosGanados()
       << " | PE: "       << e.statsHistoricas.getPartidosEmpatados()
       << " | PP: "       << e.statsHistoricas.getPartidosPerdidos();
    return os;
}
