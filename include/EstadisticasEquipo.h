#ifndef ESTADISTICASEQUIPO_H
#define ESTADISTICASEQUIPO_H

#include <fstream>

class EstadisticasEquipo {
private:
    int golesAFavor;
    int golesEnContra;
    int partidosGanados;
    int partidosEmpatados;
    int partidosPerdidos;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;

public:
    EstadisticasEquipo();
    EstadisticasEquipo(const EstadisticasEquipo& otra);

    int getGolesAFavor()       const;
    int getGolesEnContra()     const;
    int getPartidosGanados()   const;
    int getPartidosEmpatados() const;
    int getPartidosPerdidos()  const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas()     const;
    int getFaltas()            const;

    void setGolesAFavor       (int v);
    void setGolesEnContra     (int v);
    void setPartidosGanados   (int v);
    void setPartidosEmpatados (int v);
    void setPartidosPerdidos  (int v);
    void setTarjetasAmarillas (int v);
    void setTarjetasRojas     (int v);
    void setFaltas            (int v);

    EstadisticasEquipo& operator+=(const EstadisticasEquipo& otra);
    EstadisticasEquipo& operator= (const EstadisticasEquipo& otra);

    void guardar(std::fstream& archivo) const;
    void cargar (std::fstream& archivo);
};

#endif
