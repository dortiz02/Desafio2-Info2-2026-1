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

// Clase principal — coordina todas las funcionalidades del torneo
// Es duena de los 48 equipos en memoria dinamica
class Torneo {
private:
    Lista<Equipo*>          equipos;
    Lista<Grupo>            grupos;
    Lista<Etapa>            etapas;
    Sorteo                  sorteo;
    Simulador               simulador;
    mutable MedidorRecursos medidor;  // mutable: se modifica incluso en metodos const

    static const std::string FECHA_ELIMINATORIAS;

    /// Parsea una linea con separador ';' en campos individuales
    void parsearLineaCSV(const std::string& linea, std::string campos[], int maxCampos) const;

    /// Normaliza el nombre de la confederacion al formato interno
    std::string normalizarConfederacion(const std::string& conf) const;

    /// Devuelve los 8 mejores terceros por puntos para armar R16
    Lista<Equipo*> mejoresTerceros(Lista<Lista<Equipo*>>& tablas) const;

    /// Configura los 32 partidos de R16 respetando que no se repitan grupos
    void configurarR16(Lista<Lista<Equipo*>>& tablas);

    /// Configura los partidos de una etapa a partir de los clasificados anteriores
    void configurarEtapa(Etapa& etapa, Lista<Equipo*>& clasificados);

public:
    Torneo();
    Torneo(const Torneo& otro);
    ~Torneo();

    void cargarDatos      (const std::string& archivoCSV);
    void persistirDatos   (const std::string& archivoJugadores);
    void cargarPlantillas (const std::string& archivoJugadores);
    void conformarGrupos  ();
    void simularFaseGrupos();
    void transicionR16    ();
    void simularFaseEliminatoria();
    void generarEstadisticas    ();

    Lista<Equipo*>&       getEquipos();
    Lista<Grupo>&         getGrupos();
    Lista<Etapa>&         getEtapas();
    MedidorRecursos&      getMedidor();
    const Lista<Equipo*>& getEquipos() const;
    const Lista<Grupo>&   getGrupos()  const;
    const Lista<Etapa>&   getEtapas()  const;

    Lista<Jugador*> getTopGoleadores     (int n)              const;
    std::string     getConfMayorPresencia(const Etapa& etapa) const;
    size_t          calcularMemoria      ()                    const;

    friend std::ostream& operator<<(std::ostream& os, const Torneo& t);
};
#endif
