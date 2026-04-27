#include "../include/Torneo.h"
#include <fstream>
#include <iostream>

const std::string Torneo::FECHA_ELIMINATORIAS = "10-07-2026";

Torneo::Torneo() {}

Torneo::Torneo(const Torneo& otro)
    : grupos(otro.grupos), etapas(otro.etapas),
      sorteo(otro.sorteo), simulador(otro.simulador), medidor(otro.medidor) {
    for (int i=0; i<otro.equipos.getTam(); i++)
        equipos.agregar(new Equipo(*otro.equipos[i]));
}

Torneo::~Torneo() {
    for (int i=0; i<equipos.getTam(); i++) { delete equipos[i]; equipos[i]=nullptr; }
}

// -- Helpers privados -------------------------------------------------

/// Parsea una linea con separador ';' en campos individuales
/// Elimina caracteres '\r' al final de la linea (formato Windows)
void Torneo::parsearLineaCSV(const std::string& lineaRaw,
                              std::string campos[], int maxCampos) const {
    // Limpiar \r al final si existe
    std::string linea = lineaRaw;
    if (!linea.empty() && linea.back() == '\r') linea.pop_back();

    int idx=0, ini=0;
    for (int i=0; i<=(int)linea.size() && idx<maxCampos; i++) {
        if (i==(int)linea.size() || linea[i]==';') {
            campos[idx++]=linea.substr(ini, i-ini);
            ini=i+1;
        }
    }
}

/// Normaliza el nombre de la confederacion al formato interno
/// Convierte "Concacaf" → "CONCACAF" para uniformidad
std::string Torneo::normalizarConfederacion(const std::string& conf) const {
    if (conf=="Concacaf" || conf=="CONCACAF" || conf=="concacaf") return "CONCACAF";
    if (conf=="UEFA")     return "UEFA";
    if (conf=="CONMEBOL") return "CONMEBOL";
    if (conf=="CAF")      return "CAF";
    if (conf=="AFC")      return "AFC";
    if (conf=="OFC")      return "OFC";
    return conf;
}

/// Devuelve los 8 mejores terceros por puntos para armar R16
Lista<Equipo*> Torneo::mejoresTerceros(Lista<Lista<Equipo*>>& tablas) const {
    Lista<Equipo*> terceros;
    Lista<int>     pts;
    for (int g=0; g<grupos.getTam(); g++) {
        Lista<Equipo*> tabla=grupos[g].getTablaClasificacion();
        if (tabla.getTam()>=3) {
            terceros.agregar(tabla[2]);
            pts.agregar(grupos[g].getPuntos(2));
        }
        medidor.incrementar(4);
    }
    // Ordenar por puntos descendente (burbuja)
    for (int i=0; i<terceros.getTam()-1; i++)
        for (int j=0; j<terceros.getTam()-i-1; j++) {
            if (pts[j]<pts[j+1]) {
                Equipo* tE=terceros[j]; terceros[j]=terceros[j+1]; terceros[j+1]=tE;
                int tP=pts[j]; pts[j]=pts[j+1]; pts[j+1]=tP;
            }
            medidor.incrementar();
        }
    Lista<Equipo*> mejores;
    for (int i=0;i<8&&i<terceros.getTam();i++) { mejores.agregar(terceros[i]); medidor.incrementar(); }
    return mejores;
}

