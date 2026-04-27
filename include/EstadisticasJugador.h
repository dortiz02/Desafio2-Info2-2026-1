#ifndef ESTADISTICASJUGADOR_H
#define ESTADISTICASJUGADOR_H

#include <fstream>

// Estadisticas históricas acumuladas de un jugador
// Se actualiza luego de cada partido — separada de Jugador
// para poder usar operator+= limpiamente
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
    // Constructores
    EstadisticasJugador();
    EstadisticasJugador(const EstadisticasJugador& otra);  // copia

    // Getters
    int getPartidosJugados()    const;
    int getGoles()              const;
    int getMinutosJugados()     const;
    int getAsistencias()        const;
    int getTarjetasAmarillas()  const;
    int getTarjetasRojas()      const;
    int getFaltas()             const;

    // Setters
    void setPartidosJugados   (int v);
    void setGoles             (int v);
    void setMinutosJugados    (int v);
    void setAsistencias       (int v);
    void setTarjetasAmarillas (int v);
    void setTarjetasRojas     (int v);
    void setFaltas            (int v);

    // Acumula los valores de otra estadistica sobre esta
    EstadisticasJugador& operator+=(const EstadisticasJugador& otra);

    // Asignacion
    EstadisticasJugador& operator=(const EstadisticasJugador& otra);

    // Persistencia
    void guardar(std::fstream& archivo) const;
    void cargar (std::fstream& archivo);
};

#endif
