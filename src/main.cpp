#include "../include/Menu.h"

// La semilla aleatoria se inicializa dentro de Simulador
// usando std::random_device
int main() {
    Menu menu("data/selecciones_clasificadas_mundial.csv",
              "data/jugadores.dat");
    menu.ejecutar();
    return 0;
}
