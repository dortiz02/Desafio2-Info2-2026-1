#ifndef ESTADISTICASEQUIPO_H
#define ESTADISTICASEQUIPO_H

#include <fstream>

// Estadísticas historicas acumuladas de un equipo
// Separada de Equipo para evitar duplicacion y facilitar
// la acumulación con operator+= tras cada partido
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
    // Constructores
    EstadisticasEquipo();
    EstadisticasEquipo(const EstadisticasEquipo& otra);  // copia

    // Getters
    int getGolesAFavor()        const;
    int getGolesEnContra()      const;
    int getPartidosGanados()    const;
    int getPartidosEmpatados()  const;
    int getPartidosPerdidos()   const;
    int getTarjetasAmarillas()  const;
    int getTarjetasRojas()      const;
    int getFaltas()             const;

    // Setters
    void setGolesAFavor       (int v);
    void setGolesEnContra     (int v);
    void setPartidosGanados   (int v);
    void setPartidosEmpatados (int v);
    void setPartidosPerdidos  (int v);
    void setTarjetasAmarillas (int v);
    void setTarjetasRojas     (int v);
    void setFaltas            (int v);

    // Acumula los valores de otra estadistica sobre esta
    EstadisticasEquipo& operator+=(const EstadisticasEquipo& otra);

    // Asignacion
    EstadisticasEquipo& operator=(const EstadisticasEquipo& otra);

    // Persistencia
    void guardar(std::fstream& archivo) const;
    void cargar (std::fstream& archivo);
};

#endif
