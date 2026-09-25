#include "ListaReplay.h"
#include "Tablero.h" 

ListaReplay::ListaReplay() : cabeza(nullptr), cola(nullptr), actual(nullptr) {}

ListaReplay::~ListaReplay() {
    NodoDoble* temp = cabeza;
    while (temp != nullptr) {
        NodoDoble* aBorrar = temp;
        temp = temp->siguiente;
        delete aBorrar;
    }
}

//Borra los nodos que están "adelante" en el tiempo si cambiaste la historia
void ListaReplay::limpiarFuturo() {
    if (actual != nullptr && actual->siguiente != nullptr) {
        NodoDoble* temp = actual->siguiente;
        while (temp != nullptr) {
            NodoDoble* aBorrar = temp;
            temp = temp->siguiente;
            delete aBorrar;
        }
        actual->siguiente = nullptr; //Nodo actual ahora es el último
        cola = actual;
    }
}

// Guarda una "foto" nueva en la lista
void ListaReplay::registrarMovimiento(Pieza pieza, Tablero& tableroRef, int puntaje) {
    limpiarFuturo(); //Elimina los movimientos que se hicieron en el futuro

    EstadoJuego nuevoEstado;
    nuevoEstado.piezaActual = pieza;
    nuevoEstado.puntaje = puntaje;

    // Tomamos foto celda por celda del tablero
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            nuevoEstado.tableroSnapshot[y][x] = tableroRef.obtenerCelda(x, y);
        }
    }

    NodoDoble* nuevo = new NodoDoble{ nuevoEstado, actual, nullptr };

    if (cabeza == nullptr) {
        cabeza = nuevo;
    }
    else if (actual != nullptr) {
        actual->siguiente = nuevo;
    }

    cola = nuevo;
    actual = nuevo; //Avanzamos en el tiempo al nodo recién creado
}

//Retrocede un movimiento
bool ListaReplay::deshacer(EstadoJuego& estadoSalida) {
    if (actual != nullptr && actual->anterior != nullptr) {
        actual = actual->anterior; // Viajamos un paso atrás
        estadoSalida = actual->estado; //Extraemos la foto del tablero en ese momento
        return true;
    }
    return false;
}

//Avanzar un movimiento deshecho
bool ListaReplay::rehacer(EstadoJuego& estadoSalida) {
    if (actual != nullptr && actual->siguiente != nullptr) {
        actual = actual->siguiente; //Viajamos un paso adelante
        estadoSalida = actual->estado; //Extraemos la foto de ese momento
        return true;
    }
    return false;
}