#pragma once

#ifndef TABLERO_H
#define TABLERO_H

#include "TipoPieza.h"

struct NodoFila {
    int celdas[10]; //indica que hay en cada celda ya sea =0 (vacia) o !=0 (ocupado)
    NodoFila* siguiente;
};

class Tablero {
private:
    NodoFila* cabeza; 

   
    NodoFila* obtenerFila(int y);

public:
    Tablero();
    ~Tablero();
    void crearTableroVacio();
    bool hayColision(Pieza p, int nuevoX, int nuevoY, int nuevaRotacion);
    void fijarPieza(Pieza p);
    bool rotarPieza(Pieza& p);
    int obtenerCelda(int x, int y);
    void insertarFilaVacia();
    int eliminarFilasCompletas(); 
};

#endif