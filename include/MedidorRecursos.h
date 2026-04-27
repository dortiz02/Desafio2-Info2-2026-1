#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <iostream>
#include "Lista.h"

// Mide el consumo de recursos de cada funcionalidad
// Se pasa por referencia a los metodos que necesitan contar iteraciones
// Al finalizar cada funcionalidad se llama a reportar()
class MedidorRecursos {
private:
    long long iteraciones;  // contador de iteraciones directas e indirectas
    size_t    memoriaBytes; // total de bytes de todas las estructuras activas

public:
    // Constructores
    MedidorRecursos();
    MedidorRecursos(const MedidorRecursos& otro);  // copia — requerido por enunciado
    ~MedidorRecursos();

    // Getters
    long long getIteraciones()  const;
    size_t    getMemoriaBytes() const;

    // Incrementa el contador de iteraciones en n unidades
    // Se llama desde cualquier metodo que itere
    void incrementar(long long n = 1);

    // Resetea ambos contadores a cero
    // Se llama al inicio de cada funcionalidad
    void resetear();

    // Calcula el total de memoria de todas las estructuras activas
    // Recibe los tamanos individuales de cada componente
    void calcularMemoria(
        int numEquipos,
        int jugadoresPorEquipo,
        int numGrupos,
        int partidosPorGrupo,
        int numEtapas,
        int partidosPorEtapa
    );

    // Imprime el reporte de iteraciones y memoria al finalizar
    // una funcionalidad
    void reportar(const std::string& nombreFuncionalidad) const;

    // Suma dos medidores — util para acumular reportes parciales
    MedidorRecursos operator+(const MedidorRecursos& otro) const;

    friend std::ostream& operator<<(std::ostream& os, const MedidorRecursos& m);
};

#endif
