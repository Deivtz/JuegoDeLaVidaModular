/*
 * Universidad Mariano Galvez
 * Curso: Programacion 1
 * Estudiante: David Barrios
 * Tarea: Implementacion del Juego de la Vida de Conway
 * Descripcion: Simulacion celular basada en reglas matematicas utilizando matrices bidimensionales.
 * Cumple con los requisitos de programacion modular y convenciones de nombres.
 */

#include <iostream>  // Para entrada y salida de datos en consola (cin, cout)
#include <vector>    // Para el manejo de matrices dinamicas
#include <cstdlib>   // Para la generacion de numeros aleatorios (rand, srand)
#include <ctime>     // Para inicializar la semilla de aleatoriedad con la hora del sistema
#include <thread>    // Para el manejo de hilos y pausas de tiempo
#include <chrono>    // Para la medicion de tiempo en milisegundos
#include <windows.h> // Para ajustar la codificacion de la consola a UTF-8

// Constante global (Convencion: UPPER_SNAKE_CASE)
const int TAMAÑO_TABLERO = 10; 

// Declaracion de funciones modulares (Convencion: snake_case para funciones)
void inicializar_tablero(std::vector<std::vector<int>>& tablero, int tamaño);
int calcular_vecinos_vivos(const std::vector<std::vector<int>>& tablero, int fila, int columna, int tamaño);
void actualizar_tablero(std::vector<std::vector<int>>& tablero, int tamaño);
void imprimir_tablero(const std::vector<std::vector<int>>& tablero, int tamaño);

int main() {
    // Configuracion de la consola de Windows para soportar caracteres UTF-8 (como el cuadro solido)
    SetConsoleOutputCP(CP_UTF8);

    // Declaracion de variables locales (Convencion: camelCase para variables)
    int tamanoTablero, numeroGeneraciones; 
    
    // Ingreso de parametros iniciales por el usuario
    std::cout << "Ingrese el tamano del tablero (ej. 20 o 25 para evitar desbordamiento visual): ";
    std::cin >> tamanoTablero; 
    std::cout << "Ingrese el numero de generaciones a simular: ";
    std::cin >> numeroGeneraciones; 

    // Creacion de la matriz bidimensional inicializada en 0 (estado muerta)
    std::vector<std::vector<int>> tablero(tamanoTablero, std::vector<int>(tamanoTablero, 0)); 
    
    // Llamada a la funcion para establecer el estado inicial de la matriz
    inicializar_tablero(tablero, tamanoTablero);

    // Ciclo principal que simula el avance de las generaciones
    for (int i = 0; i < numeroGeneraciones; ++i) {
        // Limpieza de la pantalla para dar efecto de animacion en el mismo espacio
        system("cls"); 
        
        std::cout << "Generacion: " << i + 1 << std::endl;
        
        // Se imprime el estado actual antes de calcular los cambios
        imprimir_tablero(tablero, tamanoTablero);
        
        // Se calculan y aplican las reglas para la siguiente generacion
        actualizar_tablero(tablero, tamanoTablero);
        
        // Retraso de 1000 milisegundos (1 segundo) para poder visualizar los cambios correctamente
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    }

    return 0;
}

// ==========================================
// Implementacion de las funciones del juego
// ==========================================

// Funcion que permite al usuario decidir el estado inicial del tablero
void inicializar_tablero(std::vector<std::vector<int>>& tablero, int tamaño) {
    int opcionMenu; // camelCase
    std::cout << "\nElija el metodo para inicializar el tablero:\n";
    std::cout << "1. Generacion Aleatoria\n";
    std::cout << "2. Patron Predefinido (Planeador / Glider)\n";
    std::cout << "Opcion: ";
    std::cin >> opcionMenu;

    if (opcionMenu == 1) {
        // Inicializacion aleatoria
        srand(time(0));
        for (int filaActual = 0; filaActual < tamaño; ++filaActual) {
            for (int colActual = 0; colActual < tamaño; ++colActual) {
                // Asignacion de estado: 0 (muerta) o 1 (viva) utilizando modulo
                tablero[filaActual][colActual] = rand() % 2; 
            }
        }
    } else if (opcionMenu == 2) {
        // Inicializacion con patron Planeador (Glider)
        // Se requiere un tablero de al menos 3x3 para colocar la figura
        if (tamaño >= 3) {
            // Se encienden unicamente las 5 celulas que conforman el planeador
            tablero[0][1] = 1;
            tablero[1][2] = 1;
            tablero[2][0] = 1;
            tablero[2][1] = 1;
            tablero[2][2] = 1;
        } else {
            std::cout << "Error: El tablero es muy pequeno para dibujar el patron.\n";
            system("pause");
        }
    } else {
        std::cout << "Opcion no valida. El tablero iniciara completamente vacio.\n";
        system("pause");
    }
}

