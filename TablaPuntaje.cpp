#include "TablaPuntaje.h"

TablaPuntaje::TablaPuntaje() {
    cantidadRegistros = 0;
    cargarPuntajes();
}

void TablaPuntaje::cargarPuntajes() {
    std::ifstream archivo("puntajes.txt");
    if (archivo.is_open()) {
        std::string nom;
        int pts;
        while (archivo >> nom >> pts && cantidadRegistros < 10) {
            mejores[cantidadRegistros].nombre = nom;
            mejores[cantidadRegistros].puntaje = pts;
            cantidadRegistros++;
        }
        archivo.close();
    }
}

void TablaPuntaje::guardarPuntajes() {
    std::ofstream archivo("puntajes.txt");
    if (archivo.is_open()) {
        for (int i = 0; i < cantidadRegistros; i++) {
            archivo << mejores[i].nombre << " " << mejores[i].puntaje << "\n";
        }
        archivo.close();
    }
}

bool TablaPuntaje::calificaEnTop10(int puntaje) {
    if (cantidadRegistros < 10) return true;
    return puntaje > mejores[cantidadRegistros - 1].puntaje;
}

void TablaPuntaje::registrarPuntaje(std::string nombre, int puntaje, int opcionOrdenamiento) {
    if (cantidadRegistros == 10) {
        mejores[9].nombre = nombre;
        mejores[9].puntaje = puntaje;
    }
    else {
        mejores[cantidadRegistros].nombre = nombre;
        mejores[cantidadRegistros].puntaje = puntaje;
        cantidadRegistros++;
    }

    if (opcionOrdenamiento == 1) {
        ordenarPorInsercion();
    }
    else {
        ordenarPorQuicksort();
    }

    guardarPuntajes();
}

void TablaPuntaje::ordenarPorInsercion() {
    for (int i = 1; i < cantidadRegistros; i++) {
        HistorialPuntaje clave = mejores[i];
        int j = i - 1;

        while (j >= 0 && mejores[j].puntaje < clave.puntaje) {
            mejores[j + 1] = mejores[j];
            j = j - 1;
        }
        mejores[j + 1] = clave;
    }
}

void TablaPuntaje::quicksortRecursivo(int inicio, int fin) {
    if (inicio >= fin) return;

    int pivote = mejores[fin].puntaje;
    int i = inicio - 1;

    for (int j = inicio; j < fin; j++) {
        if (mejores[j].puntaje > pivote) {
            i++;
            HistorialPuntaje temp = mejores[i];
            mejores[i] = mejores[j];
            mejores[j] = temp;
        }
    }

    HistorialPuntaje temp = mejores[i + 1];
    mejores[i + 1] = mejores[fin];
    mejores[fin] = temp;

    int pi = i + 1;
    quicksortRecursivo(inicio, pi - 1);
    quicksortRecursivo(pi + 1, fin);
}

void TablaPuntaje::ordenarPorQuicksort() {
    quicksortRecursivo(0, cantidadRegistros - 1);
}

void TablaPuntaje::mostrarTabla() {
    std::cout << "\n=== TOP 10 MEJORES PUNTAJES ===\n";
    for (int i = 0; i < cantidadRegistros; i++) {
        std::cout << i + 1 << ". " << mejores[i].nombre << " - " << mejores[i].puntaje << " pts\n";
    }
    std::cout << "===============================\n";
}

std::string TablaPuntaje::obtenerTablaString() {
    std::string resultado = "";
    for (int i = 0; i < cantidadRegistros; i++) {
        resultado += std::to_string(i + 1) + ". " + mejores[i].nombre + " - " + std::to_string(mejores[i].puntaje) + " pts\n";
    }
    return resultado;
}