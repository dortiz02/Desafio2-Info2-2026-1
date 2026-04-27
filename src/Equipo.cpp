#include "../include/Equipo.h"

// ── Constructores ─────────────────────────────────────────────

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
    // Tarjetas y faltas inician en 0 
}

// Constructor de copia
Equipo::Equipo(const Equipo& otro)
    : pais           (otro.pais),
      confederacion  (otro.confederacion),
      rankingFIFA    (otro.rankingFIFA),
      directorTecnico(otro.directorTecnico),
      statsHistoricas(otro.statsHistoricas),
      plantilla      (otro.plantilla) {}       // Lista<T> tiene su propio copy

Equipo::~Equipo() {}

// ── Getters ───────────────────────────────────────────────────

std::string Equipo::getPais()          const { return pais;            }
std::string Equipo::getConfederacion() const { return confederacion;   }
int         Equipo::getRankingFIFA()   const { return rankingFIFA;     }
std::string Equipo::getDT()            const { return directorTecnico; }

EstadisticasEquipo& Equipo::getStatsHistoricas() { return statsHistoricas; }
Lista<Jugador>&     Equipo::getPlantilla()        { return plantilla;       }

// Promedio de goles a favor por partido
double Equipo::getPromGF() const {
    int totalPartidos = statsHistoricas.getPartidosGanados()
                      + statsHistoricas.getPartidosEmpatados()
                      + statsHistoricas.getPartidosPerdidos();
    if (totalPartidos == 0) return 1.0;  // valor neutro si no hay historial
    return static_cast<double>(statsHistoricas.getGolesAFavor()) / totalPartidos;
}

// Promedio de goles en contra por partido
double Equipo::getPromGC() const {
    int totalPartidos = statsHistoricas.getPartidosGanados()
                      + statsHistoricas.getPartidosEmpatados()
                      + statsHistoricas.getPartidosPerdidos();
    if (totalPartidos == 0) return 1.0;
    return static_cast<double>(statsHistoricas.getGolesEnContra()) / totalPartidos;
}

// ── Setters ───────────────────────────────────────────────────

void Equipo::setPais          (const std::string& v) { pais           = v; }
void Equipo::setConfederacion (const std::string& v) { confederacion  = v; }
void Equipo::setRankingFIFA   (int v)                { rankingFIFA    = v; }
void Equipo::setDT            (const std::string& v) { directorTecnico = v; }

// ── Logica ────────────────────────────────────────────────────

// Genera plantilla artificial de 26 jugadores
// Camisetas 1..26, nombres "nombre1".."nombre26"
// Goles repartidos uniformemente entre los jugadores
void Equipo::generarPlantilla() {
    int golesTotal    = statsHistoricas.getGolesAFavor();
    int golesPorJug   = golesTotal / 26;
    int golesRestantes = golesTotal % 26;

    for (int i = 1; i <= 26; i++) {
        std::string nom = "nombre"   + std::to_string(i);
        std::string ape = "apellido" + std::to_string(i);
        Jugador j(nom, ape, i);

        // Los primeros golesRestantes jugadores reciben un gol extra
        int golesJugador = golesPorJug + (i <= golesRestantes ? 1 : 0);
        j.getStatsHistoricas().setGoles(golesJugador);

        plantilla.agregar(j);
    }
}

// Acumula las stats del partido sobre el histórico del equipo
void Equipo::actualizarStats(const EstadisticasEquipo& statsPartido) {
    statsHistoricas += statsPartido;
}

// ── Persistencia ──────────────────────────────────────────────

// Guarda la plantilla completa en binario
// Formato: [int numJugadores][Jugador0][Jugador1]...[JugadorN]
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

// ── Operadores ────────────────────────────────────────────────

// Ordena por ranking FIFA — menor número = mejor ranking
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
