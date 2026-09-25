#include "PilaHold.h"

PilaHold::PilaHold() : tope(nullptr), llena(false) {}

PilaHold::~PilaHold() {
    if (llena && tope != nullptr) {
        delete tope;
    }
}

void PilaHold::apilar(Pieza p) {
    if (!llena) {
        NodoPila* nuevo = new NodoPila{ p, nullptr };
        tope = nuevo;
        llena = true;
    }
}

Pieza PilaHold::desapilar() {
    if (llena && tope != nullptr) {
        Pieza p = tope->pieza;
        NodoPila* temp = tope;
        tope = nullptr;
        delete temp;
        llena = false;
        return p;
    }
    return Pieza(); 
}

bool PilaHold::estaLlena() {
    return llena;
}

Pieza PilaHold::verTope() {
    if (llena && tope != nullptr) {
        return tope->pieza;
    }
    //Retorna una pieza vacía si no hay nada
    Pieza vacia;
    vacia.tipo = -1;
    return vacia;
}