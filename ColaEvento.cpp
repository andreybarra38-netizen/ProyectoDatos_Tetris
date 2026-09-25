#include "ColaEvento.h"

ColaEvento::ColaEvento() : frente(nullptr) {}

ColaEvento::~ColaEvento() {
    while (!estaVacia()) {
        extraerFrente();
    }
}

// Inserción ordenada estricta (no inserta al final ni reordena todo)
void ColaEvento::insertarOrdenado(Evento nuevoEvento) {
    NodoColaEventos* nuevo = new NodoColaEventos{ nuevoEvento, nullptr };

    // Caso: Lista vacía o el evento ocurre antes que el primero actual
    if (frente == nullptr || nuevoEvento.tiempoDisparo < frente->evento.tiempoDisparo) {
        nuevo->siguiente = frente;
        frente = nuevo;
    }
    else {
        // Caso: Buscar la posición correcta en medio o al final
        NodoColaEventos* actual = frente;
        while (actual->siguiente != nullptr && actual->siguiente->evento.tiempoDisparo <= nuevoEvento.tiempoDisparo) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }
}

bool ColaEvento::estaVacia() const {
    return frente == nullptr;
}

Evento ColaEvento::verFrente() const {
    if (frente != nullptr) {
        return frente->evento;
    }
    return Evento{ -1.0f, 0, 0 };
}

void ColaEvento::extraerFrente() {
    if (frente != nullptr) {
        NodoColaEventos* temp = frente;
        frente = frente->siguiente;
        delete temp;
    }
}