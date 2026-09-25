Proyecto Tetris - Estructuras de Datos (EIF207)

Este proyecto es una versión funcional y simplificada del clásico juego Tetris, desarrollada en C++ empleando la biblioteca gráfica
SFML. Implementa estructuras de datos lineales propias (`std::queue`, `std::stack`, `std::list`, `std::priority_queue` o `std::sort`).

Contenido y Estructuras Implementadas

*   Cola de Piezas-ColaPiezas: Generación de piezas mediante bolsas de 7 tetrominós aleatorios (Cola FIFO propia).
*   Pila de Hold-PilaHold:Reserva de pieza activa con capacidad fija de 1 elemento (Pila LIFO propia).
*   Historial y Replay-ListaReplay:Captura de movimientos para la navegación mediante Deshacer/Rehacer y
    reproducción de la partida completa al finalizar (Lista Doblemente Enlazada propia).
*   Cola de Eventos-ColaEventos: Eventos temporizados y programados ordenados dinámicamente por tiempo de disparo.
*   Tablero-Tablero:Representación del área de juego mediante una lista enlazada de 20 nodos de fila.
*   Tabla de Puntajes-TablaPuntajes: Persistencia en archivo y ordenamiento de marcadores mediante dos algoritmos
    propios ($O(n^2)$ como Inserción/Burbuja y $O(n \log n)$ como Quicksort/Mergesort).

Requisitos del Sistema

1.  Compilador de C++: Compatible con C++17 o superior (GCC / MinGW / MSVC).
2.  Biblioteca Gráfica: SFML (Simple and Fast Multimedia Library) v2.5+ o v3.x.
3.  Archivos de Recursos: Debe existir un archivo de fuente tipográfica (`arial.ttf`) ubicado en la misma carpeta raíz del ejecutable.

Controles del Juego

Flechas Izquierda / Derecha: Mover la pieza horizontalmente.
Flecha Arriba: Rotar la pieza (4 orientaciones precalculadas).
Flecha Abajo: Caída rápida (Soft Drop).
Tecla C: Enviar/intercambiar pieza en la casilla de reserva (Hold).
Tecla Z: Deshacer movimiento (Undo en el historial).
Tecla Y: Rehacer movimiento (Redo en el historial).
Controles de Replay (Pantalla de Game Over):
Flecha Izquierda: Retroceder un paso en la repetición de la partida.
Flecha Derecha: Avanzar un paso en la repetición de la partida.
Enter: Salir del Replay e ir a la consola a guardar el puntaje.

Entorno de Desarrollo (Zinjal / Visual Studio)

Abre el proyecto dentro del entorno (archivo .zpr o .sln).
Verifica que las propiedades del proyecto apunten a los directorios de inclusión (include) y librerías (lib) de SFML.
Asegúrate de incluir las librerías dinámicas (sfml-graphics.dll, sfml-window.dll, sfml-system.dll) y la fuente arial.ttf 
en la carpeta del ejecutable.
Compila y ejecuta el proyecto (F5 o botón de Ejecutar).
