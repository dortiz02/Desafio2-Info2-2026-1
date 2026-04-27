#include "../include/Grupo.h"
#include "../include/Simulador.h"
#include <iostream>

Grupo::Grupo() : letra(' ') { for (int i=0;i<4;i++) puntos[i]=0; }
Grupo::Grupo(char l) : letra(l) { for (int i=0;i<4;i++) puntos[i]=0; }
Grupo::Grupo(const Grupo& otro) : letra(otro.letra), equipos(otro.equipos), partidos(otro.partidos) {
    for (int i=0;i<4;i++) puntos[i]=otro.puntos[i];
}
Grupo::~Grupo() {}

char                  Grupo::getLetra()         const { return letra;    }
Lista<Equipo*>&       Grupo::getEquipos()              { return equipos;  }
Lista<Partido>&       Grupo::getPartidos()             { return partidos; }
const Lista<Partido>& Grupo::getPartidos()       const { return partidos; }
int                   Grupo::getPuntos(int i)    const { return (i>=0&&i<4)?puntos[i]:0; }

int Grupo::indiceEquipo(Equipo* e) const {
    for (int i=0; i<equipos.getTam(); i++) if (equipos[i]==e) return i;
    return -1;
}

void Grupo::agregarEquipo(Equipo* e) { if (equipos.getTam()<4) equipos.agregar(e); }

/// Genera los 6 partidos round-robin (todos contra todos una vez)
/// Combinaciones: (0,1),(0,2),(0,3),(1,2),(1,3),(2,3)
void Grupo::generarPartidos() {
    for (int i=0; i<equipos.getTam(); i++)
        for (int j=i+1; j<equipos.getTam(); j++) {
            Partido p(equipos[i], equipos[j], "", "nombreSede");
            partidos.agregar(p);
        }
}

/// Asigna fechas a los partidos respetando:
/// - Máx 4 partidos por día en todo el torneo (partidosPorDia compartido)
/// - Ningún equipo juega dos veces en menos de 3 días
void Grupo::asignarFechas(const std::string& fechaInicio,
                           int* partidosPorDia, int diasDisponibles) {
    int dia  = std::stoi(fechaInicio.substr(0,2));
    int mes  = std::stoi(fechaInicio.substr(3,2));
    int anio = std::stoi(fechaInicio.substr(6,4));
    int ultimoDia[4]; for (int i=0;i<4;i++) ultimoDia[i]=-99;

    for (int p=0; p<partidos.getTam(); p++) {
        Equipo* local     = partidos[p].getActaLocal().getEquipo();
        Equipo* visitante = partidos[p].getActaVisitante().getEquipo();
        int idxL = indiceEquipo(local);
        int idxV = indiceEquipo(visitante);
        bool asignado = false;
        for (int d=0; d<diasDisponibles && !asignado; d++) {
            if (partidosPorDia[d] >= 4) continue;
            if (d - ultimoDia[idxL] < 3) continue;
            if (d - ultimoDia[idxV] < 3) continue;
            int dF=dia+d, mF=mes, aF=anio;
            while (dF>30) { dF-=30; mF++; }
            if (mF>12)    { mF-=12; aF++; }
            std::string fecha = (dF<10?"0":"") + std::to_string(dF)
                              + "-" + (mF<10?"0":"") + std::to_string(mF)
                              + "-" + std::to_string(aF);
            partidos[p].setFecha(fecha);
            partidosPorDia[d]++;
            ultimoDia[idxL]=d; ultimoDia[idxV]=d;
            asignado=true;
        }
    }
}

/// Actualiza puntos internos: 3 por victoria, 1 por empate, 0 por derrota
void Grupo::actualizarPuntos(int idxL, int idxV, int gL, int gV) {
    if      (gL > gV) { puntos[idxL] += 3; }
    else if (gL==gV)  { puntos[idxL]++;  puntos[idxV]++; }
    else              { puntos[idxV] += 3; }
}

/// Simula todos los partidos y actualiza históricos tras cada uno
void Grupo::simularPartidos(Simulador& sim) {
    int combL[6]={0,0,0,1,1,2};
    int combV[6]={1,2,3,2,3,3};
    for (int p=0; p<partidos.getTam(); p++) {
        sim.simularPartido(partidos[p]);
        int gL = partidos[p].getActaLocal().getGolesAFavor();
        int gV = partidos[p].getActaVisitante().getGolesAFavor();
        actualizarPuntos(combL[p], combV[p], gL, gV);
        partidos[p].actualizarHistoricos();
    }
}

/// Ordena equipos por: puntos > diferencia de goles > goles a favor > sorteo
Lista<Equipo*> Grupo::getTablaClasificacion() const {
    int pts[4], gf[4], gc[4];
    for (int i=0;i<4;i++) { pts[i]=puntos[i]; gf[i]=0; gc[i]=0; }
    int combL[6]={0,0,0,1,1,2}, combV[6]={1,2,3,2,3,3};
    for (int p=0; p<partidos.getTam(); p++) {
        int gL=partidos[p].getActaLocal().getGolesAFavor();
        int gV=partidos[p].getActaVisitante().getGolesAFavor();
        gf[combL[p]]+=gL; gc[combL[p]]+=gV;
        gf[combV[p]]+=gV; gc[combV[p]]+=gL;
    }
    int orden[4]={0,1,2,3};
    for (int i=0;i<3;i++) for (int j=i+1;j<4;j++) {
        int a=orden[i], b=orden[j];
        bool swap=false;
        if      (pts[a]<pts[b]) swap=true;
        else if (pts[a]==pts[b] && (gf[a]-gc[a])<(gf[b]-gc[b])) swap=true;
        else if (pts[a]==pts[b] && (gf[a]-gc[a])==(gf[b]-gc[b]) && gf[a]<gf[b]) swap=true;
        if (swap) { int tmp=orden[i]; orden[i]=orden[j]; orden[j]=tmp; }
    }
    Lista<Equipo*> tabla;
    for (int i=0;i<4;i++) tabla.agregar(equipos[orden[i]]);
    return tabla;
}

void Grupo::imprimirTabla() const {
    Lista<Equipo*> tabla = getTablaClasificacion();
    std::cout << "\n=== GRUPO " << letra << " ===\n";
    std::cout << "Pos | Pais                | Pts\n";
    std::cout << "----|---------------------|----\n";
    for (int i=0; i<tabla.getTam(); i++) {
        int idx = indiceEquipo(tabla[i]);
        std::cout << " " << (i+1) << "  | " << tabla[i]->getPais() << " | " << puntos[idx] << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, const Grupo& g) {
    os << "Grupo " << g.letra << ":\n";
    for (int i=0; i<g.equipos.getTam(); i++) os << "  " << *g.equipos[i] << "\n";
    return os;
}