/// Configura R16: cabezas vs terceros, cabezas vs peores segundos, segundos entre si
/// Garantiza que no se repitan enfrentamientos del mismo grupo
void Torneo::configurarR16(Lista<Lista<Equipo*>>& tablas) {
    Etapa r16("R16");
    Lista<Equipo*> primeros, segundos;
    for (int g=0;g<grupos.getTam();g++) {
        Lista<Equipo*> tabla=grupos[g].getTablaClasificacion();
        primeros.agregar(tabla[0]); segundos.agregar(tabla[1]);
        medidor.incrementar(2);
    }
    Lista<Equipo*> terceros=mejoresTerceros(tablas);

    Lista<bool> tUsado, pUsado, sUsado;
    for (int i=0;i<terceros.getTam();i++) tUsado.agregar(false);
    for (int i=0;i<primeros.getTam();i++) { pUsado.agregar(false); sUsado.agregar(false); }

    // 1. Primeros vs terceros
    for (int t=0;t<terceros.getTam();t++) {
        for (int p=0;p<primeros.getTam();p++) {
            if (pUsado[p]) { medidor.incrementar(); continue; }
            bool mismoGrupo=false;
            for (int g=0;g<grupos.getTam();g++) {
                Lista<Equipo*>& eq=grupos[g].getEquipos();
                bool tieneT=false, tieneP=false;
                for (int e=0;e<eq.getTam();e++) {
                    if (eq[e]==terceros[t]) tieneT=true;
                    if (eq[e]==primeros[p]) tieneP=true;
                    medidor.incrementar();
                }
                if (tieneT&&tieneP) { mismoGrupo=true; break; }
            }
            if (!mismoGrupo) {
                Partido part(primeros[p],terceros[t],FECHA_ELIMINATORIAS,"nombreSede");
                r16.agregarPartido(part);
                pUsado[p]=true; tUsado[t]=true; break;
            }
        }
    }

    // 2. Primeros restantes vs 4 peores segundos
    Lista<int> ptsSeg;
    for (int g=0;g<grupos.getTam();g++) { ptsSeg.agregar(grupos[g].getPuntos(1)); medidor.incrementar(); }
    int peores=0;
    while (peores<4) {
        int minP=999, minIdx=-1;
        for (int j=0;j<segundos.getTam();j++) {
            if (!sUsado[j]&&ptsSeg[j]<minP) { minP=ptsSeg[j]; minIdx=j; }
            medidor.incrementar();
        }
        if (minIdx==-1) break;
        for (int p=0;p<primeros.getTam();p++) {
            if (pUsado[p]) { medidor.incrementar(); continue; }
            bool mismoGrupo=false;
            for (int g=0;g<grupos.getTam();g++) {
                Lista<Equipo*>& eq=grupos[g].getEquipos();
                bool tieneS=false, tieneP=false;
                for (int e=0;e<eq.getTam();e++) {
                    if (eq[e]==segundos[minIdx]) tieneS=true;
                    if (eq[e]==primeros[p])      tieneP=true;
                    medidor.incrementar();
                }
                if (tieneS&&tieneP) { mismoGrupo=true; break; }
            }
            if (!mismoGrupo) {
                Partido part(primeros[p],segundos[minIdx],FECHA_ELIMINATORIAS,"nombreSede");
                r16.agregarPartido(part);
                pUsado[p]=true; sUsado[minIdx]=true; peores++; break;
            }
        }
    }

    // 3. Segundos restantes entre si
    Lista<int> disp;
    for (int i=0;i<segundos.getTam();i++) if (!sUsado[i]) disp.agregar(i);
    for (int i=0;i<disp.getTam()-1;i+=2) {
        Partido part(segundos[disp[i]],segundos[disp[i+1]],FECHA_ELIMINATORIAS,"nombreSede");
        r16.agregarPartido(part); medidor.incrementar();
    }
    etapas.agregar(r16);
}

/// Configura partidos de una etapa emparejando clasificados en orden
void Torneo::configurarEtapa(Etapa& etapa, Lista<Equipo*>& clasificados) {
    for (int i=0;i<clasificados.getTam()-1;i+=2) {
        Partido p(clasificados[i],clasificados[i+1],FECHA_ELIMINATORIAS,"nombreSede");
        etapa.agregarPartido(p); medidor.incrementar();
    }
}

// -- Funcionalidades principales --------------------------------------

