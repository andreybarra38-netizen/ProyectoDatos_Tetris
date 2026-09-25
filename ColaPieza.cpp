#include "ColaPieza.h"
#include <cstdlib>

ColaPieza::ColaPieza() : frente(nullptr), final(nullptr), cantidad(0) {}

ColaPieza::~ColaPieza() {
    while (frente != nullptr) {
        desencolar();
    }
}

void ColaPieza::encolar(Pieza p) {
    NodoCola* nuevo = new NodoCola{ p, nullptr };
    if (final == nullptr) {
        frente = final = nuevo;
    }
    else {
        final->siguiente = nuevo;
        final = nuevo;
    }
    cantidad++;
}

Pieza ColaPieza::desencolar() {
    if (frente == nullptr) return Pieza{ -1, 0, 0, 0 };

    NodoCola* temp = frente;
    Pieza p = temp->pieza;
    frente = frente->siguiente;
    if (frente == nullptr) final = nullptr;

    delete temp;
    cantidad--;
    return p;
}

void ColaPieza::generarBolsa() {
    int bolsa[7] = { 0, 1, 2, 3, 4, 5, 6 }; // Los 7 tipos de tetrominós

    // Algoritmo de mezcla (Fisher-Yates)
    for (int i = 6; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = bolsa[i];
        bolsa[i] = bolsa[j];
        bolsa[j] = temp;
    }

    // Encolar las 7 piezas ajustando sus coordenadas iniciales
    for (int i = 0; i < 7; i++) {
        // x = 3, y = 0 para posicionarla centrada en la parte superior del tablero de 10 columnas
        Pieza nueva = { bolsa[i], 0, 3, 0 };
        encolar(nueva);
    }
}

bool ColaPieza::necesitaMasPiezas() {
    return cantidad <= 3; // Mantiene la cola llena para ver las siguientes piezas
}

void ColaPieza::verProximasTres(Pieza arregloSalida[3]) {
    NodoCola* actual = frente;
    int contador = 0;

    //Recorre la cola para optener las proximas 3 piezas
    while (actual != nullptr && contador < 3) {
        arregloSalida[contador] = actual->pieza;
        actual = actual->siguiente;
        contador++;
    }
}