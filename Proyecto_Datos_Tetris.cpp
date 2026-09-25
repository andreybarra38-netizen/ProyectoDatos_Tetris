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

int main() {
    // Semilla para generar números aleatorios 
    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow ventana(sf::VideoMode({ 750, 600 }), "Tetris - Etapa 1");
    ventana.setFramerateLimit(60);

    Tablero tablero;
    ColaPieza colaPieza;
    TablaPuntaje tabla;
    PilaHold pilaHold;
    ListaReplay listaReplay;

    colaPieza.generarBolsa();
    Pieza piezaActual = colaPieza.desencolar();

    //Variables de control para el Hold
    bool yaUsoHold = false;

    //Permite los textos en la interfaz
    sf::Font fuente;
    if (!fuente.openFromFile("ARLRDBD.ttf")) {
        std::cout << "ADVERTENCIA: No se encontro el archivo ARLRDBD.ttf en la carpeta.\n";
        return -1;
    }

    //Puntaje en tiempo real
    sf::Text textoPuntaje(fuente);
    textoPuntaje.setString("Puntaje: 0");
    textoPuntaje.setCharacterSize(24);
    textoPuntaje.setFillColor(sf::Color::White);
    textoPuntaje.setPosition({ 400.0f, 50.0f });

    //Textos para Hold y Next
    sf::Text textoHold(fuente, "En espera (C)", 20);
    textoHold.setFillColor(sf::Color::Yellow);
    textoHold.setPosition({ 600.0f, 50.0f });

    sf::Text textoNext(fuente, "Piezas Siguientes", 20);
    textoNext.setFillColor(sf::Color::Yellow);
    textoNext.setPosition({ 600.0f, 200.0f });

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

    //Variables de control
    sf::Clock reloj;
    float temporizador = 0;
    float velocidadCaida = 0.5f;
    int puntajeActual = 0;
    bool perdio = false;

    // Registrar el primer estado del juego usando el método nativo de ListaReplay
    listaReplay.registrarMovimiento(piezaActual, tablero, puntajeActual);

    // Ciclo que permite correr el juego
    while (ventana.isOpen()) {
        if (perdio) {
            break;
        }

        float tiempo = reloj.restart().asSeconds();
        temporizador += tiempo;

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
                        yaUsoHold = false;
                        movimientoExitoso = true;

                        if (tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y, piezaActual.rotacion)) {
                            perdio = true;
                        }
                    }
                }
                //Hold C
                else if (teclaPresionada->code == sf::Keyboard::Key::C && !yaUsoHold) {
                    if (!pilaHold.estaLlena()) {
                        Pieza aGuardar = piezaActual;
                        aGuardar.x = 3; aGuardar.y = 0; aGuardar.rotacion = 0;
                        pilaHold.apilar(aGuardar);
                        piezaActual = colaPieza.desencolar();
                    }
                    else {
                        Pieza sacada = pilaHold.desapilar();
                        Pieza aGuardar = piezaActual;
                        aGuardar.x = 3; aGuardar.y = 0; aGuardar.rotacion = 0;
                        pilaHold.apilar(aGuardar);
                        piezaActual = sacada;
                    }
                    yaUsoHold = true;
                    temporizador = 0;
                    movimientoExitoso = true;
                }
                //Replay Z = Deshacer, Y = Rehacer
                else if (teclaPresionada->code == sf::Keyboard::Key::Z) {
                    EstadoJuego anterior;
                    if (listaReplay.deshacer(anterior)) {
                        piezaActual = anterior.piezaActual;
                        tablero.restaurarDesdeSnapshot(anterior.tableroSnapshot);
                        puntajeActual = anterior.puntaje;
                        textoPuntaje.setString("Puntaje: " + std::to_string(puntajeActual));
                        temporizador = 0;
                    }
                }
                else if (teclaPresionada->code == sf::Keyboard::Key::Y) {
                    EstadoJuego siguiente;
                    if (listaReplay.rehacer(siguiente)) {
                        piezaActual = siguiente.piezaActual;
                        tablero.restaurarDesdeSnapshot(siguiente.tableroSnapshot);
                        puntajeActual = siguiente.puntaje;
                        textoPuntaje.setString("Puntaje: " + std::to_string(puntajeActual));
                        temporizador = 0;
                    }
                }
            }
        }

        // Gravedad del tetris
        if (temporizador > velocidadCaida) {
            if (!tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y + 1, piezaActual.rotacion)) {
                piezaActual.y++;
                movimientoExitoso = true;
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
                yaUsoHold = false;
                movimientoExitoso = true;

                if (tablero.hayColision(piezaActual, piezaActual.x, piezaActual.y, piezaActual.rotacion)) {
                    perdio = true;
                }
            }
            temporizador = 0;
        }

        // Guardamos el historial del tablero si algo cambió
        if (movimientoExitoso) {
            listaReplay.registrarMovimiento(piezaActual, tablero, puntajeActual);
        }

        // Renderizado
        ventana.clear(sf::Color::Black);

        sf::RectangleShape bloque({ 28.0f, 28.0f });
        bloque.setOutlineThickness(1.0f);
        bloque.setOutlineColor(sf::Color(50, 50, 50));

        //Tablero
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

        //Pieza actual
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (FORMAPIEZA[piezaActual.tipo][piezaActual.rotacion][i][j] != 0) {
                    bloque.setPosition({ (piezaActual.x + j) * 30.0f + 50.0f, (piezaActual.y + i) * 30.0f });
                    bloque.setFillColor(sf::Color::Green);
                    ventana.draw(bloque);
                }
            }
        }

        //Dibujar Pieza en Hold
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

        //Dibujar Próximas 3 piezas
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

        //Dibuja los textos de la interfaz
        ventana.draw(textoPuntaje);
        ventana.draw(tituloRanking);
        ventana.draw(textoRanking);
        ventana.draw(textoHold);
        ventana.draw(textoNext);
        ventana.display();
    }

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
                            piezaActual = siguiente.piezaActual;
                            tablero.restaurarDesdeSnapshot(siguiente.tableroSnapshot);
                            puntajeActual = siguiente.puntaje;
                            textoPuntaje.setString("Puntaje: " + std::to_string(puntajeActual));
                        }
                    }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Left) {
                        EstadoJuego anterior;
                        if (listaReplay.deshacer(anterior)) {
                            piezaActual = anterior.piezaActual;
                            tablero.restaurarDesdeSnapshot(anterior.tableroSnapshot);
                            puntajeActual = anterior.puntaje;
                            textoPuntaje.setString("Puntaje: " + std::to_string(puntajeActual));
                        }
                    }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Enter) {
                        enModoReplay = false;
                    }
                }
            }

            // Renderizado del Modo Replay
            ventana.clear(sf::Color::Black);

            sf::RectangleShape bloque({ 28.0f, 28.0f });
            bloque.setOutlineThickness(1.0f);
            bloque.setOutlineColor(sf::Color(50, 50, 50));

            //Tablero replay
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

            //Pieza actual
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (FORMAPIEZA[piezaActual.tipo][piezaActual.rotacion][i][j] != 0) {
                        bloque.setPosition({ (piezaActual.x + j) * 30.0f + 50.0f, (piezaActual.y + i) * 30.0f });
                        bloque.setFillColor(sf::Color::Green);
                        ventana.draw(bloque);
                    }
                }
            }

            // Texto indicador en pantalla
            sf::Text textoReplayInfo(fuente, "MODO REPLAY: <- Izq / Der -> (Enter para salir)", 14);
            textoReplayInfo.setFillColor(sf::Color::Yellow);
            textoReplayInfo.setPosition({ 380.0f, 10.0f });

            ventana.draw(textoReplayInfo);
            ventana.draw(textoPuntaje);
            ventana.draw(tituloRanking);
            ventana.draw(textoRanking);

            ventana.display();
        }

        //Registro del jugador
        std::cout << "\nTu puntaje final es: " << puntajeActual << "\n";

        if (tabla.calificaEnTop10(puntajeActual)) {
            std::string nombreJugador;
            int opcionSort;

            std::cout << "\n Entraste al top 10 de los mejores jugadores \n";
            std::cout << "Escribe tu nombre (sin espacios): ";
            std::cin >> nombreJugador;

            std::cout << "\n  Que algoritmo usar? \n 1.Insertion Sort \n 2.Quicksort \n Opcion: ";
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