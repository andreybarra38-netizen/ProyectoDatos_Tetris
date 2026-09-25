#ifndef LISTAREPLAY_H
#define LISTAREPLAY_H

#include "TipoPieza.h"

struct EstadoJuego {
    Pieza piezaActual;
    int tableroSnapshot[20][10]; //Copia de cómo está el tablero en este instante
    int puntaje;
};

struct NodoDoble {
    EstadoJuego estado;
    NodoDoble* anterior;
    NodoDoble* siguiente;
};

class ListaReplay {
private:
    NodoDoble* cabeza;
    NodoDoble* cola;
    NodoDoble* actual; 

    void limpiarFuturo();

public:
    ListaReplay();
    ~ListaReplay();

    void registrarMovimiento(Pieza pieza, class Tablero& tableroRef, int puntaje);
    bool deshacer(EstadoJuego& estadoSalida);
    bool rehacer(EstadoJuego& estadoSalida);

};

#endif
