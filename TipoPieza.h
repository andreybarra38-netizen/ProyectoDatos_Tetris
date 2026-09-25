#pragma once
#ifndef TIPOPIEZA_H
#define TIPOPIEZA_H
#include <string>

struct Pieza {
	int tipo;
	int rotacion;
	int x, y; // coordenadas en la matriz
};
struct HistorialPuntaje {
	std::string nombre;
	int puntaje = 0;
};

class TipoPieza
{

};
extern const int FORMAPIEZA[7][4][4][4];//Arreglo de 4 dimenciones(7 formass,4 rotaciones distintas y cuadricula 4x4)

#endif

