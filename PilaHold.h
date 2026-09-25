#ifndef PILAHOLD_H
#define PILAHOLD_H

#include "TipoPieza.h"

struct NodoPila {
    Pieza pieza;
    NodoPila* siguiente;
};

class PilaHold {
private:
    NodoPila* tope;
    bool llena;

public:
    PilaHold();
    ~PilaHold();

    void apilar(Pieza p);
    Pieza desapilar();
    bool estaLlena();
    Pieza verTope(); 
};

#endif

