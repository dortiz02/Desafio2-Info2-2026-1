#include "../include/Torneo.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

const std::string Torneo::FECHA_ELIMINATORIAS = "10-07-2026";

// -- Constructores ----------------------------------------------------

Torneo::Torneo() {}

// Constructor de copia — copia profunda de los equipos en heap
Torneo::Torneo(const Torneo& otro)
    : grupos   (otro.grupos),
      etapas   (otro.etapas),
      sorteo   (otro.sorteo),
      simulador(otro.simulador),
      medidor  (otro.medidor) {
    for (int i = 0; i < otro.equipos.getTam(); i++)
        equipos.agregar(new Equipo(*otro.equipos[i]));
}

// Destructor — libera los 48 equipos del heap
Torneo::~Torneo() {
    for (int i = 0; i < equipos.getTam(); i++) {
        delete equipos[i];
        equipos[i] = nullptr;
    }
}

// -- Helpers privados -------------------------------------------------

/// Parsea una linea CSV separada por comas
/// Llena el arreglo campos[] con hasta maxCampos valores
void Torneo::parsearLineaCSV(const std::string& linea,
                              std::string campos[], int maxCampos) const {
    int idx  = 0;
    int ini  = 0;
    for (int i = 0; i <= (int)linea.size() && idx < maxCampos; i++) {
        if (i == (int)linea.size() || linea[i] == ',') {
            campos[idx++] = linea.substr(ini, i - ini);
            ini = i + 1;
        }
    }
}

/// Devuelve los 8 mejores terceros por puntos para armar R16
Lista<Equipo*> Torneo::mejoresTerceros(Lista<Lista<Equipo*>>& tablas) const {
    // Recoger todos los terceros con sus puntos
    Lista<Equipo*> terceros;
    Lista<int>     puntosTerceros;

    for (int g = 0; g < grupos.getTam(); g++) {
        Lista<Equipo*> tabla = grupos[g].getTablaClasificacion();
        if (tabla.getTam() >= 3) {
            terceros.agregar(tabla[2]);
            puntosTerceros.agregar(grupos[g].getPuntos(
                // Obtener puntos del tercero buscando su indice
                grupos[g].getEquipos().getTam() > 2 ? 2 : 0));
        }
        medidor.incrementar(4); // iteracion sobre tabla
    }

    // Ordenar terceros por puntos descendente (burbuja)
    for (int i = 0; i < terceros.getTam() - 1; i++) {
        for (int j = 0; j < terceros.getTam() - i - 1; j++) {
            if (puntosTerceros[j] < puntosTerceros[j+1]) {
                Equipo* tmpE      = terceros[j];
                terceros[j]       = terceros[j+1];
                terceros[j+1]     = tmpE;
                int tmpP          = puntosTerceros[j];
                puntosTerceros[j] = puntosTerceros[j+1];
                puntosTerceros[j+1] = tmpP;
            }
            medidor.incrementar();
        }
    }

    // Tomar los 8 mejores
    Lista<Equipo*> mejores;
    for (int i = 0; i < 8 && i < terceros.getTam(); i++) {
        mejores.agregar(terceros[i]);
        medidor.incrementar();
    }
    return mejores;
}

