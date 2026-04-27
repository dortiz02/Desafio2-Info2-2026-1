#ifndef TORNEO_H
#define TORNEO_H

#include <string>
#include <fstream>
#include <iostream>
#include "Lista.h"
#include "Equipo.h"
#include "Grupo.h"
#include "Sorteo.h"
#include "Etapa.h"
#include "Simulador.h"
#include "MedidorRecursos.h"

// Clase principal del sistema — coordina todas las funcionalidades
// Es duena de los 48 equipos por puntero (memoria dinamica)
// Todos los demas componentes referencian esos mismos punteros
class Torneo {
private:
    Lista<Equipo*>   equipos;    // 48 equipos — dueno, memoria dinamica
    Lista<Grupo>     grupos;     // 12 grupos — referencia a equipos
    Lista<Etapa>     etapas;     // etapas eliminatorias
    Sorteo           sorteo;
    Simulador        simulador;
    MedidorRecursos  medidor;

    // Fecha fija para etapas eliminatorias segun enunciado
    static const std::string FECHA_ELIMINATORIAS;

    // Lee una linea del CSV y extrae los campos separados por coma
    void parsearLineaCSV(const std::string& linea, std::string campos[], int maxCampos) const;

    // Ordena los equipos clasificados en terceros por puntos para R16
    Lista<Equipo*> mejoresTerceros(Lista<Lista<Equipo*>>& tablas) const;

    // Configura los 32 partidos de R16 respetando que no se repitan
    // equipos del mismo grupo
    void configurarR16(Lista<Lista<Equipo*>>& tablas);

    // Configura los partidos de una etapa eliminatoria a partir
    // de los clasificados de la etapa anterior
    void configurarEtapa(Etapa& etapa, Lista<Equipo*>& clasificados);

public:
    // Constructores
    Torneo();
    Torneo(const Torneo& otro);  // copia — requerido por enunciado
    ~Torneo();                   // libera los 48 equipos del heap

    // I - Carga de datos desde CSV
    // Lee selecciones_clasificadas_mundial.csv y genera plantillas
    void cargarDatos(const std::string& archivoCSV);

    // Guarda plantillas actualizadas al archivo de jugadores
    void persistirDatos(const std::string& archivoJugadores);

    // Carga plantillas desde archivo si existe (inicio de segunda ejecucion)
    void cargarPlantillas(const std::string& archivoJugadores);

    // II - Conformacion de grupos
    void conformarGrupos();

    // III-a - Simulacion fase de grupos
    void simularFaseGrupos();

    // III-b - Transicion a R16: construye tabla y configura partidos
    void transicionR16();

    // III-c - Simulacion del resto de etapas
    void simularFaseEliminatoria();

    // IV - Estadisticas finales del torneo
    void generarEstadisticas();

    // Getters
    Lista<Equipo*>&  getEquipos();
    Lista<Grupo>&    getGrupos();
    Lista<Etapa>&    getEtapas();
    MedidorRecursos& getMedidor();

    // Devuelve los n mayores goleadores de todo el torneo
    Lista<Jugador*> getTopGoleadores(int n) const;

    // Devuelve la confederacion con mayor presencia en una etapa
    std::string getConfMayorPresencia(const Etapa& etapa) const;

    // Calcula memoria total del torneo en bytes
    size_t calcularMemoria() const;

    friend std::ostream& operator<<(std::ostream& os, const Torneo& t);
};

#endif
