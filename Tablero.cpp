#include "Tablero.h"

Tablero::Tablero() : cabeza(nullptr) {
    crearTableroVacio();
}

Tablero::~Tablero() {//Libera memoria 
    NodoFila* actual = cabeza;
    while (actual != nullptr) {
        NodoFila* sig = actual->siguiente;
        delete actual;
        actual = sig;
    }
}

void Tablero::crearTableroVacio() {
    /* Crear exactamente 20 filas con 10 celdas (Lista enlazada de 20 nodos y cada nodo es un array de n=10)*/
    for (int i = 0; i < 20; i++) {
        NodoFila* nuevaFila = new NodoFila();
        for (int j = 0; j < 10; j++) {
            nuevaFila->celdas[j] = 0;
        }

        nuevaFila->siguiente = cabeza;
        cabeza = nuevaFila;
    }
}

//Metodos para encontrar coordenadas
NodoFila* Tablero::obtenerFila(int y) {
    if (y < 0 || y >= 20) return nullptr;

    NodoFila* actual = cabeza;
    for (int i = 0; i < y && actual != nullptr; i++) {
        actual = actual->siguiente;
    }
    return actual;
}

int Tablero::obtenerCelda(int x, int y) {
    NodoFila* fila = obtenerFila(y);
    if (fila != nullptr && x >= 0 && x < 10) {
        return fila->celdas[x];
    }
    return 0;
}

bool Tablero::hayColision(Pieza p, int nuevoX, int nuevoY, int nuevaRotacion) {
    // Recorre la cuadricula 4x4 de cada figura
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // Convierte/adapta/ubica la posicion de la cuadricula 4x4 al tablero
            if (FORMAPIEZA[p.tipo][nuevaRotacion][i][j] != 0) {
                int tableroX = nuevoX + j;
                int tableroY = nuevoY + i;

                //Colision con el tablero
                if (tableroX < 0 || tableroX >= 10 || tableroY >= 20) {
                    return true;
                }

                //Translape entre piezas 
                if (tableroY >= 0) {
                    NodoFila* fila = obtenerFila(tableroY);
                    if (fila != nullptr && fila->celdas[tableroX] != 0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void Tablero::fijarPieza(Pieza p) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (FORMAPIEZA[p.tipo][p.rotacion][i][j] != 0) {
                int tableroX = p.x + j;
                int tableroY = p.y + i;

                if (tableroY >= 0 && tableroY < 20) {
                    NodoFila* fila = obtenerFila(tableroY);
                    if (fila != nullptr) {
                        fila->celdas[tableroX] = p.tipo + 1;
                    }
                }
            }
        }
    }
}

bool Tablero::rotarPieza(Pieza& p) {
    int siguienteRotacion = (p.rotacion + 1) % 4; //Rotacion de 90,180,270,360 grados

    //Rota si no hay colision
    if (!hayColision(p, p.x, p.y, siguienteRotacion)) {
        p.rotacion = siguienteRotacion;
        return true;
    }
    return false;
}

void Tablero::insertarFilaVacia() {
    NodoFila* nuevaFila = new NodoFila();
    for (int i = 0; i < 10; i++) {
        nuevaFila->celdas[i] = 0;
    }

    nuevaFila->siguiente = cabeza;
    cabeza = nuevaFila;
}

int Tablero::eliminarFilasCompletas() {
    NodoFila* actual = cabeza;
    NodoFila* anterior = nullptr;
    int lineasEliminadas = 0;

    while (actual != nullptr) {
        bool estaLlena = true;

        for (int i = 0; i < 10; i++) {
            if (actual->celdas[i] == 0) {
                estaLlena = false;
                break; //No está llena
            }
        }

        if (estaLlena) {
            NodoFila* aBorrar = actual; //Desconectar el nodo de la lista

            if (anterior == nullptr) {// Si la fila superior está llena         
                cabeza = actual->siguiente;
            }
            else {
                anterior->siguiente = actual->siguiente;
            }
            actual = actual->siguiente;
            delete aBorrar;
            insertarFilaVacia();
            lineasEliminadas++;
        }
        else {// Si no estaba llena avanza al siguiente nodo
            anterior = actual;
            actual = actual->siguiente;
        }
    }
    return lineasEliminadas;
}

void Tablero::restaurarDesdeSnapshot(int snapshot[20][10]) {
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            modificarCelda(x, y, snapshot[y][x]);
        }
    }
}

// Sobrescribe el valor de una celda específica navegando por la lista enlazada
void Tablero::modificarCelda(int x, int y, int valor) {
    if (y < 0 || y >= 20 || x < 0 || x >= 10) return; // Protección fuera de límites

    NodoFila* actual = cabeza;
    // Recorremos la lista hasta llegar a la fila 'y'
    for (int i = 0; i < y && actual != nullptr; i++) {
        actual = actual->siguiente;
    }

    // Si encontramos la fila, modificamos el valor de la celda 'x'
    if (actual != nullptr) {
        actual->celdas[x] = valor;
    }
}