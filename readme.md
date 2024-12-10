# Robot in Labirinto

#### Laboratorio 8 del corso Laboratorio di Programmazione
#### Studente: [Giacomo Giorgi](https://github.com/giacomo1215)

## Introduzione
Il progetto consiste in un robot che si muove all'interno di un labirinto rappresentato da una griglia generata tramite un file di testo. Il robot può essere di due tipi:

- **RandomRobot**: si muove casualmente all'interno del labirinto.
- **RightHandRuleRobot**: segue la regola della mano destra per trovare l'uscita.

## Struttura del Progetto
- `main.cpp`: contiene la funzione `main` e gestisce l'esecuzione del programma.
- `include/Maze.h`: definisce la classe `Maze` per la gestione del labirinto.
- `include/Robot.h`: definisce la classe base `Robot` e le classi derivate `RandomRobot` e `RightHandRuleRobot`.
- `CMakeLists.txt`: file per la configurazione della compilazione con CMake.

## Requisiti
- Compilatore C++ compatibile con lo standard C++17 o superiore.
- CMake versione 3.10 o superiore.

## Installazione
1. Clonare il repository:
2. Creare una directory `build` all'interno della directory del progetto.
3. Eseguire il comando `cmake ..` all'interno della directory `build`.
4. Eseguire il comando `make` all'interno della directory `build`.

## Utilizzo
Assicurarsi che il file `maze.txt` sia presente nella directory del progetto. Il file rappresenta il labirinto da risolvere.

Eseguire il comando `./MazeRobot` all'interno della directory `build` per far partire il programma.