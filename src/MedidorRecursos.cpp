#include "../include/MedidorRecursos.h"
#include <iostream>

// -- Constructores ----------------------------------------------------

MedidorRecursos::MedidorRecursos()
    : iteraciones(0), memoriaBytes(0) {}

MedidorRecursos::MedidorRecursos(const MedidorRecursos& otro)
    : iteraciones (otro.iteraciones),
      memoriaBytes(otro.memoriaBytes) {}

MedidorRecursos::~MedidorRecursos() {}

// -- Getters ----------------------------------------------------------

long long MedidorRecursos::getIteraciones()  const { return iteraciones;  }
size_t    MedidorRecursos::getMemoriaBytes() const { return memoriaBytes; }

// -- Logica -----------------------------------------------------------

/// Incrementa el contador de iteraciones en n unidades
/// Se llama desde cualquier bucle o llamada recursiva del sistema
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
/// Incluye objetos en heap y estimacion de variables locales relevantes
void MedidorRecursos::calcularMemoria(
    int numEquipos,
    int jugadoresPorEquipo,
    int numGrupos,
    int partidosPorGrupo,
    int numEtapas,
    int partidosPorEtapa)
{
    memoriaBytes = 0;

    // Equipos y sus estadisticas
    memoriaBytes += sizeof(double) * 4;             // EstadisticasEquipo (8 ints)
    memoriaBytes += (sizeof(double) * 8             // EstadisticasEquipo
                  +  sizeof(double) * 7             // EstadisticasJugador
                  +  sizeof(int)    * 3             // campos basicos Jugador
                  +  64)                            // strings nombre/apellido estimado
                  * jugadoresPorEquipo * numEquipos;

    // Equipos
    memoriaBytes += (sizeof(int) + 64 * 3)          // rankingFIFA + strings pais/conf/dt
                  * numEquipos;

    // Grupos
    memoriaBytes += sizeof(int) * 4                 // puntos[4]
                  * numGrupos;

    // Partidos de fase de grupos
    memoriaBytes += (sizeof(bool) * 2               // flags partido
                  +  64 * 5                         // strings fecha/hora/sede/arbitros
                  +  sizeof(ActaEquipo) * 2         // dos actas por partido
                  +  sizeof(ActaJugador) * 11 * 2)  // 11 jugadores por acta
                  * partidosPorGrupo * numGrupos;

    // Partidos de etapas eliminatorias
    memoriaBytes += (sizeof(bool) * 2
                  +  64 * 5
                  +  sizeof(ActaEquipo) * 2
                  +  sizeof(ActaJugador) * 11 * 2)
                  * partidosPorEtapa * numEtapas;

    // Listas dinamicas (overhead de puntero + tam + cap)
    memoriaBytes += sizeof(void*) * 3
                  * (numEquipos + numGrupos + numEtapas);
}

/// Imprime el reporte de la funcionalidad recien ejecutada
void MedidorRecursos::reportar(const std::string& nombreFuncionalidad) const {
    std::cout << "\n--- Reporte de recursos: " << nombreFuncionalidad << " ---\n";
    std::cout << "Iteraciones:    " << iteraciones  << "\n";
    std::cout << "Memoria total:  " << memoriaBytes << " bytes ("
              << memoriaBytes / 1024.0 << " KB)\n";
    std::cout << "---------------------------------------------\n";
}

/// Suma dos medidores para acumular reportes parciales
MedidorRecursos MedidorRecursos::operator+(const MedidorRecursos& otro) const {
    MedidorRecursos resultado;
    resultado.iteraciones  = iteraciones  + otro.iteraciones;
    resultado.memoriaBytes = memoriaBytes + otro.memoriaBytes;
    return resultado;
}

std::ostream& operator<<(std::ostream& os, const MedidorRecursos& m) {
    os << "Iteraciones: " << m.iteraciones
       << " | Memoria: " << m.memoriaBytes << " bytes";
    return os;
}
