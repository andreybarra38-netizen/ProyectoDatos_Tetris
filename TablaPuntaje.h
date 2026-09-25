#ifndef TABLAPUNTAJE_H
#define TABLAPUNTAJE_H

#include "TipoPieza.h"
#include <iostream>
#include <fstream>
#include <string>

class TablaPuntaje {
private:
    HistorialPuntaje mejores[10];
    int cantidadRegistros;

    void cargarPuntajes();
    void guardarPuntajes();
    void quicksortRecursivo(int inicio, int fin);

public:
    TablaPuntaje();

    bool calificaEnTop10(int puntaje);
    void registrarPuntaje(std::string nombre, int puntaje, int opcionOrdenamiento);

    void ordenarPorInsercion();
    void ordenarPorQuicksort();

    void mostrarTabla();
    std::string obtenerTablaString();
};

#endif