/// Configura los 32 partidos de R16:
/// 1. Cabezas de grupo vs terceros clasificados
/// 2. Cabezas restantes vs 4 peores segundos
/// 3. Segundos restantes entre si
/// Garantiza que no se repitan enfrentamientos del mismo grupo
void Torneo::configurarR16(Lista<Lista<Equipo*>>& tablas) {
    Etapa r16("R16");
    Lista<Equipo*> primeros, segundos, terceros;

    for (int g = 0; g < grupos.getTam(); g++) {
        Lista<Equipo*> tabla = grupos[g].getTablaClasificacion();
        primeros.agregar(tabla[0]);
        segundos.agregar(tabla[1]);
        medidor.incrementar(2);
    }

    // Obtener 8 mejores terceros
    terceros = mejoresTerceros(tablas);

    // 1. Primeros vs terceros (12 vs 8, se usan los primeros disponibles)
    Lista<bool> terceroUsado, primeroUsado;
    for (int i = 0; i < terceros.getTam(); i++) terceroUsado.agregar(false);
    for (int i = 0; i < primeros.getTam(); i++) primeroUsado.agregar(false);

    for (int t = 0; t < terceros.getTam(); t++) {
        for (int p = 0; p < primeros.getTam(); p++) {
            if (primeroUsado[p]) { medidor.incrementar(); continue; }
            // Verificar que no sean del mismo grupo
            bool mismoGrupo = false;
            for (int g = 0; g < grupos.getTam(); g++) {
                Lista<Equipo*>& eq = grupos[g].getEquipos();
                bool tieneTercero = false, tienePrimero = false;
                for (int e = 0; e < eq.getTam(); e++) {
                    if (eq[e] == terceros[t]) tieneTercero = true;
                    if (eq[e] == primeros[p]) tienePrimero = true;
                    medidor.incrementar();
                }
                if (tieneTercero && tienePrimero) { mismoGrupo = true; break; }
            }
            if (!mismoGrupo) {
                Partido part(primeros[p], terceros[t], FECHA_ELIMINATORIAS, "nombreSede");
                r16.agregarPartido(part);
                primeroUsado[p]  = true;
                terceroUsado[t]  = true;
                break;
            }
        }
    }

    // 2. Primeros restantes vs 4 peores segundos
    // Ordenar segundos por puntos ascendente para obtener los peores
    Lista<int> puntosSegundos;
    for (int g = 0; g < grupos.getTam(); g++) {
        Lista<Equipo*> tabla = grupos[g].getTablaClasificacion();
        // Buscar indice del segundo en el grupo para obtener sus puntos
        puntosSegundos.agregar(grupos[g].getPuntos(1));
        medidor.incrementar();
    }

    Lista<bool> segundoUsado;
    for (int i = 0; i < segundos.getTam(); i++) segundoUsado.agregar(false);

    int peoresContados = 0;
    for (int i = 0; i < segundos.getTam() && peoresContados < 4; i++) {
        // Buscar el segundo con menos puntos aun no usado
        int minPts = 999, minIdx = -1;
        for (int j = 0; j < segundos.getTam(); j++) {
            if (!segundoUsado[j] && puntosSegundos[j] < minPts) {
                minPts = puntosSegundos[j];
                minIdx = j;
            }
            medidor.incrementar();
        }
        if (minIdx == -1) break;

        // Emparejar con el primer primero disponible del grupo diferente
        for (int p = 0; p < primeros.getTam(); p++) {
            if (primeroUsado[p]) { medidor.incrementar(); continue; }
            bool mismoGrupo = false;
            for (int g = 0; g < grupos.getTam(); g++) {
                Lista<Equipo*>& eq = grupos[g].getEquipos();
                bool tieneSegundo = false, tienePrimero = false;
                for (int e = 0; e < eq.getTam(); e++) {
                    if (eq[e] == segundos[minIdx]) tieneSegundo = true;
                    if (eq[e] == primeros[p])      tienePrimero = true;
                    medidor.incrementar();
                }
                if (tieneSegundo && tienePrimero) { mismoGrupo = true; break; }
            }
            if (!mismoGrupo) {
                Partido part(primeros[p], segundos[minIdx], FECHA_ELIMINATORIAS, "nombreSede");
                r16.agregarPartido(part);
                primeroUsado[p]    = true;
                segundoUsado[minIdx] = true;
                peoresContados++;
                break;
            }
        }
    }

    // 3. Segundos restantes entre si
    Lista<int> disponibles;
    for (int i = 0; i < segundos.getTam(); i++)
        if (!segundoUsado[i]) disponibles.agregar(i);

    for (int i = 0; i < disponibles.getTam() - 1; i += 2) {
        Partido part(segundos[disponibles[i]], segundos[disponibles[i+1]],
                     FECHA_ELIMINATORIAS, "nombreSede");
        r16.agregarPartido(part);
        medidor.incrementar();
    }

    etapas.agregar(r16);
}

/// Configura los partidos de una etapa a partir de los clasificados
/// Empareja en orden: clasificado[0] vs clasificado[1], etc.
void Torneo::configurarEtapa(Etapa& etapa, Lista<Equipo*>& clasificados) {
    for (int i = 0; i < clasificados.getTam() - 1; i += 2) {
        Partido p(clasificados[i], clasificados[i+1],
                  FECHA_ELIMINATORIAS, "nombreSede");
        etapa.agregarPartido(p);
        medidor.incrementar();
    }
}

