#include "../include/Jugador.h"

// ── Constructores ─────────────────────────────────────────────

Jugador::Jugador()
    : nombre(""), apellido(""), numeroCamiseta(0) {}

Jugador::Jugador(const std::string& nom, const std::string& ape, int numCamiseta)
    : nombre(nom), apellido(ape), numeroCamiseta(numCamiseta) {}

// Constructor de copia — requerido por enunciado
// aunque Lista<T> lo invoca internamente al redimensionar
Jugador::Jugador(const Jugador& otro)
    : nombre         (otro.nombre),
      apellido       (otro.apellido),
      numeroCamiseta (otro.numeroCamiseta),
      statsHistoricas(otro.statsHistoricas) {}

Jugador::~Jugador() {}

// ── Getters ───────────────────────────────────────────────────

std::string Jugador::getNombre()         const { return nombre;         }
std::string Jugador::getApellido()       const { return apellido;       }
int         Jugador::getNumeroCamiseta() const { return numeroCamiseta; }

EstadisticasJugador& Jugador::getStatsHistoricas() {
    return statsHistoricas;
}

// ── Setters ───────────────────────────────────────────────────

void Jugador::setNombre         (const std::string& v) { nombre         = v; }
void Jugador::setApellido       (const std::string& v) { apellido       = v; }
void Jugador::setNumeroCamiseta (int v)                { numeroCamiseta = v; }

// ── Lógica ────────────────────────────────────────────────────

// Acumula las estadísticas de un partido sobre el histórico
// Se llama solo cuando el partido termina completamente
void Jugador::actualizarStats(const EstadisticasJugador& statsPartido) {
    statsHistoricas += statsPartido;
}

// ── Persistencia ──────────────────────────────────────────────

// Guarda en binario: primero longitud del string, luego los chars
// Formato propio — documentado en el informe
void Jugador::guardar(std::fstream& archivo) const {
    // Guardar nombre
    int lenNombre = nombre.size();
    archivo.write(reinterpret_cast<const char*>(&lenNombre), sizeof(int));
    archivo.write(nombre.c_str(), lenNombre);

    // Guardar apellido
    int lenApellido = apellido.size();
    archivo.write(reinterpret_cast<const char*>(&lenApellido), sizeof(int));
    archivo.write(apellido.c_str(), lenApellido);

    // Guardar número de camiseta
    archivo.write(reinterpret_cast<const char*>(&numeroCamiseta), sizeof(int));

    // Guardar estadísticas históricas
    statsHistoricas.guardar(archivo);
}

void Jugador::cargar(std::fstream& archivo) {
    // Cargar nombre
    int lenNombre = 0;
    archivo.read(reinterpret_cast<char*>(&lenNombre), sizeof(int));
    nombre.resize(lenNombre);
    archivo.read(&nombre[0], lenNombre);

    // Cargar apellido
    int lenApellido = 0;
    archivo.read(reinterpret_cast<char*>(&lenApellido), sizeof(int));
    apellido.resize(lenApellido);
    archivo.read(&apellido[0], lenApellido);

    // Cargar número de camiseta
    archivo.read(reinterpret_cast<char*>(&numeroCamiseta), sizeof(int));

    // Cargar estadísticas históricas
    statsHistoricas.cargar(archivo);
}

// ── Operadores ────────────────────────────────────────────────

// Dos jugadores son iguales si tienen el mismo número de camiseta
bool Jugador::operator==(const Jugador& otro) const {
    return numeroCamiseta == otro.numeroCamiseta;
}

std::ostream& operator<<(std::ostream& os, const Jugador& j) {
    os << "Camiseta #" << j.numeroCamiseta
       << " | " << j.nombre << " " << j.apellido
       << " | Goles: "    << j.statsHistoricas.getGoles()
       << " | Partidos: " << j.statsHistoricas.getPartidosJugados()
       << " | Minutos: "  << j.statsHistoricas.getMinutosJugados()
       << " | Amarillas: "<< j.statsHistoricas.getTarjetasAmarillas()
       << " | Rojas: "    << j.statsHistoricas.getTarjetasRojas()
       << " | Faltas: "   << j.statsHistoricas.getFaltas();
    return os;
}