/// I - Lee el CSV y crea los 48 equipos en heap, genera plantillas
/// Formato: ranking;pais;dt;federacion;confederacion;gf;gc;pg;pe;pp
/// Salta las 2 primeras filas (titulo + encabezado de columnas)
void Torneo::cargarDatos(const std::string& archivoCSV) {
    medidor.resetear();
    std::fstream archivo(archivoCSV, std::ios::in);
    if (!archivo.is_open()) {
        std::cout << "Error: no se pudo abrir " << archivoCSV << "\n";
        return;
    }

    std::string linea;

    // Saltar fila 1: titulo del documento
    std::getline(archivo, linea);
    // Saltar fila 2: encabezado de columnas
    std::getline(archivo, linea);
    medidor.incrementar(2);

    while (std::getline(archivo, linea)) {
        medidor.incrementar();
        if (linea.empty() || linea == "\r") continue;

        std::string campos[10];
        parsearLineaCSV(linea, campos, 10);

        // Columnas: 0=ranking, 1=pais, 2=dt, 3=federacion, 4=confederacion
        //           5=gf, 6=gc, 7=pg, 8=pe, 9=pp
        int         ranking = std::stoi(campos[0]);
        std::string pais    = campos[1];
        std::string dt      = campos[2];
        std::string conf    = normalizarConfederacion(campos[4]);
        int gf  = std::stoi(campos[5]);
        int gc  = std::stoi(campos[6]);
        int pg  = std::stoi(campos[7]);
        int pe  = std::stoi(campos[8]);
        int pp  = std::stoi(campos[9]);

        Equipo* e = new Equipo(pais, conf, ranking, dt, gf, gc, pg, pe, pp);
        e->generarPlantilla();
        equipos.agregar(e);
        medidor.incrementar(26); // generarPlantilla itera 26 veces
    }

    archivo.close();
    std::cout << "Equipos cargados: " << equipos.getTam() << "\n";
    medidor.calcularMemoria(48,26,0,0,0,0);
    medidor.reportar("I - Carga de datos");
}

/// Guarda plantillas actualizadas en archivo binario propio
/// Formato: [int numEquipos] por cada equipo: [int lenPais][chars pais][plantilla]
void Torneo::persistirDatos(const std::string& archivoJugadores) {
    std::fstream archivo(archivoJugadores, std::ios::out|std::ios::binary|std::ios::trunc);
    if (!archivo.is_open()) return;
    int n=equipos.getTam();
    archivo.write(reinterpret_cast<const char*>(&n),sizeof(int));
    for (int i=0;i<n;i++) {
        int lenPais=equipos[i]->getPais().size();
        archivo.write(reinterpret_cast<const char*>(&lenPais),sizeof(int));
        archivo.write(equipos[i]->getPais().c_str(),lenPais);
        equipos[i]->guardarPlantilla(archivo);
        medidor.incrementar(26);
    }
    archivo.close();
}

/// Carga plantillas desde archivo binario si existe
void Torneo::cargarPlantillas(const std::string& archivoJugadores) {
    std::fstream archivo(archivoJugadores, std::ios::in|std::ios::binary);
    if (!archivo.is_open()) return;
    int n=0;
    archivo.read(reinterpret_cast<char*>(&n),sizeof(int));
    for (int i=0;i<n&&i<equipos.getTam();i++) {
        int lenPais=0;
        archivo.read(reinterpret_cast<char*>(&lenPais),sizeof(int));
        std::string pais(lenPais,' ');
        archivo.read(&pais[0],lenPais);
        for (int j=0;j<equipos.getTam();j++) {
            if (equipos[j]->getPais()==pais) {
                equipos[j]->cargarPlantilla(archivo);
                medidor.incrementar(26); break;
            }
            medidor.incrementar();
        }
    }
    archivo.close();
}

/// II - Arma bombos y realiza el sorteo de grupos
void Torneo::conformarGrupos() {
    medidor.resetear();
    sorteo.armarBombos(equipos);
    medidor.incrementar(47*47);
    sorteo.conformarGrupos();
    medidor.incrementar(12*4*10);
    grupos=sorteo.getGrupos();
    int partidosPorDia[19]={};
    for (int i=0;i<grupos.getTam();i++) {
        grupos[i].generarPartidos();
        grupos[i].asignarFechas("20-06-2026",partidosPorDia,19);
        medidor.incrementar(6);
    }
    sorteo.imprimirGrupos();
    medidor.calcularMemoria(48,26,12,6,0,0);
    medidor.reportar("II - Conformacion de grupos");
}