// -- Funcionalidades principales --------------------------------------

/// I - Lee el CSV y crea los 48 equipos en heap
/// Formato CSV: pais,confederacion,ranking,dt,gf,gc,pg,pe,pp
void Torneo::cargarDatos(const std::string& archivoCSV) {
    medidor.resetear();
    std::fstream archivo(archivoCSV, std::ios::in);
    if (!archivo.is_open()) {
        std::cout << "Error: no se pudo abrir " << archivoCSV << "\n";
        return;
    }

    std::string linea;
    // Saltar encabezado
    std::getline(archivo, linea);
    medidor.incrementar();

    while (std::getline(archivo, linea)) {
        medidor.incrementar();
        if (linea.empty()) continue;

        std::string campos[9];
        parsearLineaCSV(linea, campos, 9);

        std::string pais   = campos[0];
        std::string conf   = campos[1];
        int ranking        = std::stoi(campos[2]);
        std::string dt     = campos[3];
        int gf             = std::stoi(campos[4]);
        int gc             = std::stoi(campos[5]);
        int pg             = std::stoi(campos[6]);
        int pe             = std::stoi(campos[7]);
        int pp             = std::stoi(campos[8]);

        Equipo* e = new Equipo(pais, conf, ranking, dt, gf, gc, pg, pe, pp);
        e->generarPlantilla();
        equipos.agregar(e);
        medidor.incrementar(26); // generarPlantilla itera 26 veces
    }

    archivo.close();
    medidor.calcularMemoria(48, 26, 0, 0, 0, 0);
    medidor.reportar("I - Carga de datos");
}

/// Guarda las plantillas actualizadas en archivo binario propio
/// Formato: [int numEquipos][string pais][plantilla Equipo]...
void Torneo::persistirDatos(const std::string& archivoJugadores) {
    std::fstream archivo(archivoJugadores,
                         std::ios::out | std::ios::binary | std::ios::trunc);
    if (!archivo.is_open()) return;

    int n = equipos.getTam();
    archivo.write(reinterpret_cast<const char*>(&n), sizeof(int));

    for (int i = 0; i < n; i++) {
        // Guardar pais como clave de identificacion
        int lenPais = equipos[i]->getPais().size();
        archivo.write(reinterpret_cast<const char*>(&lenPais), sizeof(int));
        archivo.write(equipos[i]->getPais().c_str(), lenPais);
        equipos[i]->guardarPlantilla(archivo);
        medidor.incrementar(26);
    }
    archivo.close();
}