// Funcion encargada de mostrar la matriz en consola con un formato visual claro
void imprimir_tablero(const std::vector<std::vector<int>>& tablero, int tamaño) {
    for (int filaActual = 0; filaActual < tamaño; ++filaActual) {
        for (int colActual = 0; colActual < tamaño; ++colActual) {
            if (tablero[filaActual][colActual] == 1) {
                std::cout << "■ "; // Representacion de celula viva
            } else {
                std::cout << ". "; // Representacion de celula muerta
            }
        }
        // Se utiliza std::endl para vaciar el buffer de salida y garantizar que se imprima la linea
        std::cout << std::endl; 
    }
}

// Funcion que cuenta cuantas celulas vivas existen alrededor de una coordenada especifica
int calcular_vecinos_vivos(const std::vector<std::vector<int>>& tablero, int fila, int columna, int tamaño) {
    int cantidadVecinos = 0; 

    // Recorrido de la submatriz de 3x3 que rodea a la celula evaluada
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            // Condicion para no contar la celula central (la celula que se esta evaluando)
            if (i == 0 && j == 0) continue;

            int filaVecino = fila + i;
            int colVecino = columna + j;

            // Manejo de bordes: Borde estatico. 
            // Solo se suman las celulas si las coordenadas del vecino estan dentro de los limites de la matriz.
            if (filaVecino >= 0 && filaVecino < tamaño && colVecino >= 0 && colVecino < tamaño) {
                cantidadVecinos += tablero[filaVecino][colVecino];
            }
        }
    }
    return cantidadVecinos;
}

// Funcion principal de logica matematica donde se aplican las reglas de Conway
void actualizar_tablero(std::vector<std::vector<int>>& tablero, int tamaño) {
    // Eficiencia: Creacion de una copia temporal del tablero.
    // Esto evita que los cambios en las primeras celulas afecten el calculo de los vecinos de las ultimas celulas.
    std::vector<std::vector<int>> tableroTemporal = tablero; 

    for (int filaActual = 0; filaActual < tamaño; ++filaActual) {
        for (int colActual = 0; colActual < tamaño; ++colActual) {
            
            // Se obtienen los vecinos de la celula actual leyendo el tablero ORIGINAL
            int vecinos = calcular_vecinos_vivos(tablero, filaActual, colActual, tamaño);

            // Aplicacion estricta de las 4 Reglas de Conway sobre el tablero TEMPORAL
            if (tablero[filaActual][colActual] == 1) { // Si la celula esta actualmente viva
                
                // Regla 1 (Subpoblacion): Menos de 2 vecinos -> Muere
                // Regla 3 (Sobrepoblacion): Mas de 3 vecinos -> Muere
                if (vecinos < 2 || vecinos > 3) {
                    tableroTemporal[filaActual][colActual] = 0; 
                }
                
                // Regla 2 (Supervivencia): 2 o 3 vecinos -> Sigue viva (se mantiene el 1)
                
            } else { // Si la celula esta actualmente muerta
                
                // Regla 4 (Reproduccion): Exactamente 3 vecinos -> Nace una nueva celula
                if (vecinos == 3) {
                    tableroTemporal[filaActual][colActual] = 1; 
                }
            }
        }
    }
    
    // Al finalizar la evaluacion de todas las celdas, se sobreescribe el tablero original con el nuevo estado
    tablero = tableroTemporal;
}