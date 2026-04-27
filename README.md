# UdeAWorldCup — Simulador del Mundial FIFA 2026

Informatica II | Semestre 2026-1 | Universidad de Antioquia

## Descripcion

Simulador del torneo FIFA World Cup 2026 implementado en C++ con programacion orientada a objetos. Gestiona 48 equipos, fase de grupos, etapas eliminatorias y estadisticas finales.

## Estructura del proyecto

```
UdeAWorldCup/
├── include/          # Headers (.h)
├── src/              # Implementaciones (.cpp)
├── data/             # CSV de selecciones clasificadas
├── bin/              # Ejecutable generado
├── Makefile
└── README.md
```

## Compilacion

```bash
make
```

Para limpiar y recompilar:

```bash
make rebuild
```

## Ejecucion

```bash
make run
# o directamente:
./bin/UdeAWorldCup
```

El programa busca el CSV en `data/selecciones_clasificadas_mundial.csv`.

## Menu principal

```
1. Cargar datos (CSV)
2. Conformar grupos (sorteo)
3. Simular fase de grupos
4. Transicion a R16
5. Simular fase eliminatoria
6. Estadisticas finales
7. Guardar datos actualizados
0. Salir
```

Las opciones deben ejecutarse en orden (1 → 2 → 3 → 4 → 5 → 6).

## Formato del archivo de jugadores

Al ejecutar la opcion 7, se genera `data/jugadores.dat` en formato binario propio:

```
[int: numEquipos]
  por cada equipo:
    [int: longitud del nombre del pais]
    [chars: nombre del pais]
    [int: numJugadores]
      por cada jugador:
        [int: longitud nombre] [chars: nombre]
        [int: longitud apellido] [chars: apellido]
        [int: numeroCamiseta]
        [int x 7: partidosJugados, goles, minutos,
                  asistencias, amarillas, rojas, faltas]
```

En la siguiente ejecucion, si el archivo existe, las plantillas se cargan desde el en lugar de generarse artificialmente.

## Clases principales

| Clase | Responsabilidad |
|---|---|
| `Lista<T>` | Estructura de datos dinamica propia |
| `Jugador` | Jugador con estadisticas historicas |
| `Equipo` | Equipo con plantilla de 26 jugadores |
| `Partido` | Encuentro con dos actas |
| `Simulador` | Logica probabilistica de simulacion |
| `Grupo` | Fase de grupos con round-robin |
| `Sorteo` | Conformacion de grupos con restricciones |
| `Etapa` | Ronda eliminatoria |
| `Torneo` | Coordinador principal |
| `MedidorRecursos` | Reporte de iteraciones y memoria |

## Requisitos

- g++ con soporte C++17
- Sistema Linux/Unix
