#ifndef MENU_H
#define MENU_H
#include "Torneo.h"

// Maneja la interaccion con el usuario por consola
// Es dueno del Torneo por valor
// Cada opcion corresponde a una funcionalidad del enunciado
class Menu {
private:
    Torneo      torneo;
    std::string archivoCSV;
    std::string archivoJugadores;

    void mostrarOpciones()   const;
    bool datosListos()       const;
    bool gruposListos()      const;
    bool gruposSimulados()   const;
    bool r16Listo()          const;

public:
    Menu();
    Menu(const std::string& csv, const std::string& jugadores);
    Menu(const Menu& otro);
    ~Menu();

    void ejecutar();

    void opcionCargar              ();
    void opcionConformarGrupos     ();
    void opcionSimularGrupos       ();
    void opcionTransicionR16       ();
    void opcionSimularEliminatorias();
    void opcionEstadisticas        ();
    void opcionPersistir           ();
};
#endif
