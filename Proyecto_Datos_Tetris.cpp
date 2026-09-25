#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include "TipoPieza.h"
#include "ColaPieza.h"
#include "Tablero.h"
#include "TablaPuntaje.h"

int main() {

    //Semilla para generar números aleatorios 
    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow ventana(sf::VideoMode({ 650, 600 }), "Tetris - Etapa 1");
    ventana.setFramerateLimit(60); 
    Tablero tablero;
    ColaPieza colaPieza;
    colaPieza.generarBolsa();
    Pieza piezaActual = colaPieza.desencolar();
    TablaPuntaje tabla;

    //Permite los textos en la interfas
    sf::Font fuente;
    if (!fuente.openFromFile("ARLRDBD.ttf")) {
        std::cout << "ADVERTENCIA: No se encontro el archivo arial.ttf en la carpeta.\n";
    }

    //Puntaje en tiempo real
    sf::Text textoPuntaje(fuente);
    textoPuntaje.setString("Puntaje: 0");
    textoPuntaje.setCharacterSize(24);
    textoPuntaje.setFillColor(sf::Color::White);
    textoPuntaje.setPosition({ 400.0f, 50.0f });

    //Texto para el top 10
    sf::Text tituloRanking(fuente);
    tituloRanking.setString("TOP 10 MEJORES");
    tituloRanking.setCharacterSize(20);
    tituloRanking.setFillColor(sf::Color::Yellow);
    tituloRanking.setPosition({ 400.0f, 130.0f });
    sf::Text textoRanking(fuente);
    textoRanking.setString(tabla.obtenerTablaString()); 
    textoRanking.setCharacterSize(16);
    textoRanking.setFillColor(sf::Color::White);
    textoRanking.setPosition({ 400.0f, 170.0f });

    // Variables de control para el tiempo y el estado del juego 
    sf::Clock reloj;
    float temporizador = 0;
    float velocidadCaida = 0.5f; 
    int puntajeActual = 0;
    bool perdio = false;

    //Ciclo que permite correr el juego
    while (ventana.isOpen()) {
        if (perdio) {
            break;
        }

        // Acumula el tiempo transcurrido desde el último frame
        float tiempo = reloj.restart().asSeconds();
        temporizador += tiempo;

        
        if (colaPieza.necesitaMasPiezas()) {
            colaPieza.generarBolsa();
        }

        
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
                        
                        tablero.fijarPieza(piezaActual);
                        int lineas = tablero.eliminarFilasCompletas();

                        if (lineas > 0) {
                            if (lineas == 1) puntajeActual += 100;
                            else if (lineas == 2) puntajeActual += 300;
                            else if (lineas == 3) puntajeActual += 500;
                            else if (lineas == 4) puntajeActual += 800;

                            textoPuntaje.setString("Puntaje: " + std::to_string(puntajeActual));
                        }

                        piezaActual = colaPieza.desencolar(); 

                        
                        if (tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y, piezaActual.rotacion)) {
                            perdio = true;
                        }
                    }
                }
            }
        }

       
        if (temporizador > velocidadCaida) {
            if (!tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y + 1, piezaActual.rotacion)) {
                piezaActual.y++; 
            }
            else {
                // Si choca al caer automáticamente, se "pega" al tablero
                tablero.fijarPieza(piezaActual);
                int lineas = tablero.eliminarFilasCompletas(); // Verifica y elimina líneas llenas (Nodos de la lista enlazada)

                // Actualiza puntaje según la cantidad de líneas eliminadas
                if (lineas > 0) {
                    if (lineas == 1) puntajeActual += 100;
                    else if (lineas == 2) puntajeActual += 300;
                    else if (lineas == 3) puntajeActual += 500;
                    else if (lineas == 4) puntajeActual += 800;

                    textoPuntaje.setString("Puntaje: " + std::to_string(puntajeActual)); // Refresca el texto en pantalla
                }

                // Extrae nueva pieza y verifica Game Over
                piezaActual = colaPieza.desencolar();
                if (tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y, piezaActual.rotacion)) {
                    perdio = true;
                }
            }
            temporizador = 0; // Reinicia el contador de gravedad
        }

        // 6. RENDERIZADO (DIBUJO EN PANTALLA)
        ventana.clear(sf::Color::Black); // Limpia el frame anterior

        // Plantilla para dibujar celdas cuadradas de 28x28 pixeles con borde
        sf::RectangleShape bloque({ 28.0f, 28.0f });
        bloque.setOutlineThickness(1.0f);
        bloque.setOutlineColor(sf::Color(50, 50, 50));

        // Recorre y dibuja el mapa del tablero (Piezas ya fijadas)
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

        // Recorre la matriz de 4x4 de la pieza actual en caída y la dibuja sobre el mapa
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                // FORMAPIEZA es la matriz 3D o arreglo constante donde guardas las orientaciones
                if (FORMAPIEZA[piezaActual.tipo][piezaActual.rotacion][i][j] != 0) {
                    bloque.setPosition({ (piezaActual.x + j) * 30.0f + 50.0f, (piezaActual.y + i) * 30.0f });
                    bloque.setFillColor(sf::Color::Green);
                    ventana.draw(bloque);
                }
            }
        }

        // Dibuja los textos de la interfaz gráfica a la derecha
        ventana.draw(textoPuntaje);
        ventana.draw(tituloRanking);
        ventana.draw(textoRanking);

        ventana.display(); // Muestra en pantalla todo lo que se dibujó en este frame
    }

    // 7. LÓGICA DE FIN DE JUEGO (PERSISTENCIA Y ORDENAMIENTO)
    if (perdio) {
        std::cout << "\n=== GAME OVER ===\nTu puntaje final es: " << puntajeActual << "\n";

        // Verifica si los puntos logrados superan al último lugar del arreglo guardado
        if (tabla.calificaEnTop10(puntajeActual)) {
            std::string nombreJugador;
            int opcionSort;

            std::cout << "\n Entraste al top 10 de los mejores jugadores \n";
            std::cout << "Escribe tu nombre(sin espacios): ";
            std::cin >> nombreJugador; // Captura en consola

            // Permite cumplir el requisito del proyecto de elegir el método de ordenamiento (O(n^2) vs O(n log n))
            std::cout << "\n¿Que algoritmo usar?\n1. Insertion Sort\n2. Quicksort\nOpcion: ";
            std::cin >> opcionSort;

            tabla.registrarPuntaje(nombreJugador, puntajeActual, opcionSort);

            // Refresca visualmente el objeto de texto para la memoria (opcional)
            textoRanking.setString(tabla.obtenerTablaString());
            std::cout << "\n¡Puntaje guardado correctamente en puntajes.txt!\n";
        }
        else {
            std::cout << "No lograste entrar al Top 10. ¡Sigue practicando!\n";
        }

        // Cierra la ventana gráfica al terminar de utilizar la consola de comandos
        ventana.close();
    }

    return 0;
}