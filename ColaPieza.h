#pragma once
#include "TipoPieza.h"
#ifndef COLAPIEZA_H
#define COLAPIEZA_H

struct NodoCola {
    Pieza pieza;  //Guarda los datos de la pieza
    NodoCola* siguiente;
};

class ColaPieza {
private:
    NodoCola* frente;
    NodoCola* final;
    int cantidad;

public:
    ColaPieza();
    ~ColaPieza();

    void encolar(Pieza p);
    Pieza desencolar();
    void generarBolsa();         //Generar y encola piezas
    bool necesitaMasPiezas();    // Mantiene el  flujo de piezas 
    void verProximasTres(Pieza arregloSalida[3]);
};

#endif