#ifndef ACTAEQUIPO_H
#define ACTAEQUIPO_H

#include <iostream>
#include "Equipo.h"
#include "ActaJugador.h"
#include "Lista.h"

// Registra lo que hizo un equipo en un partido especifico
// No es dueño del Equipo — solo guarda un puntero
class ActaEquipo {
private:
    Equipo*          equipo;
    int              golesAFavor;
    int              golesEnContra;
    double           posesion;
    Lista<ActaJugador> convocados;   // exactamente 11

public:
    ActaEquipo();
    ActaEquipo(Equipo* e);
    ActaEquipo(const ActaEquipo& otra);  // copia — requerido por enunciado
    ~ActaEquipo();

    Equipo*            getEquipo()       const;
    int                getGolesAFavor()  const;
    int                getGolesEnContra()const;
    double             getPosesion()     const;
    Lista<ActaJugador>& getConvocados();

    void setGolesAFavor  (int v);
    void setGolesEnContra(int v);
    void setPosesion     (double v);

    void agregarConvocado(const ActaJugador& aj);

    // Construye EstadisticasEquipo del partido para acumular al histórico
    EstadisticasEquipo toStats(bool gano, bool empato) const;

    friend std::ostream& operator<<(std::ostream& os, const ActaEquipo& ae);
};

#endif
