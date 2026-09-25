#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "TipoPieza.h"
#include "ColaPieza.h"
#include "Tablero.h"


int main() {

    srand(static_cast<unsigned int>(time(0)));    // Inicializar la semilla para la generación aleatoria de piezas

    sf::RenderWindow ventana(sf::VideoMode({ 400, 600 }), "Tetris - Etapa 1");
    ventana.setFramerateLimit(60);

    Tablero tablero;
    ColaPieza colaPieza;

    colaPieza.generarBolsa();  // Llenar la cola inicial con la primera bolsa de 7 piezas
    Pieza piezaActual = colaPieza.desencolar();

    sf::Clock reloj;
    float temporizador = 0;
    float velocidadCaida = 0.5f; // Caída automática cada 0.5 segundos
    int puntajeActual = 0;

    while (ventana.isOpen()) {
        float tiempo = reloj.restart().asSeconds();
        temporizador += tiempo;

        if (colaPieza.necesitaMasPiezas()) { // Repone la cola si se está quedando sin piezas
            colaPieza.generarBolsa();
        }
        //captura de eventos y teclado
        while (const auto evento = ventana.pollEvent()) {
            if (evento->is<sf::Event::Closed>()) {
                ventana.close();
            }
            else if (const auto* teclaPresionada = evento->getIf<sf::Event::KeyPressed>()) {
                if (teclaPresionada->code == sf::Keyboard::Key::Left) {
                    if (!tablero.hayColision(piezaActual, piezaActual.x - 1, piezaActual.y, piezaActual.rotacion)) {
                        piezaActual.x--;
                    }
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::Right) {
                    if (!tablero.hayColision(piezaActual, piezaActual.x + 1, piezaActual.y, piezaActual.rotacion)) {
                        piezaActual.x++;
                    }
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::Up) {
                    tablero.rotarPieza(piezaActual);
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::Down) {
                    if (!tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y + 1, piezaActual.rotacion)) {
                        piezaActual.y++;
                    }
                    else {
                      
                        tablero.fijarPieza(piezaActual);//Colision con el suelo o piezas

                        int lineas = tablero.eliminarFilasCompletas();
                        if (lineas > 0) {
                           //Puntaje
                        }

                        piezaActual = colaPieza.desencolar();

                        // Validar condición de Game Over
                        if (tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y, piezaActual.rotacion)) {
                            ventana.close();
                        }
                    }
                }
            }
        }

        //Caida de piezas por temporizador
        if (temporizador > velocidadCaida) {
            if (!tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y + 1, piezaActual.rotacion)) {
                piezaActual.y++;
            }
            else {
                tablero.fijarPieza(piezaActual);

                int lineas = tablero.eliminarFilasCompletas();
                if (lineas > 0) {
                    //Puntaje
                }

                piezaActual = colaPieza.desencolar();

                //Validar condición de Game Over
                if (tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y, piezaActual.rotacion)) {
                    ventana.close();
                }
            }
            temporizador = 0;
        }

        //Dibujo
        ventana.clear(sf::Color::Black);

        sf::RectangleShape bloque({ 28.0f, 28.0f });
        bloque.setOutlineThickness(1.0f);
        bloque.setOutlineColor(sf::Color(50, 50, 50));

        //Dibuja los nodos fijados al tablero
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 10; x++) {
                int celda = tablero.obtenerCelda(x, y);
                if (celda != 0) {
                    bloque.setPosition({ x * 30.0f + 50.0f, y * 30.0f });
                    bloque.setFillColor(sf::Color::Cyan);
                    ventana.draw(bloque);
                }
            }
        }

        //Dibuja la pieza cayendo 
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (FORMAPIEZA[piezaActual.tipo][piezaActual.rotacion][i][j] != 0) {
                    bloque.setPosition({ (piezaActual.x + j) * 30.0f + 50.0f, (piezaActual.y + i) * 30.0f });
                    bloque.setFillColor(sf::Color::Green);
                    ventana.draw(bloque);
                }
            }
        }

        ventana.display();
    }

    return 0;
}