/// III-a - Simula los 72 partidos de la fase de grupos
void Torneo::simularFaseGrupos() {
    medidor.resetear();
    for (int i=0;i<grupos.getTam();i++) {
        grupos[i].simularPartidos(simulador);
        medidor.incrementar(6*22);
        Lista<Partido>& parts=grupos[i].getPartidos();
        for (int p=0;p<parts.getTam();p++) {
            std::cout<<parts[p]<<"\n";
            parts[p].imprimirGoleadores();
            medidor.incrementar();
        }
    }
    medidor.calcularMemoria(48,26,12,6,0,0);
    medidor.reportar("III-a - Fase de grupos");
}

/// III-b - Construye tablas, imprime clasificacion y configura R16
void Torneo::transicionR16() {
    medidor.resetear();
    Lista<Lista<Equipo*>> tablas;
    for (int i=0;i<grupos.getTam();i++) {
        grupos[i].imprimirTabla();
        tablas.agregar(grupos[i].getTablaClasificacion());
        medidor.incrementar(4);
    }
    configurarR16(tablas);
    std::cout<<"\n=== PARTIDOS R16 CONFIGURADOS ===\n";
    Lista<Partido>& partsR16=etapas[0].getPartidos();
    for (int i=0;i<partsR16.getTam();i++) {
        std::cout<<partsR16[i].getActaLocal().getEquipo()->getPais()
                 <<" vs "<<partsR16[i].getActaVisitante().getEquipo()->getPais()
                 <<" | "<<partsR16[i].getFecha()<<"\n";
        medidor.incrementar();
    }
    medidor.calcularMemoria(48,26,12,6,1,32);
    medidor.reportar("III-b - Transicion a R16");
}

/// III-c - Simula R16, R8, QF, SF, 3er puesto y Final
void Torneo::simularFaseEliminatoria() {
    medidor.resetear();
    std::string nombresEtapas[]={"R8","QF","SF"};
    etapas[0].simularEtapa(simulador);
    etapas[0].determinarClasificados();
    etapas[0].imprimir();
    medidor.incrementar(32*22);
    for (int e=0;e<3;e++) {
        Etapa etapa(nombresEtapas[e]);
        configurarEtapa(etapa,etapas[etapas.getTam()-1].getClasificados());
        etapas.agregar(etapa);
        etapas[etapas.getTam()-1].simularEtapa(simulador);
        etapas[etapas.getTam()-1].determinarClasificados();
        etapas[etapas.getTam()-1].imprimir();
        medidor.incrementar(etapas[etapas.getTam()-1].getPartidos().getTam()*22);
    }
    // Perdedores de SF juegan 3er puesto
    // Se capturan por valor ANTES de agregar mas etapas para evitar
    // que la referencia quede invalida al redimensionar la lista
    Lista<Partido> partsSFcopy = etapas[etapas.getTam()-1].getPartidos();
    Lista<Equipo*> clasSFcopy  = etapas[etapas.getTam()-1].getClasificados();

    Lista<Equipo*> perdedores;
    for (int i=0;i<partsSFcopy.getTam();i++) {
        Equipo* g=partsSFcopy[i].getGanador();
        Equipo* l=partsSFcopy[i].getActaLocal().getEquipo();
        Equipo* v=partsSFcopy[i].getActaVisitante().getEquipo();
        perdedores.agregar(g==l?v:l);
        medidor.incrementar();
    }
    Etapa tercerPuesto("3er puesto");
    if (perdedores.getTam()>=2) {
        Partido p3(perdedores[0],perdedores[1],FECHA_ELIMINATORIAS,"nombreSede");
        tercerPuesto.agregarPartido(p3);
    }
    tercerPuesto.simularEtapa(simulador);
    tercerPuesto.determinarClasificados();
    tercerPuesto.imprimir();
    etapas.agregar(tercerPuesto);
    medidor.incrementar(22);
    Etapa final("Final");
    configurarEtapa(final,clasSFcopy);
    etapas.agregar(final);
    etapas[etapas.getTam()-1].simularEtapa(simulador);
    etapas[etapas.getTam()-1].determinarClasificados();
    etapas[etapas.getTam()-1].imprimir();
    medidor.incrementar(22);
    medidor.calcularMemoria(48,26,12,6,6,16);
    medidor.reportar("III-c - Fase eliminatoria");
}

