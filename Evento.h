#ifndef EVENTO_H
#define EVENTO_H

enum TipoEvento {
    INCREMENTO_VELOCIDAD_INICIO = 1,
    INCREMENTO_VELOCIDAD_FIN = 2,
    PIEZA_ESPECIAL_BOMBA = 3,
    BONIFICACION_PUNTOS_INICIO = 4,
    BONIFICACION_PUNTOS_FIN = 5
};

struct Evento {
    float tiempoDisparo;
    int tipo;
    int puntosOtorgados;
};

struct NodoColaEventos {
    Evento evento;
    NodoColaEventos* siguiente;
};

#endif