/// Carga plantillas desde archivo binario si existe
void Torneo::cargarPlantillas(const std::string& archivoJugadores) {
    std::fstream archivo(archivoJugadores,
                         std::ios::in | std::ios::binary);
    if (!archivo.is_open()) return;

    int n = 0;
    archivo.read(reinterpret_cast<char*>(&n), sizeof(int));

    for (int i = 0; i < n && i < equipos.getTam(); i++) {
        int lenPais = 0;
        archivo.read(reinterpret_cast<char*>(&lenPais), sizeof(int));
        std::string pais(lenPais, ' ');
        archivo.read(&pais[0], lenPais);
        // Buscar el equipo por pais y cargar su plantilla
        for (int j = 0; j < equipos.getTam(); j++) {
            if (equipos[j]->getPais() == pais) {
                equipos[j]->cargarPlantilla(archivo);
                medidor.incrementar(26);
                break;
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
    medidor.incrementar(47 * 47); // costo del ordenamiento burbuja
    sorteo.conformarGrupos();
    medidor.incrementar(12 * 4 * 10); // costo promedio del sorteo
    grupos = sorteo.getGrupos();

    // Generar partidos y asignar fechas
    int partidosPorDia[19] = {};
    for (int i = 0; i < grupos.getTam(); i++) {
        grupos[i].generarPartidos();
        grupos[i].asignarFechas("20-06-2026", partidosPorDia, 19);
        medidor.incrementar(6); // 6 partidos por grupo
    }

    sorteo.imprimirGrupos();
    medidor.calcularMemoria(48, 26, 12, 6, 0, 0);
    medidor.reportar("II - Conformacion de grupos");
}

/// III-a - Simula los 72 partidos de la fase de grupos
void Torneo::simularFaseGrupos() {
    medidor.resetear();
    for (int i = 0; i < grupos.getTam(); i++) {
        grupos[i].simularPartidos(simulador);
        medidor.incrementar(6 * 22); // 6 partidos x 22 jugadores
        // Imprimir partidos del grupo
        Lista<Partido>& parts = grupos[i].getPartidos();
        for (int p = 0; p < parts.getTam(); p++) {
            std::cout << parts[p] << "\n";
            parts[p].imprimirGoleadores();
            medidor.incrementar();
        }
    }
    medidor.calcularMemoria(48, 26, 12, 6, 0, 0);
    medidor.reportar("III-a - Fase de grupos");
}

/// III-b - Construye tablas, imprime clasificacion y configura R16
void Torneo::transicionR16() {
    medidor.resetear();

    // Imprimir todas las tablas de clasificacion
    Lista<Lista<Equipo*>> tablas;
    for (int i = 0; i < grupos.getTam(); i++) {
        grupos[i].imprimirTabla();
        tablas.agregar(grupos[i].getTablaClasificacion());
        medidor.incrementar(4);
    }

    configurarR16(tablas);

    // Imprimir partidos configurados para R16 sin simular
    std::cout << "\n=== PARTIDOS R16 CONFIGURADOS ===\n";
    Lista<Partido>& partsR16 = etapas[0].getPartidos();
    for (int i = 0; i < partsR16.getTam(); i++) {
        std::cout << partsR16[i].getActaLocal().getEquipo()->getPais()
                  << " vs "
                  << partsR16[i].getActaVisitante().getEquipo()->getPais()
                  << " | " << partsR16[i].getFecha() << "\n";
        medidor.incrementar();
    }

    medidor.calcularMemoria(48, 26, 12, 6, 1, 32);
    medidor.reportar("III-b - Transicion a R16");
}

/// III-c - Simula R16, R8, QF, SF, Final y 3er puesto
void Torneo::simularFaseEliminatoria() {
    medidor.resetear();

    // Nombres de etapas en orden
    std::string nombresEtapas[] = {"R8", "QF", "SF"};

    // Simular R16 (ya esta en etapas[0])
    etapas[0].simularEtapa(simulador);
    etapas[0].determinarClasificados();
    etapas[0].imprimir();
    medidor.incrementar(32 * 22);

    // Simular R8, QF, SF encadenando clasificados
    for (int e = 0; e < 3; e++) {
        Etapa etapa(nombresEtapas[e]);
        configurarEtapa(etapa, etapas[etapas.getTam()-1].getClasificados());
        etapas.agregar(etapa);
        etapas[etapas.getTam()-1].simularEtapa(simulador);
        etapas[etapas.getTam()-1].determinarClasificados();
        etapas[etapas.getTam()-1].imprimir();
        medidor.incrementar(etapa.getPartidos().getTam() * 22);
    }

    // Semifinal — los perdedores juegan el 3er puesto
    Lista<Equipo*>& clasSF = etapas[etapas.getTam()-1].getClasificados();
    Lista<Partido>& partsSF = etapas[etapas.getTam()-1].getPartidos();

    // Recoger perdedores de SF
    Lista<Equipo*> perdedoresSF;
    for (int i = 0; i < partsSF.getTam(); i++) {
        Equipo* ganador = partsSF[i].getGanador();
        Equipo* local   = partsSF[i].getActaLocal().getEquipo();
        Equipo* visit   = partsSF[i].getActaVisitante().getEquipo();
        perdedoresSF.agregar(ganador == local ? visit : local);
        medidor.incrementar();
    }

    // 3er puesto
    Etapa tercerPuesto("3er puesto");
    if (perdedoresSF.getTam() >= 2) {
        Partido p3(perdedoresSF[0], perdedoresSF[1],
                   FECHA_ELIMINATORIAS, "nombreSede");
        tercerPuesto.agregarPartido(p3);
    }
    tercerPuesto.simularEtapa(simulador);
    tercerPuesto.determinarClasificados();
    tercerPuesto.imprimir();
    etapas.agregar(tercerPuesto);
    medidor.incrementar(22);

    // Final
    Etapa final("Final");
    configurarEtapa(final, clasSF);
    etapas.agregar(final);
    etapas[etapas.getTam()-1].simularEtapa(simulador);
    etapas[etapas.getTam()-1].determinarClasificados();
    etapas[etapas.getTam()-1].imprimir();
    medidor.incrementar(22);

    medidor.calcularMemoria(48, 26, 12, 6, 6, 16);
    medidor.reportar("III-c - Fase eliminatoria");
}

/// IV - Imprime estadisticas finales del torneo
void Torneo::generarEstadisticas() {
    medidor.resetear();

    std::cout << "\n========================================\n";
    std::cout << "ESTADISTICAS FINALES DEL TORNEO\n";
    std::cout << "========================================\n";

    // 1. Ranking de los 4 primeros puestos
    std::cout << "\n--- Top 4 ---\n";
    Lista<Etapa>& et = etapas;
    // El campeon esta en los clasificados de la Final
    for (int i = et.getTam()-1; i >= 0; i--) {
        if (et[i].getNombre() == "Final") {
            Lista<Equipo*>& clas = et[i].getClasificados();
            if (clas.getTam() > 0)
                std::cout << "1. " << clas[0]->getPais() << "\n";
            // Subcampeon: el perdedor de la final
            Lista<Partido>& pF = et[i].getPartidos();
            if (pF.getTam() > 0) {
                Equipo* g = pF[0].getGanador();
                Equipo* l = pF[0].getActaLocal().getEquipo();
                Equipo* v = pF[0].getActaVisitante().getEquipo();
                std::cout << "2. " << (g==l ? v->getPais() : l->getPais()) << "\n";
            }
            break;
        }
        medidor.incrementar();
    }
    // 3er y 4to del partido de tercer puesto
    for (int i = et.getTam()-1; i >= 0; i--) {
        if (et[i].getNombre() == "3er puesto") {
            Lista<Equipo*>& clas = et[i].getClasificados();
            Lista<Partido>& p3   = et[i].getPartidos();
            if (clas.getTam() > 0)
                std::cout << "3. " << clas[0]->getPais() << "\n";
            if (p3.getTam() > 0) {
                Equipo* g = p3[0].getGanador();
                Equipo* l = p3[0].getActaLocal().getEquipo();
                Equipo* v = p3[0].getActaVisitante().getEquipo();
                std::cout << "4. " << (g==l ? v->getPais() : l->getPais()) << "\n";
            }
            break;
        }
        medidor.incrementar();
    }

    // 2. Maximo goleador del equipo campeon
    std::cout << "\n--- Maximo goleador del campeon ---\n";
    Lista<Jugador*> top = getTopGoleadores(3);
    // Buscar campeon
    Equipo* campeon = nullptr;
    for (int i = et.getTam()-1; i >= 0; i--) {
        if (et[i].getNombre() == "Final") {
            Lista<Equipo*>& c = et[i].getClasificados();
            if (c.getTam() > 0) campeon = c[0];
            break;
        }
        medidor.incrementar();
    }
    if (campeon != nullptr) {
        Jugador* maxGoleador = nullptr;
        int maxGoles = -1;
        Lista<Jugador>& plantilla = campeon->getPlantilla();
        for (int i = 0; i < plantilla.getTam(); i++) {
            if (plantilla[i].getStatsHistoricas().getGoles() > maxGoles) {
                maxGoles     = plantilla[i].getStatsHistoricas().getGoles();
                maxGoleador  = &plantilla[i];
            }
            medidor.incrementar();
        }
        if (maxGoleador != nullptr)
            std::cout << *maxGoleador << "\n";
    }

    // 3. Top 3 goleadores del torneo
    std::cout << "\n--- Top 3 goleadores del torneo ---\n";
    for (int i = 0; i < top.getTam(); i++)
        std::cout << (i+1) << ". " << *top[i] << "\n";

    // 4. Equipo con mas goles historicos
    std::cout << "\n--- Equipo con mas goles historicos ---\n";
    Equipo* maxEquipo = nullptr;
    int maxGolesEq    = -1;
    for (int i = 0; i < equipos.getTam(); i++) {
        int gf = equipos[i]->getStatsHistoricas().getGolesAFavor();
        if (gf > maxGolesEq) { maxGolesEq = gf; maxEquipo = equipos[i]; }
        medidor.incrementar();
    }
    if (maxEquipo != nullptr)
        std::cout << maxEquipo->getPais() << " — " << maxGolesEq << " goles\n";

    // 5. Confederacion con mayor presencia en R16, R8, QF
    std::cout << "\n--- Confederacion dominante por etapa ---\n";
    std::string etapasBuscar[] = {"R16", "R8", "QF"};
    for (int e = 0; e < 3; e++) {
        for (int i = 0; i < et.getTam(); i++) {
            if (et[i].getNombre() == etapasBuscar[e]) {
                std::cout << etapasBuscar[e] << ": "
                          << getConfMayorPresencia(et[i]) << "\n";
                break;
            }
            medidor.incrementar();
        }
    }

    medidor.calcularMemoria(48, 26, 12, 6, 6, 16);
    medidor.reportar("IV - Estadisticas finales");
}

// -- Getters ----------------------------------------------------------

Lista<Equipo*>&  Torneo::getEquipos() { return equipos; }
Lista<Grupo>&    Torneo::getGrupos()  { return grupos;  }
Lista<Etapa>&    Torneo::getEtapas()  { return etapas;  }
MedidorRecursos& Torneo::getMedidor() { return medidor; }

/// Devuelve los n mayores goleadores de todo el torneo
/// Busca en la plantilla de los 48 equipos
Lista<Jugador*> Torneo::getTopGoleadores(int n) const {
    Lista<Jugador*> todos;
    Lista<int>      goles;

    for (int i = 0; i < equipos.getTam(); i++) {
        Lista<Jugador>& plantilla = equipos[i]->getPlantilla();
        for (int j = 0; j < plantilla.getTam(); j++) {
            todos.agregar(&plantilla[j]);
            goles.agregar(plantilla[j].getStatsHistoricas().getGoles());
        }
    }

    // Ordenar por goles descendente (burbuja parcial — solo necesitamos n)
    for (int i = 0; i < n && i < todos.getTam(); i++) {
        for (int j = i+1; j < todos.getTam(); j++) {
            if (goles[j] > goles[i]) {
                Jugador* tmpJ = todos[i]; todos[i] = todos[j]; todos[j] = tmpJ;
                int tmpG      = goles[i]; goles[i] = goles[j]; goles[j] = tmpG;
            }
        }
    }

    Lista<Jugador*> top;
    for (int i = 0; i < n && i < todos.getTam(); i++)
        top.agregar(todos[i]);
    return top;
}

/// Devuelve la confederacion con mayor presencia en una etapa
std::string Torneo::getConfMayorPresencia(const Etapa& etapa) const {
    // Contar equipos por confederacion en los partidos de la etapa
    std::string confs[]  = {"UEFA","CONMEBOL","CONCACAF","CAF","AFC","OFC"};
    int         conteo[] = {0, 0, 0, 0, 0, 0};
    int         numConfs = 6;

    const Lista<Partido>& parts = const_cast<Etapa&>(etapa).getPartidos();
    for (int i = 0; i < parts.getTam(); i++) {
        std::string confL = parts[i].getActaLocal().getEquipo()->getConfederacion();
        std::string confV = parts[i].getActaVisitante().getEquipo()->getConfederacion();
        for (int c = 0; c < numConfs; c++) {
            if (confs[c] == confL) conteo[c]++;
            if (confs[c] == confV) conteo[c]++;
        }
    }

    int maxIdx = 0;
    for (int c = 1; c < numConfs; c++)
        if (conteo[c] > conteo[maxIdx]) maxIdx = c;
    return confs[maxIdx];
}

/// Calcula memoria total del torneo en bytes
size_t Torneo::calcularMemoria() const {
    size_t mem = sizeof(Torneo);
    mem += sizeof(Equipo)  * equipos.getTam();
    mem += sizeof(Jugador) * 26 * equipos.getTam();
    mem += sizeof(Grupo)   * grupos.getTam();
    for (int i = 0; i < etapas.getTam(); i++)
        mem += const_cast<Etapa&>(etapas[i]).calcularMemoria();
    return mem;
}

std::ostream& operator<<(std::ostream& os, const Torneo& t) {
    os << "Torneo — " << t.equipos.getTam() << " equipos | "
       << t.grupos.getTam()  << " grupos | "
       << t.etapas.getTam()  << " etapas";
    return os;
}
