#ifndef ACTAEQUIPO_H
#define ACTAEQUIPO_H
#include <iostream>
#include "Equipo.h"
#include "ActaJugador.h"
#include "Lista.h"
class ActaEquipo {
private:
    Equipo*            equipo;
    int                golesAFavor;
    int                golesEnContra;
    double             posesion;
    Lista<ActaJugador> convocados;
public:
    ActaEquipo();
    ActaEquipo(Equipo* e);
    ActaEquipo(const ActaEquipo& otra);
    ~ActaEquipo();
    Equipo*             getEquipo()        const;
    int                 getGolesAFavor()   const;
    int                 getGolesEnContra() const;
    double              getPosesion()      const;
    Lista<ActaJugador>&       getConvocados();
    const Lista<ActaJugador>& getConvocados() const;
    void setGolesAFavor  (int v);
    void setGolesEnContra(int v);
    void setPosesion     (double v);
    void agregarConvocado(const ActaJugador& aj);
    EstadisticasEquipo toStats(bool gano, bool empato) const;
    friend std::ostream& operator<<(std::ostream& os, const ActaEquipo& ae);
};
#endif