/// IV - Imprime estadisticas finales del torneo
void Torneo::generarEstadisticas() {
    medidor.resetear();
    std::cout<<"\n========================================\n";
    std::cout<<"ESTADISTICAS FINALES DEL TORNEO\n";
    std::cout<<"========================================\n";
    // Top 4
    std::cout<<"\n--- Top 4 ---\n";
    for (int i=etapas.getTam()-1;i>=0;i--) {
        if (etapas[i].getNombre()=="Final") {
            Lista<Equipo*>& c=etapas[i].getClasificados();
            Lista<Partido>& pF=etapas[i].getPartidos();
            if (c.getTam()>0) std::cout<<"1. "<<c[0]->getPais()<<"\n";
            if (pF.getTam()>0) {
                Equipo* g=pF[0].getGanador();
                Equipo* l=pF[0].getActaLocal().getEquipo();
                Equipo* v=pF[0].getActaVisitante().getEquipo();
                std::cout<<"2. "<<(g==l?v->getPais():l->getPais())<<"\n";
            }
            break;
        }
        medidor.incrementar();
    }
    for (int i=etapas.getTam()-1;i>=0;i--) {
        if (etapas[i].getNombre()=="3er puesto") {
            Lista<Equipo*>& c=etapas[i].getClasificados();
            Lista<Partido>& p3=etapas[i].getPartidos();
            if (c.getTam()>0) std::cout<<"3. "<<c[0]->getPais()<<"\n";
            if (p3.getTam()>0) {
                Equipo* g=p3[0].getGanador();
                Equipo* l=p3[0].getActaLocal().getEquipo();
                Equipo* v=p3[0].getActaVisitante().getEquipo();
                std::cout<<"4. "<<(g==l?v->getPais():l->getPais())<<"\n";
            }
            break;
        }
        medidor.incrementar();
    }
    // Maximo goleador del campeon
    std::cout<<"\n--- Maximo goleador del campeon ---\n";
    Equipo* campeon=nullptr;
    for (int i=etapas.getTam()-1;i>=0;i--) {
        if (etapas[i].getNombre()=="Final") {
            Lista<Equipo*>& c=etapas[i].getClasificados();
            if (c.getTam()>0) campeon=c[0];
            break;
        }
        medidor.incrementar();
    }
    if (campeon!=nullptr) {
        Jugador* maxJ=nullptr; int maxG=-1;
        Lista<Jugador>& pl=campeon->getPlantilla();
        for (int i=0;i<pl.getTam();i++) {
            if (pl[i].getStatsHistoricas().getGoles()>maxG) {
                maxG=pl[i].getStatsHistoricas().getGoles(); maxJ=&pl[i];
            }
            medidor.incrementar();
        }
        if (maxJ!=nullptr) std::cout<<*maxJ<<"\n";
    }
    // Top 3 goleadores
    std::cout<<"\n--- Top 3 goleadores del torneo ---\n";
    Lista<Jugador*> top=getTopGoleadores(3);
    for (int i=0;i<top.getTam();i++) std::cout<<(i+1)<<". "<<*top[i]<<"\n";
    // Equipo con mas goles historicos
    std::cout<<"\n--- Equipo con mas goles historicos ---\n";
    Equipo* maxEq=nullptr; int maxGEq=-1;
    for (int i=0;i<equipos.getTam();i++) {
        int gf=equipos[i]->getStatsHistoricas().getGolesAFavor();
        if (gf>maxGEq) { maxGEq=gf; maxEq=equipos[i]; }
        medidor.incrementar();
    }
    if (maxEq!=nullptr) std::cout<<maxEq->getPais()<<" — "<<maxGEq<<" goles\n";
    // Confederacion dominante
    std::cout<<"\n--- Confederacion dominante por etapa ---\n";
    std::string buscar[]={"R16","R8","QF"};
    for (int e=0;e<3;e++)
        for (int i=0;i<etapas.getTam();i++) {
            if (etapas[i].getNombre()==buscar[e]) {
                std::cout<<buscar[e]<<": "<<getConfMayorPresencia(etapas[i])<<"\n"; break;
            }
            medidor.incrementar();
        }
    medidor.calcularMemoria(48,26,12,6,6,16);
    medidor.reportar("IV - Estadisticas finales");
}

