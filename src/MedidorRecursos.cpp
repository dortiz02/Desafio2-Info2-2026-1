#include "../include/MedidorRecursos.h"
#include <iostream>

MedidorRecursos::MedidorRecursos() : iteraciones(0), memoriaBytes(0) {}

MedidorRecursos::MedidorRecursos(const MedidorRecursos& otro)
    : iteraciones(otro.iteraciones), memoriaBytes(otro.memoriaBytes) {}

MedidorRecursos::~MedidorRecursos() {}

long long MedidorRecursos::getIteraciones()  const { return iteraciones;  }
size_t    MedidorRecursos::getMemoriaBytes() const { return memoriaBytes; }

/// Incrementa el contador en n unidades
/// Se llama desde cualquier bucle o llamada del sistema
void MedidorRecursos::incrementar(long long n) {
    iteraciones += n;
}

/// Resetea contadores al inicio de cada funcionalidad
void MedidorRecursos::resetear() {
    iteraciones  = 0;
    memoriaBytes = 0;
}

/// Calcula el total de memoria de todas las estructuras activas
/// Suma el tamano de cada clase multiplicado por su cantidad
/// Incluye estimacion de strings y overhead de listas dinamicas
void MedidorRecursos::calcularMemoria(
    int numEquipos, int jugadoresPorEquipo,
    int numGrupos,  int partidosPorGrupo,
    int numEtapas,  int partidosPorEtapa)
{
    memoriaBytes = 0;

    // Jugadores: stats + campos basicos + strings estimados
    memoriaBytes += (sizeof(int) * 7          // EstadisticasJugador
                  +  sizeof(int) * 3          // numeroCamiseta + overhead
                  +  64)                      // nombre + apellido estimado
                  * jugadoresPorEquipo * numEquipos;

    // Equipos: stats + campos + strings
    memoriaBytes += (sizeof(int) * 8          // EstadisticasEquipo
                  +  sizeof(int)              // rankingFIFA
                  +  64 * 3)                  // pais + confederacion + dt estimados
                  * numEquipos;

    // Grupos: puntos
    memoriaBytes += sizeof(int) * 4 * numGrupos;

    // Partidos fase grupos
    size_t tamPartido = sizeof(bool) * 2
                      + 64 * 5               // strings fecha/hora/sede/arbitros
                      + sizeof(ActaEquipo) * 2
                      + sizeof(ActaJugador) * 11 * 2;
    memoriaBytes += tamPartido * partidosPorGrupo * numGrupos;

    // Partidos etapas eliminatorias
    memoriaBytes += tamPartido * partidosPorEtapa * numEtapas;

    // Overhead de listas dinamicas (puntero datos + tam + cap)
    memoriaBytes += sizeof(void*) * 3
                  * (numEquipos + numGrupos + numEtapas);
}

/// Imprime el reporte de la funcionalidad recien ejecutada
void MedidorRecursos::reportar(const std::string& nombreFuncionalidad) const {
    std::cout << "\n--- Reporte de recursos: " << nombreFuncionalidad << " ---\n";
    std::cout << "Iteraciones:   " << iteraciones  << "\n";
    std::cout << "Memoria total: " << memoriaBytes << " bytes ("
              << memoriaBytes / 1024.0 << " KB)\n";
    std::cout << "---------------------------------------------\n";
}

/// Suma dos medidores para acumular reportes parciales
MedidorRecursos MedidorRecursos::operator+(const MedidorRecursos& otro) const {
    MedidorRecursos r;
    r.iteraciones  = iteraciones  + otro.iteraciones;
    r.memoriaBytes = memoriaBytes + otro.memoriaBytes;
    return r;
}

std::ostream& operator<<(std::ostream& os, const MedidorRecursos& m) {
    os << "Iteraciones: " << m.iteraciones
       << " | Memoria: "  << m.memoriaBytes << " bytes";
    return os;
}
