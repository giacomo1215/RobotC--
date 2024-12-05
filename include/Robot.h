#ifndef ROBOT_H
#define ROBOT_H

#include "Maze.h"

#include <utility>
#include <random>
#include <vector>
#include <algorithm>

class Maze;

class Robot {
  protected:
    std::pair<int, int> position; // Posizione del robot
    std::random_device rd;        // Generatore di numeri casuali
    std::mt19937 gen;

  public:
    Robot(std::pair<int, int> startPos) : position(startPos), gen(rd()) {}  // Costruttore di Robot
    virtual ~Robot() = default;                                             // Distruttore virtuale
    virtual bool move(Maze& maze) = 0;                                      // Metodo virtuale puro
    std::pair<int, int> getPosition() const { return position; }            // Getter per la posizione
};

/*
  Un robot che effettua movimenti casuali tra le 8 caselle vicine alla posizione corrente
*/
class RandomRobot : public Robot {
  public:
    RandomRobot(std::pair<int, int> startPos) : Robot(startPos) {}  // Costruttore di RandomRobot

    bool move(Maze& maze) override {                                // Implementazione del metodo move per RandomRobot
      // Distribuzioni uniformi per x e y
      std::uniform_int_distribution<> distX(-1, 1);                 // Distribuzione uniforme per x
      std::uniform_int_distribution<> distY(-1, 1);                 // Distribuzione uniforme per y

      // Provo a fare mosse casuali finché non ne trovo una valida
      for (int attempts = 0; attempts < 8; ++attempts) {
        int dx = distX(gen), dy = distY(gen);

        // Evito di stare nello stesso posto
        if (dx == 0 && dy == 0) continue;

        // Calcolo la nuova posizione
        std::pair<int, int> newPos = {position.first + dx, position.second + dy};

        // Controllo se la nuova posizione è valida
        if (maze.isValidMove(newPos)) {
          position = newPos; 
          return true;
        }
      }
      return false;
    }
};


/*
  Un robot che si muove in modo che la sua mano destra sia sempre in contatto con una parete. 
  Se la posizione iniziale non è a contatto con nessuna parete, si sceglie una direzione iniziale casuale che determina tutti i successivi spostamenti finché il robot non entra in contatto con una parete. 
  In questo caso, la presenza di muri interni al labirinto può far sì che il robot non esca mai dal labirinto
*/
class RightHandRuleRobot : public Robot {
  private:
    enum class Direction { UP, RIGHT, DOWN, LEFT }; // Enum per le direzioni
    Direction currentDirection;                     // Direzione attuale

  public:
    RightHandRuleRobot(std::pair<int,int> startPos)       // Costruttore di RightHandRuleRobot
    : Robot(startPos), currentDirection(Direction::UP) {} 

    bool move(Maze& maze) override {                      // Implementazione del metodo move per RightHandRuleRobot
      // Controllo se la posizione attuale è una delle uscite
      if (maze.isExit(position)) return true;

      // Controllo se la posizione attuale è il punto di partenza
      if (position == maze.getStart()) {
        // Se la posizione iniziale non è a contatto con un muro, scelgo una direzione casuale
        if (!maze.isValidMove({position.first - 1, position.second})) currentDirection = Direction::UP;
        else if (!maze.isValidMove({position.first, position.second + 1})) currentDirection = Direction::RIGHT;
        else if (!maze.isValidMove({position.first + 1, position.second})) currentDirection = Direction::DOWN;
        else if (!maze.isValidMove({position.first, position.second - 1})) currentDirection = Direction::LEFT;
      };

      // Controllo se la posizione attuale è un muro
      if (!maze.isValidMove(position)) return false;

      // Vettore di possibili direzioni dove andare
      std::vector<std::pair<int, int>> possibleMoves;

      // Aggiungo le possibili mosse al vettore in base alla direzione attuale
      switch (currentDirection) {
        case Direction::UP:
          possibleMoves = {
            {position.first - 1, position.second}, // Sopra
            {position.first, position.second + 1}, // Destra
            {position.first, position.second - 1}  // Sinistra
          };
          break;

        case Direction::RIGHT:
          possibleMoves = {
            {position.first, position.second + 1}, // Destra
            {position.first + 1, position.second}, // Sotto
            {position.first - 1, position.second}  // Sopra
          };
          break;

        case Direction::DOWN:
          possibleMoves = {
            {position.first + 1, position.second}, // Sotto
            {position.first, position.second + 1}, // Destra
            {position.first, position.second - 1}  // Sinistra
          };
          break;

        case Direction::LEFT:
          possibleMoves = {
            {position.first, position.second - 1}, // Sinistra
            {position.first + 1, position.second}, // Sotto
            {position.first - 1, position.second}  // Sopra
          };
          break;

        default:
          break;
      }

      // Controllo se le possibili mosse sono valide
      for (const auto& move : possibleMoves) {
        if (maze.isValidMove(move)) {
          position = move; // Aggiorno la posizione

          // Aggiorno la direzione attuale
          if (move.first == position.first - 1) currentDirection = Direction::UP;           // Sopra
          else if (move.second == position.second + 1) currentDirection = Direction::RIGHT; // Destra
          else if (move.first == position.first + 1) currentDirection = Direction::DOWN;    // Sotto
          else if (move.second == position.second - 1) currentDirection = Direction::LEFT;  // Sinistra
          return true;
        }
      }

      return false;
    };
};

#endif // ROBOT_H