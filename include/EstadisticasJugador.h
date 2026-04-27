#ifndef ESTADISTICASJUGADOR_H
#define ESTADISTICASJUGADOR_H

#include <fstream>

// Estadísticas históricas acumuladas de un jugador
// Separada de Jugador para poder usar operator+= limpiamente
class EstadisticasJugador {
private:
    int partidosJugados;
    int goles;
    int minutosJugados;
    int asistencias;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;

public:
    EstadisticasJugador();
    EstadisticasJugador(const EstadisticasJugador& otra);

    int getPartidosJugados()    const;
    int getGoles()              const;
    int getMinutosJugados()     const;
    int getAsistencias()        const;
    int getTarjetasAmarillas()  const;
    int getTarjetasRojas()      const;
    int getFaltas()             const;

    void setPartidosJugados   (int v);
    void setGoles             (int v);
    void setMinutosJugados    (int v);
    void setAsistencias       (int v);
    void setTarjetasAmarillas (int v);
    void setTarjetasRojas     (int v);
    void setFaltas            (int v);

    EstadisticasJugador& operator+=(const EstadisticasJugador& otra);
    EstadisticasJugador& operator= (const EstadisticasJugador& otra);

    void guardar(std::fstream& archivo) const;
    void cargar (std::fstream& archivo);
};

#endif
