#ifndef COLA_EVENTOS_H
#define COLA_EVENTOS_H

#include "Evento.h"

class ColaEvento {
private:
    NodoColaEventos* frente;

public:
    ColaEvento();
    ~ColaEvento();
    void insertarOrdenado(Evento nuevoEvento);
    bool estaVacia() const;
    Evento verFrente() const;
    void extraerFrente();
};

#endif