// -- Getters ----------------------------------------------------------

Lista<Equipo*>&       Torneo::getEquipos()       { return equipos; }
Lista<Grupo>&         Torneo::getGrupos()        { return grupos;  }
Lista<Etapa>&         Torneo::getEtapas()        { return etapas;  }
MedidorRecursos&      Torneo::getMedidor()       { return medidor; }
const Lista<Equipo*>& Torneo::getEquipos() const { return equipos; }
const Lista<Grupo>&   Torneo::getGrupos()  const { return grupos;  }
const Lista<Etapa>&   Torneo::getEtapas()  const { return etapas;  }

/// Devuelve los n mayores goleadores buscando en las 48 plantillas
Lista<Jugador*> Torneo::getTopGoleadores(int n) const {
    Lista<Jugador*> todos; Lista<int> goles;
    for (int i=0;i<equipos.getTam();i++) {
        Lista<Jugador>& pl=equipos[i]->getPlantilla();
        for (int j=0;j<pl.getTam();j++) {
            todos.agregar(&pl[j]);
            goles.agregar(pl[j].getStatsHistoricas().getGoles());
        }
    }
    for (int i=0;i<n&&i<todos.getTam();i++)
        for (int j=i+1;j<todos.getTam();j++)
            if (goles[j]>goles[i]) {
                Jugador* tJ=todos[i]; todos[i]=todos[j]; todos[j]=tJ;
                int tG=goles[i]; goles[i]=goles[j]; goles[j]=tG;
            }
    Lista<Jugador*> top;
    for (int i=0;i<n&&i<todos.getTam();i++) top.agregar(todos[i]);
    return top;
}

/// Devuelve la confederacion con mayor presencia en los partidos de una etapa
std::string Torneo::getConfMayorPresencia(const Etapa& etapa) const {
    std::string confs[]={"UEFA","CONMEBOL","CONCACAF","CAF","AFC","OFC"};
    int conteo[]={0,0,0,0,0,0};
    const Lista<Partido>& parts=const_cast<Etapa&>(etapa).getPartidos();
    for (int i=0;i<parts.getTam();i++) {
        std::string cL=parts[i].getActaLocal().getEquipo()->getConfederacion();
        std::string cV=parts[i].getActaVisitante().getEquipo()->getConfederacion();
        for (int c=0;c<6;c++) { if (confs[c]==cL) conteo[c]++; if (confs[c]==cV) conteo[c]++; }
    }
    int maxIdx=0;
    for (int c=1;c<6;c++) if (conteo[c]>conteo[maxIdx]) maxIdx=c;
    return confs[maxIdx];
}

/// Calcula memoria total del torneo en bytes
size_t Torneo::calcularMemoria() const {
    size_t mem=sizeof(Torneo);
    mem+=sizeof(Equipo)*equipos.getTam();
    mem+=sizeof(Jugador)*26*equipos.getTam();
    mem+=sizeof(Grupo)*grupos.getTam();
    for (int i=0;i<etapas.getTam();i++) mem+=const_cast<Etapa&>(etapas[i]).calcularMemoria();
    return mem;
}

std::ostream& operator<<(std::ostream& os, const Torneo& t) {
    os<<"Torneo — "<<t.equipos.getTam()<<" equipos | "
      <<t.grupos.getTam()<<" grupos | "<<t.etapas.getTam()<<" etapas";
    return os;
}
