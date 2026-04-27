#ifndef LISTA_H
#define LISTA_H

#include <fstream>

// Template de lista dinamica propia — reemplaza contenedores STL
// Usa memoria dinámica con redimensionamiento automático
template <typename T>
class Lista {
private:
    T*  datos;
    int tam;
    int cap;

    // Duplica la capacidad cuando se llena
    void redimensionar() {
        cap = (cap == 0) ? 4 : cap * 2;
        T* nuevo = new T[cap];
        for (int i = 0; i < tam; i++)
            nuevo[i] = datos[i];
        delete[] datos;
        datos = nuevo;
    }

public:
    // Constructor por defecto
    Lista() : datos(nullptr), tam(0), cap(0) {}

    // Constructor de copia — requerido por el enunciado
    Lista(const Lista<T>& otra) : datos(nullptr), tam(0), cap(0) {
        cap   = otra.cap;
        tam   = otra.tam;
        if (cap > 0) {
            datos = new T[cap];
            for (int i = 0; i < tam; i++)
                datos[i] = otra.datos[i];
        }
    }

    // Destructor
    ~Lista() {
        delete[] datos;
        datos = nullptr;
    }

    // Operador de asignación
    Lista<T>& operator=(const Lista<T>& otra) {
        if (this == &otra) return *this;
        delete[] datos;
        datos = nullptr;
        cap   = otra.cap;
        tam   = otra.tam;
        if (cap > 0) {
            datos = new T[cap];
            for (int i = 0; i < tam; i++)
                datos[i] = otra.datos[i];
        }
        return *this;
    }

    // Agrega un elemento al final
    void agregar(const T& elem) {
        if (tam == cap) redimensionar();
        datos[tam++] = elem;
    }

    // Acceso por índice
    T& operator[](int i) {
        return datos[i];
    }

    const T& operator[](int i) const {
        return datos[i];
    }

    // Tamaño actual
    int getTam() const {
        return tam;
    }

    // Elimina el elemento en la posición i
    void eliminar(int i) {
        for (int j = i; j < tam - 1; j++)
            datos[j] = datos[j + 1];
        tam--;
    }

    // Vacía la lista sin liberar capacidad
    void limpiar() {
        tam = 0;
    }
};

#endif
