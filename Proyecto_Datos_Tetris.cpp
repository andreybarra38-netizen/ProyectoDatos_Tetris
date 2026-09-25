#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include "TipoPieza.h"
#include "ColaPieza.h"
#include "Tablero.h"
#include "TablaPuntaje.h"
#include "PilaHold.h"    
#include "ListaReplay.h" 
#include "Evento.h"
#include "ColaEvento.h"

int main() {
    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow ventana(sf::VideoMode({ 750, 600 }), "Tetris - Etapa 1");
    ventana.setFramerateLimit(60);

    Tablero tablero;
    ColaPieza colaPieza;
    TablaPuntaje tabla;
    PilaHold pilaHold;
    ListaReplay listaReplay;
    ColaEvento colaEvento;

    // Primer evento de bomba a los 45 segundos
    colaEvento.insertarOrdenado({ 45.0f, PIEZA_ESPECIAL_BOMBA, 0 });

    colaPieza.generarBolsa();
    Pieza piezaActual = colaPieza.desencolar();

    bool yaUsoHold = false;

    sf::Font fuente;
    if (!fuente.openFromFile("ARLRDBD.ttf")) {
        std::cout << "ADVERTENCIA: No se encontro el archivo ARLRDBD.ttf en la carpeta.\n";
        return -1;
    }

    sf::Text textoPuntaje(fuente, "Puntaje: 0", 24);
    textoPuntaje.setFillColor(sf::Color::White);
    textoPuntaje.setPosition({ 400.0f, 50.0f });

    sf::Text textoHold(fuente, "En espera (C)", 20);
    textoHold.setFillColor(sf::Color::Yellow);
    textoHold.setPosition({ 600.0f, 50.0f });

    sf::Text textoNext(fuente, "Piezas Siguientes", 20);
    textoNext.setFillColor(sf::Color::Yellow);
    textoNext.setPosition({ 600.0f, 200.0f });

    sf::Text tituloRanking(fuente, "TOP 10 MEJORES", 20);
    tituloRanking.setFillColor(sf::Color::Yellow);
    tituloRanking.setPosition({ 400.0f, 130.0f });

    sf::Text textoRanking(fuente, tabla.obtenerTablaString(), 16);
    textoRanking.setFillColor(sf::Color::White);
    textoRanking.setPosition({ 400.0f, 170.0f });

    sf::Clock reloj;
    sf::Clock relojJuegoTotal;
    float temporizador = 0;
    float temporizadorSinLineas = 0.0f; // Controla la inactividad de 25 segundos
    float velocidadCaida = 0.5f;
    int puntajeActual = 0;
    bool perdio = false;

    listaReplay.registrarMovimiento(piezaActual, tablero, puntajeActual);

    auto aplicarEstado = [&]( EstadoJuego& estado) {
        piezaActual = estado.piezaActual;
        tablero.restaurarDesdeSnapshot(estado.tableroSnapshot);
        puntajeActual = estado.puntaje;
        textoPuntaje.setString("Puntaje: " + std::to_string(puntajeActual));
        temporizador = 0;
        };

    // Procesa el bloqueo de piezas, eliminación de líneas, cálculo de puntaje y colisión final
    auto fijarYProcesarPieza = [&](bool& movimientoExitoso) {
        tablero.fijarPieza(piezaActual);
        int lineas = tablero.eliminarFilasCompletas();

        if (lineas > 0) {
            temporizadorSinLineas = 0.0f; // Reinicia el contador de 25s
            if (velocidadCaida == 0.2f) {
                velocidadCaida = 0.5f;
                std::cout << "VELOCIDAD RESTAURADA Has eliminado lineas a tiempo\n";
            }

            if (lineas == 1) puntajeActual += 100;
            else if (lineas == 2) puntajeActual += 300;
            else if (lineas == 3) {
                puntajeActual += 1000;
                std::cout << "BONO DE PUNTOS Eliminaste 3 filas a la vez (+500 pts extra)\n";
            }
            else if (lineas == 4) puntajeActual += 800;

            textoPuntaje.setString("Puntaje: " + std::to_string(puntajeActual));
        }

        piezaActual = colaPieza.desencolar();
        yaUsoHold = false;
        movimientoExitoso = true;

        if (tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y, piezaActual.rotacion)) {
            perdio = true;
        }
        };

    // Renderiza celdas fijadas del tablero y la pieza que cae
    sf::RectangleShape bloque({ 28.0f, 28.0f });
    bloque.setOutlineThickness(1.0f);
    bloque.setOutlineColor(sf::Color(50, 50, 50));

    auto dibujarTableroYPiezaActual = [&]() {
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 10; x++) {
                if (tablero.obtenerCelda(x, y) != 0) {
                    bloque.setPosition({ x * 30.0f + 50.0f, y * 30.0f });
                    bloque.setFillColor(sf::Color::Cyan);
                    ventana.draw(bloque);
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (FORMAPIEZA[piezaActual.tipo][piezaActual.rotacion][i][j] != 0) {
                    bloque.setPosition({ (piezaActual.x + j) * 30.0f + 50.0f, (piezaActual.y + i) * 30.0f });
                    bloque.setFillColor(sf::Color::Green);
                    ventana.draw(bloque);
                }
            }
        }
        };

    // --- BUCLE PRINCIPAL DEL JUEGO ---
    while (ventana.isOpen() && !perdio) {
        float tiempo = reloj.restart().asSeconds();
        temporizador += tiempo;
        temporizadorSinLineas += tiempo;
        float tiempoTotalPartida = relojJuegoTotal.getElapsedTime().asSeconds();

        // Incremento de velocidad tras 25 segundos sin eliminar filas
        if (temporizadorSinLineas >= 25.0f && velocidadCaida != 0.2f) {
            velocidadCaida = 0.2f;
            std::cout << "EVENTO VELOCIDAD - " << static_cast<int>(tiempoTotalPartida)
                << " Llevas 25 segundos sin eliminar lineas se aumenta la velocidad\n";
        }

        // Evento bomba cada 45 segundos
        while (!colaEvento.estaVacia() && tiempoTotalPartida >= colaEvento.verFrente().tiempoDisparo) {
            Evento eventoActual = colaEvento.verFrente();
            colaEvento.extraerFrente();

            if (eventoActual.tipo == PIEZA_ESPECIAL_BOMBA) {
                for (int y = 19; y >= 2; y--) {
                    for (int x = 0; x < 10; x++) {
                        tablero.modificarCelda(x, y, tablero.obtenerCelda(x, y - 2));
                    }
                }
                for (int y = 0; y < 2; y++) {
                    for (int x = 0; x < 10; x++) {
                        tablero.modificarCelda(x, y, 0);
                    }
                }
                std::cout << "EVENTO ACTIVADO - " << static_cast<int>(tiempoTotalPartida)
                    << " BOMBA ACTIVADA Se destruyeron las 2 filas inferiores.\n";

                colaEvento.insertarOrdenado({ eventoActual.tiempoDisparo + 45.0f, PIEZA_ESPECIAL_BOMBA, 0 });
            }
        }

        if (colaPieza.necesitaMasPiezas()) {
            colaPieza.generarBolsa();
        }

        bool movimientoExitoso = false;

        while (const auto evento = ventana.pollEvent()) {
            if (evento->is<sf::Event::Closed>()) {
                ventana.close();
            }
            else if (const auto* teclaPresionada = evento->getIf<sf::Event::KeyPressed>()) {
                if (teclaPresionada->code == sf::Keyboard::Key::Left) {
                    if (!tablero.hayColision(piezaActual, piezaActual.x - 1, piezaActual.y, piezaActual.rotacion)) {
                        piezaActual.x--;
                        movimientoExitoso = true;
                    }
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::Right) {
                    if (!tablero.hayColision(piezaActual, piezaActual.x + 1, piezaActual.y, piezaActual.rotacion)) {
                        piezaActual.x++;
                        movimientoExitoso = true;
                    }
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::Up) {
                    tablero.rotarPieza(piezaActual);
                    movimientoExitoso = true;
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::Down) {
                    if (!tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y + 1, piezaActual.rotacion)) {
                        piezaActual.y++;
                        movimientoExitoso = true;
                    }
                    else {
                        fijarYProcesarPieza(movimientoExitoso);
                    }
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::C && !yaUsoHold) {
                    Pieza aGuardar = piezaActual;
                    aGuardar.x = 3; aGuardar.y = 0; aGuardar.rotacion = 0;

                    if (!pilaHold.estaLlena()) {
                        piezaActual = colaPieza.desencolar();
                    }
                    else {
                        piezaActual = pilaHold.desapilar();
                    }
                    pilaHold.apilar(aGuardar);

                    yaUsoHold = true;
                    temporizador = 0;
                    movimientoExitoso = true;
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::Z) {
                    EstadoJuego anterior;
                    if (listaReplay.deshacer(anterior)) {
                        aplicarEstado(anterior);
                    }
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::Y) {
                    EstadoJuego siguiente;
                    if (listaReplay.rehacer(siguiente)) {
                        aplicarEstado(siguiente);
                    }
                }
            }
        }

        if (temporizador > velocidadCaida) {
            if (!tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y + 1, piezaActual.rotacion)) {
                piezaActual.y++;
                movimientoExitoso = true;
            }
            else {
                fijarYProcesarPieza(movimientoExitoso);
            }
            temporizador = 0;
        }

        if (movimientoExitoso) {
            listaReplay.registrarMovimiento(piezaActual, tablero, puntajeActual);
        }

        ventana.clear(sf::Color::Black);

        dibujarTableroYPiezaActual();

        if (pilaHold.estaLlena()) {
            Pieza pHold = pilaHold.verTope();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (FORMAPIEZA[pHold.tipo][0][i][j] != 0) {
                        bloque.setPosition({ j * 20.0f + 600.0f, i * 20.0f + 90.0f });
                        bloque.setFillColor(sf::Color(150, 150, 150));
                        ventana.draw(bloque);
                    }
                }
            }
        }

        Pieza proximas[3];
        colaPieza.verProximasTres(proximas);

        float offsetY = 240.0f;
        for (int k = 0; k < 3; k++) {
            Pieza p = proximas[k];
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (FORMAPIEZA[p.tipo][0][i][j] != 0) {
                        bloque.setPosition({ j * 20.0f + 600.0f, i * 20.0f + offsetY });
                        bloque.setFillColor(sf::Color::Magenta);
                        ventana.draw(bloque);
                    }
                }
            }
            offsetY += 90.0f;
        }

        ventana.draw(textoPuntaje);
        ventana.draw(tituloRanking);
        ventana.draw(textoRanking);
        ventana.draw(textoHold);
        ventana.draw(textoNext);

        ventana.display();
    }

    // Modo replay y fin del juego
    if (perdio) {
        std::cout << "\n=== GAME OVER ===\n";
        std::cout << "Entrando en modo REPLAY en la ventana grafica.\n";
        std::cout << "- Usa FLECHA IZQUIERDA para retroceder en el historial.\n";
        std::cout << "- Usa FLECHA DERECHA para avanzar en el historial.\n";
        std::cout << "- Presiona ENTER para salir y registrar tu puntaje.\n";

        bool enModoReplay = true;
        while (ventana.isOpen() && enModoReplay) {
            while (const auto evento = ventana.pollEvent()) {
                if (evento->is<sf::Event::Closed>()) {
                    ventana.close();
                    enModoReplay = false;
                }
                else if (const auto* teclaPresionada = evento->getIf<sf::Event::KeyPressed>()) {
                    if (teclaPresionada->code == sf::Keyboard::Key::Right) {
                        EstadoJuego siguiente;
                        if (listaReplay.rehacer(siguiente)) {
                            aplicarEstado(siguiente);
                        }
                    }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Left) {
                        EstadoJuego anterior;
                        if (listaReplay.deshacer(anterior)) {
                            aplicarEstado(anterior);
                        }
                    }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Enter) {
                        enModoReplay = false;
                    }
                }
            }

            ventana.clear(sf::Color::Black);

            dibujarTableroYPiezaActual();

            sf::Text textoReplayInfo(fuente, "MODO REPLAY: <- Izq / Der -> (Enter para salir)", 14);
            textoReplayInfo.setFillColor(sf::Color::Yellow);
            textoReplayInfo.setPosition({ 380.0f, 10.0f });

            ventana.draw(textoReplayInfo);
            ventana.draw(textoPuntaje);
            ventana.draw(tituloRanking);
            ventana.draw(textoRanking);

            ventana.display();
        }

        std::cout << "\n Tu puntaje final es: " << puntajeActual << "\n";

        if (tabla.calificaEnTop10(puntajeActual)) {
            std::string nombreJugador;
            int opcionSort;

            std::cout << "\n Entraste al top 10 de los mejores jugadores \n";
            std::cout << "Escribe tu nombre (sin espacios): ";
            std::cin >> nombreJugador;

            std::cout << "\n Que algoritmo usar \n 1.Insertion Sort \n 2.Quicksort \n Opcion: ";
            std::cin >> opcionSort;
            tabla.registrarPuntaje(nombreJugador, puntajeActual, opcionSort);
            std::cout << "\n Tu puntuacion se ha guardado correctamente \n";
        }
        else {
            std::cout << " No lograste entrar al Top 10 ¡Sigue practicando! \n";
        }
        ventana.close();
    }

    return 0;
}