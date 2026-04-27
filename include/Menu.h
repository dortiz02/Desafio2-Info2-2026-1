#ifndef MENU_H
#define MENU_H

#include "Torneo.h"

// Maneja la interaccion con el usuario por consola
// Es dueno del Torneo por valor
// Cada opcion del menu corresponde a una funcionalidad del enunciado
class Menu {
private:
    Torneo      torneo;
    std::string archivoCSV;       // ruta del CSV de equipos
    std::string archivoJugadores; // ruta del archivo binario de plantillas

    // Imprime el menu principal en consola
    void mostrarOpciones() const;

    // Verifica si ya se cargo el CSV antes de ejecutar otra opcion
    bool datosListos() const;

    // Verifica si ya se conformaron los grupos
    bool gruposListos() const;

    // Verifica si ya se simulo la fase de grupos
    bool gruposSimulados() const;

    // Verifica si ya se configuro R16
    bool r16Listo() const;

public:
    // Constructores
    Menu();
    Menu(const std::string& csv, const std::string& jugadores);
    Menu(const Menu& otro);  // copia — requerido por enunciado
    ~Menu();

    // Inicia el loop principal del menu
    void ejecutar();

    // Opciones individuales — cada una corresponde a una funcionalidad
    void opcionCargar          ();
    void opcionConformarGrupos ();
    void opcionSimularGrupos   ();
    void opcionTransicionR16   ();
    void opcionSimularEliminatorias();
    void opcionEstadisticas    ();
    void opcionPersistir       ();
};

#endif
