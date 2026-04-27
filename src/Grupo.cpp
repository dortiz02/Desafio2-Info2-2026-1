#include "../include/Grupo.h"
#include "../include/Simulador.h"
#include <iostream>

// ── Constructores ─────────────────────────────────────────────

Grupo::Grupo() : letra(' ') {
    for (int i = 0; i < 4; i++) puntos[i] = 0;
}

Grupo::Grupo(char l) : letra(l) {
    for (int i = 0; i < 4; i++) puntos[i] = 0;
}

// Constructor de copia — copia punteros (no duplica equipos)
Grupo::Grupo(const Grupo& otro)
    : letra  (otro.letra),
      equipos (otro.equipos),
      partidos(otro.partidos) {
    for (int i = 0; i < 4; i++) puntos[i] = otro.puntos[i];
}

Grupo::~Grupo() {}

// ── Getters ───────────────────────────────────────────────────

char            Grupo::getLetra()       const { return letra;   }
Lista<Equipo*>& Grupo::getEquipos()           { return equipos; }
Lista<Partido>& Grupo::getPartidos()          { return partidos;}
int             Grupo::getPuntos(int i) const { return (i>=0&&i<4) ? puntos[i] : 0; }

// ── Lógica interna ────────────────────────────────────────────

/// Busca el indice de un equipo dentro de la lista interna
/// Devuelve -1 si no se encuentra
int Grupo::indiceEquipo(Equipo* e) const {
    for (int i = 0; i < equipos.getTam(); i++)
        if (equipos[i] == e) return i;
    return -1;
}

// ── logica publica ────────────────────────────────────────────

/// Agrega un equipo al grupo
/// No agrega si ya hay 4 equipos
void Grupo::agregarEquipo(Equipo* e) {
    if (equipos.getTam() < 4) equipos.agregar(e);
}

/// Genera los 6 partidos round-robin (todos contra todos una vez)
/// Combinaciones: (0,1),(0,2),(0,3),(1,2),(1,3),(2,3)
/// Debe llamarse solo cuando los 4 equipos ya están asignados
void Grupo::generarPartidos() {
    for (int i = 0; i < equipos.getTam(); i++) {
        for (int j = i + 1; j < equipos.getTam(); j++) {
            Partido p(equipos[i], equipos[j], "", "nombreSede");
            partidos.agregar(p);
        }
    }
}

/// Asigna fechas a los 6 partidos del grupo
/// partidosPorDia: arreglo compartido con todos los grupos para controlar el limite de 4/dia
/// diasDisponibles: 19 dias desde la fecha de inicio
/// Formato fecha: "DD-MM-YYYY"
void Grupo::asignarFechas(const std::string& fechaInicio,
                           int* partidosPorDia, int diasDisponibles) {
    // Extraer dia, mes, año de la fecha de inicio
    int dia  = std::stoi(fechaInicio.substr(0, 2));
    int mes  = std::stoi(fechaInicio.substr(3, 2));
    int anio = std::stoi(fechaInicio.substr(6, 4));

    // Ultimo dia jugado por cada equipo (índice de día 0..18), -99 = nunca
    int ultimoDia[4];
    for (int i = 0; i < 4; i++) ultimoDia[i] = -99;

    for (int p = 0; p < partidos.getTam(); p++) {
        Equipo* local     = partidos[p].getActaLocal().getEquipo();
        Equipo* visitante = partidos[p].getActaVisitante().getEquipo();
        int idxL = indiceEquipo(local);
        int idxV = indiceEquipo(visitante);

        // Buscar el primer dia valido
        bool asignado = false;
        for (int d = 0; d < diasDisponibles && !asignado; d++) {
            // Restricción 1: max 4 partidos por dia en todo el torneo
            if (partidosPorDia[d] >= 4) continue;

            // Restriccion 2: ningun equipo juega en menos de 3 dias
            if (d - ultimoDia[idxL] < 3) continue;
            if (d - ultimoDia[idxV] < 3) continue;

            // Asignar fecha
            int diaFecha = dia + d;
            // Ajuste simple de mes (asume meses de 30 dias para el rango del torneo)
            int mesFecha  = mes;
            int anioFecha = anio;
            while (diaFecha > 30) { diaFecha -= 30; mesFecha++; }
            if (mesFecha > 12)    { mesFecha -= 12; anioFecha++; }

            // Formatear fecha "DD-MM-YYYY"
            std::string fecha = (diaFecha < 10 ? "0" : "") + std::to_string(diaFecha)
                              + "-" + (mesFecha < 10 ? "0" : "") + std::to_string(mesFecha)
                              + "-" + std::to_string(anioFecha);

            partidos[p].setFecha(fecha);
            partidosPorDia[d]++;
            ultimoDia[idxL] = d;
            ultimoDia[idxV] = d;
            asignado = true;
        }
    }
}

