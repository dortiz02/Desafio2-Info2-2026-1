#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H
#include <iostream>
#include <string>
#include "ActaJugador.h"
#include "ActaEquipo.h"

// Mide el consumo de recursos de cada funcionalidad
// Se pasa por referencia a los metodos que necesitan contar iteraciones
// Al finalizar cada funcionalidad se llama a reportar()
class MedidorRecursos {
private:
    long long iteraciones;
    size_t    memoriaBytes;

public:
    MedidorRecursos();
    MedidorRecursos(const MedidorRecursos& otro);
    ~MedidorRecursos();

    long long getIteraciones()  const;
    size_t    getMemoriaBytes() const;

    void incrementar(long long n = 1);
    void resetear();

    void calcularMemoria(
        int numEquipos,
        int jugadoresPorEquipo,
        int numGrupos,
        int partidosPorGrupo,
        int numEtapas,
        int partidosPorEtapa
    );

    void reportar(const std::string& nombreFuncionalidad) const;

    MedidorRecursos operator+(const MedidorRecursos& otro) const;

    friend std::ostream& operator<<(std::ostream& os, const MedidorRecursos& m);
};
#endif
