#include "../include/Jugador.h"

Jugador::Jugador()
    : nombre(""), apellido(""), numeroCamiseta(0) {}

Jugador::Jugador(const std::string& nom, const std::string& ape, int numCamiseta)
    : nombre(nom), apellido(ape), numeroCamiseta(numCamiseta) {}

Jugador::Jugador(const Jugador& otro)
    : nombre         (otro.nombre),
      apellido       (otro.apellido),
      numeroCamiseta (otro.numeroCamiseta),
      statsHistoricas(otro.statsHistoricas) {}

Jugador::~Jugador() {}

// Operador de asignacion — necesario para usar Jugador en Lista<T>
Jugador& Jugador::operator=(const Jugador& otro) {
    if (this == &otro) return *this;
    nombre          = otro.nombre;
    apellido        = otro.apellido;
    numeroCamiseta  = otro.numeroCamiseta;
    statsHistoricas = otro.statsHistoricas;
    return *this;
}

std::string Jugador::getNombre()         const { return nombre;         }
std::string Jugador::getApellido()       const { return apellido;       }
int         Jugador::getNumeroCamiseta() const { return numeroCamiseta; }

EstadisticasJugador& Jugador::getStatsHistoricas() {
    return statsHistoricas;
}

void Jugador::setNombre         (const std::string& v) { nombre         = v; }
void Jugador::setApellido       (const std::string& v) { apellido       = v; }
void Jugador::setNumeroCamiseta (int v)                { numeroCamiseta = v; }

void Jugador::actualizarStats(const EstadisticasJugador& statsPartido) {
    statsHistoricas += statsPartido;
}

void Jugador::guardar(std::fstream& archivo) const {
    int lenNombre = nombre.size();
    archivo.write(reinterpret_cast<const char*>(&lenNombre), sizeof(int));
    archivo.write(nombre.c_str(), lenNombre);

    int lenApellido = apellido.size();
    archivo.write(reinterpret_cast<const char*>(&lenApellido), sizeof(int));
    archivo.write(apellido.c_str(), lenApellido);

    archivo.write(reinterpret_cast<const char*>(&numeroCamiseta), sizeof(int));
    statsHistoricas.guardar(archivo);
}

void Jugador::cargar(std::fstream& archivo) {
    int lenNombre = 0;
    archivo.read(reinterpret_cast<char*>(&lenNombre), sizeof(int));
    nombre.resize(lenNombre);
    archivo.read(&nombre[0], lenNombre);

    int lenApellido = 0;
    archivo.read(reinterpret_cast<char*>(&lenApellido), sizeof(int));
    apellido.resize(lenApellido);
    archivo.read(&apellido[0], lenApellido);

    archivo.read(reinterpret_cast<char*>(&numeroCamiseta), sizeof(int));
    statsHistoricas.cargar(archivo);
}

bool Jugador::operator==(const Jugador& otro) const {
    return numeroCamiseta == otro.numeroCamiseta;
}

std::ostream& operator<<(std::ostream& os, const Jugador& j) {
    os << "Camiseta #" << j.numeroCamiseta
       << " | " << j.nombre << " " << j.apellido
       << " | Goles: "     << j.statsHistoricas.getGoles()
       << " | Partidos: "  << j.statsHistoricas.getPartidosJugados()
       << " | Minutos: "   << j.statsHistoricas.getMinutosJugados()
       << " | Amarillas: " << j.statsHistoricas.getTarjetasAmarillas()
       << " | Rojas: "     << j.statsHistoricas.getTarjetasRojas()
       << " | Faltas: "    << j.statsHistoricas.getFaltas();
    return os;
}