/// Actualiza los puntos internos del grupo tras un partido
/// 3 puntos por victoria, 1 por empate, 0 por derrota
void Grupo::actualizarPuntos(int idxLocal, int idxVisitante,
                              int golesL, int golesV) {
    if (golesL > golesV)       { puntos[idxLocal]    += 3; }
    else if (golesL == golesV) { puntos[idxLocal]++;  puntos[idxVisitante]++; }
    else                       { puntos[idxVisitante] += 3; }
}

/// Simula todos los partidos del grupo en orden
/// Actualiza puntos internos y históricos tras cada partido
void Grupo::simularPartidos(Simulador& sim) {
    // Combinaciones de indices correspondientes a los 6 partidos generados
    int combLocal[6]     = {0, 0, 0, 1, 1, 2};
    int combVisitante[6] = {1, 2, 3, 2, 3, 3};

    for (int p = 0; p < partidos.getTam(); p++) {
        sim.simularPartido(partidos[p]);
        int gL = partidos[p].getActaLocal().getGolesAFavor();
        int gV = partidos[p].getActaVisitante().getGolesAFavor();
        actualizarPuntos(combLocal[p], combVisitante[p], gL, gV);
        partidos[p].actualizarHistoricos();
    }
}

/// Devuelve los equipos ordenados por:
/// 1. Puntos (desc)
/// 2. Diferencia de goles en partidos del grupo (desc)
/// 3. Goles a favor en el grupo (desc)
/// 4. Sorteo (aleatorio como último criterio)
Lista<Equipo*> Grupo::getTablaClasificacion() const {
    // Armar arreglos auxiliares con puntos y stats del grupo
    int pts[4], gf[4], gc[4];
    for (int i = 0; i < 4; i++) {
        pts[i] = puntos[i];
        gf[i]  = 0;
        gc[i]  = 0;
    }

    // Acumular goles del grupo por equipo
    int combL[6] = {0,0,0,1,1,2};
    int combV[6] = {1,2,3,2,3,3};
    for (int p = 0; p < partidos.getTam(); p++) {
        int gL = partidos[p].getActaLocal().getGolesAFavor();
        int gV = partidos[p].getActaVisitante().getGolesAFavor();
        gf[combL[p]] += gL; gc[combL[p]] += gV;
        gf[combV[p]] += gV; gc[combV[p]] += gL;
    }

    // Ordenar indices por criterios (burbuja simple — solo 4 elementos)
    int orden[4] = {0, 1, 2, 3};
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            int a = orden[i], b = orden[j];
            bool intercambiar = false;
            if      (pts[a] < pts[b])                                intercambiar = true;
            else if (pts[a] == pts[b] && (gf[a]-gc[a]) < (gf[b]-gc[b])) intercambiar = true;
            else if (pts[a] == pts[b] && (gf[a]-gc[a]) == (gf[b]-gc[b])
                     && gf[a] < gf[b])                               intercambiar = true;
            if (intercambiar) { int tmp=orden[i]; orden[i]=orden[j]; orden[j]=tmp; }
        }
    }

    Lista<Equipo*> tabla;
    for (int i = 0; i < 4; i++) tabla.agregar(equipos[orden[i]]);
    return tabla;
}

/// Imprime la tabla de clasificacion del grupo en consola
void Grupo::imprimirTabla() const {
    Lista<Equipo*> tabla = getTablaClasificacion();
    std::cout << "\n=== GRUPO " << letra << " ===\n";
    std::cout << "Pos | Pais                | Pts\n";
    std::cout << "----|---------------------|----\n";
    for (int i = 0; i < tabla.getTam(); i++) {
        int idx = indiceEquipo(tabla[i]);
        std::cout << " " << (i+1) << "  | "
                  << tabla[i]->getPais()
                  << " | " << puntos[idx] << "\n";
    }
}

// ── Operadores ────────────────────────────────────────────────

std::ostream& operator<<(std::ostream& os, const Grupo& g) {
    os << "Grupo " << g.letra << ":\n";
    for (int i = 0; i < g.equipos.getTam(); i++)
        os << "  " << *g.equipos[i] << "\n";
    return os;
}
