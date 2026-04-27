#include "../include/Menu.h"
#include <iostream>
#include <limits>

// -- Constructores ----------------------------------------------------

Menu::Menu()
    : archivoCSV("data/selecciones_clasificadas_mundial.csv"),
      archivoJugadores("data/jugadores.dat") {}

Menu::Menu(const std::string& csv, const std::string& jugadores)
    : archivoCSV(csv), archivoJugadores(jugadores) {}

Menu::Menu(const Menu& otro)
    : torneo          (otro.torneo),
      archivoCSV      (otro.archivoCSV),
      archivoJugadores(otro.archivoJugadores) {}

Menu::~Menu() {}

// -- Helpers privados -------------------------------------------------

/// Imprime las opciones del menu principal
void Menu::mostrarOpciones() const {
    std::cout << "\n╔══════════════════════════════════════╗\n";
    std::cout <<   "║        UdeAWorldCup 2026             ║\n";
    std::cout <<   "╠══════════════════════════════════════╣\n";
    std::cout <<   "║ 1. Cargar datos (CSV)                ║\n";
    std::cout <<   "║ 2. Conformar grupos (sorteo)         ║\n";
    std::cout <<   "║ 3. Simular fase de grupos            ║\n";
    std::cout <<   "║ 4. Transicion a R16                  ║\n";
    std::cout <<   "║ 5. Simular fase eliminatoria         ║\n";
    std::cout <<   "║ 6. Estadisticas finales              ║\n";
    std::cout <<   "║ 7. Guardar datos actualizados        ║\n";
    std::cout <<   "║ 0. Salir                             ║\n";
    std::cout <<   "╚══════════════════════════════════════╝\n";
    std::cout <<   "Opcion: ";
}

/// Verifica que los datos del CSV ya fueron cargados
bool Menu::datosListos() const {
    if (torneo.getEquipos().getTam() == 0) {
        std::cout << "[!] Primero debe cargar los datos (opcion 1).\n";
        return false;
    }
    return true;
}

/// Verifica que los grupos ya fueron conformados
bool Menu::gruposListos() const {
    if (torneo.getGrupos().getTam() == 0) {
        std::cout << "[!] Primero debe conformar los grupos (opcion 2).\n";
        return false;
    }
    return true;
}

/// Verifica que la fase de grupos ya fue simulada
bool Menu::gruposSimulados() const {
    if (torneo.getGrupos().getTam() == 0 ||
        !torneo.getGrupos()[0].getPartidos()[0].isSimulado()) {
        std::cout << "[!] Primero debe simular la fase de grupos (opcion 3).\n";
        return false;
    }
    return true;
}

/// Verifica que R16 ya fue configurado
bool Menu::r16Listo() const {
    if (torneo.getEtapas().getTam() == 0) {
        std::cout << "[!] Primero debe hacer la transicion a R16 (opcion 4).\n";
        return false;
    }
    return true;
}

// -- Opciones del menu ------------------------------------------------

/// Opcion 1: carga el CSV y genera plantillas artificiales
/// Si existe archivo de jugadores previo, carga esas plantillas
void Menu::opcionCargar() {
    torneo.cargarDatos(archivoCSV);
    torneo.cargarPlantillas(archivoJugadores);
    std::cout << "[OK] Datos cargados.\n";
}

/// Opcion 2: arma bombos y realiza el sorteo de grupos
void Menu::opcionConformarGrupos() {
    if (!datosListos()) return;
    torneo.conformarGrupos();
    std::cout << "[OK] Grupos conformados.\n";
}

/// Opcion 3: simula los 72 partidos de la fase de grupos
void Menu::opcionSimularGrupos() {
    if (!datosListos() || !gruposListos()) return;
    torneo.simularFaseGrupos();
    std::cout << "[OK] Fase de grupos simulada.\n";
}

/// Opcion 4: construye tablas, imprime clasificacion y configura R16
void Menu::opcionTransicionR16() {
    if (!datosListos() || !gruposListos() || !gruposSimulados()) return;
    torneo.transicionR16();
    std::cout << "[OK] Transicion a R16 completada.\n";
}

/// Opcion 5: simula R16, R8, QF, SF, 3er puesto y Final
void Menu::opcionSimularEliminatorias() {
    if (!datosListos() || !gruposListos() || !gruposSimulados() || !r16Listo()) return;
    torneo.simularFaseEliminatoria();
    std::cout << "[OK] Fase eliminatoria simulada.\n";
}

/// Opcion 6: imprime estadisticas finales del torneo
void Menu::opcionEstadisticas() {
    if (!datosListos()) return;
    torneo.generarEstadisticas();
}

/// Opcion 7: guarda las plantillas actualizadas en archivo binario
void Menu::opcionPersistir() {
    if (!datosListos()) return;
    torneo.persistirDatos(archivoJugadores);
    std::cout << "[OK] Datos guardados en " << archivoJugadores << "\n";
}

// -- Loop principal ---------------------------------------------------

/// Ejecuta el loop principal hasta que el usuario elija salir
void Menu::ejecutar() {
    int opcion = -1;
    while (opcion != 0) {
        mostrarOpciones();
        std::cin >> opcion;

        // Limpiar buffer si la entrada no es valida
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            opcion = -1;
            std::cout << "[!] Entrada invalida.\n";
            continue;
        }

        switch (opcion) {
            case 1: opcionCargar();               break;
            case 2: opcionConformarGrupos();       break;
            case 3: opcionSimularGrupos();         break;
            case 4: opcionTransicionR16();         break;
            case 5: opcionSimularEliminatorias();  break;
            case 6: opcionEstadisticas();          break;
            case 7: opcionPersistir();             break;
            case 0: std::cout << "Saliendo...\n"; break;
            default: std::cout << "[!] Opcion no valida.\n"; break;
        }
    }
}
