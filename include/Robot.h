#ifndef ROBOT_H
#define ROBOT_H

#include "Maze.h"

#include <utility>
#include <random>
#include <vector>
#include <algorithm>

class Maze;

enum class Direction { UP, RIGHT, DOWN, LEFT, NONE }; // Enum per le direzioni

class Robot {
  protected:
    Direction direction;          // Direzione del robot
    std::pair<int, int> position; // Posizione del robot
    static std::mt19937 gen;      // Generatore di numeri casuali

    // Metodo per ottenere la variazione di posizione in base alla direzione
    std::pair<int, int> directionToDelta(Direction dir) const {
      switch (dir) {
        case Direction::UP: return {-1, 0};
        case Direction::RIGHT: return {0, 1};
        case Direction::DOWN: return {1, 0};
        case Direction::LEFT: return {0, -1};
        default: return {0, 0};
      }
    }

    // Metodo per calcolare la direzione tra due posizioni
    Direction calculateDirection(const std::pair<int, int>& from, const std::pair<int, int>& to) const {
      if (to.first == from.first - 1 && to.second == from.second) return Direction::UP;     // Controllo se la posizione to è sopra la posizione from
      if (to.first == from.first && to.second == from.second + 1) return Direction::RIGHT;  // Controllo se la posizione to è a destra della posizione from
      if (to.first == from.first + 1 && to.second == from.second) return Direction::DOWN;   // Controllo se la posizione to è sotto la posizione from
      if (to.first == from.first && to.second == from.second - 1) return Direction::LEFT;   // Controllo se la posizione to è a sinistra della posizione from
      return Direction::NONE;
    }

  public:
    Robot(const std::pair<int, int>& startPos) : position(startPos) {}  // Costruttore di Robot
    virtual ~Robot() = default;                                         // Distruttore virtuale
    virtual bool move(Maze& maze) = 0;                                  // Metodo virtuale puro
    std::pair<int, int> getPosition() const { return position; }        // Getter per la posizione
    // Definisci la direzione con degli ENUM
};

// Initialize static member outside the class
std::mt19937 Robot::gen{std::random_device{}()};

/*
  Un robot che effettua movimenti casuali tra le 8 caselle vicine alla posizione corrente
*/
class RandomRobot : public Robot {
  private: 
    std::vector<std::pair<int, int>> visitedPos;                    // Vettore di posizioni visitate
    Direction currentDirection = Direction::NONE;                   // Direzione attuale

  public:
    RandomRobot(const std::pair<int, int>& startPos) : Robot(startPos) {}  // Costruttore di RandomRobot

    bool move(Maze& maze) override {
      if (maze.isExit(position)) return true;         // Controllo se la posizione attuale è una delle uscite
      if (!maze.isValidMove(position)) return false;  // Controllo se la posizione attuale è un muro

      if (currentDirection != Direction::NONE) {
        auto delta = directionToDelta(currentDirection);
        std::pair<int, int> newPos = {position.first + delta.first, position.second + delta.second};

        if (maze.isValidMove(newPos)) {
          position = newPos;
          visitedPos.push_back(position);
          return true;
        }
      }

      // Se non può andare nella direzione corrente, prova a cambiare direzione in modo casuale
      std::vector<Direction> possibleDirections = {Direction::UP, Direction::RIGHT, Direction::DOWN, Direction::LEFT};
      std::shuffle(possibleDirections.begin(), possibleDirections.end(), gen);

      for (const auto& dir : possibleDirections) {
          auto delta = directionToDelta(dir);
          std::pair<int, int> newPos = {position.first + delta.first, position.second + delta.second};

          if (maze.isValidMove(newPos)) {
              position = newPos;
              visitedPos.push_back(position);
              currentDirection = dir; // Aggiorna la direzione corrente
              return true;
          }
      }

      // Nessuna mossa valida trovata
      currentDirection = Direction::NONE; // Resetta la direzione
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
    Direction currentDirection = Direction::NONE; // Direzione attuale inizializzata a none

  public:
    RightHandRuleRobot(const std::pair<int,int>& startPos)       // Costruttore di RightHandRuleRobot
    : Robot(startPos), currentDirection(Direction::UP) {} 

    bool move(Maze& maze) override {                      // Implementazione del metodo move per RightHandRuleRobot
      if (maze.isExit(position)) return true;             // Controllo se la posizione attuale è una delle uscite
      if (!maze.isValidMove(position)) return false;      // Controllo se la posizione attuale è un muro

      // Lista delle direzioni da provare: seguendo il muro alla destra
      std::vector<Direction> priorityDirections = {
        turnRight(currentDirection),        // Tentativo di girare a destra
        currentDirection,                   // Procedi dritto
        turnLeft(currentDirection),         // Tentativo di girare a sinistra
        oppositeDirection(currentDirection) // Torna indietro se necessario
      };

      auto delta = directionToDelta(currentDirection);
      std::pair<int, int> newPos = {position.first + delta.first, position.second + delta.second};

      if (maze.isValidMove(newPos)) {
        position = newPos; // Aggiorna la posizione
        return true;
      } else {
        // Prova a cambiare direzione
        for (const auto& dir : priorityDirections) {
          auto delta = directionToDelta(dir);
          std::pair<int, int> newPos = {position.first + delta.first, position.second + delta.second};

          if (maze.isValidMove(newPos)) {
            position = newPos;      // Aggiorna la posizione
            currentDirection = dir; // Aggiorna la direzione corrente
            return true;
          }
        }
      }

      return false; // Nessuna mossa valida trovata
    };

    // Metodo per ottenere la direzione a destra rispetto a quella attuale
    Direction turnRight(Direction dir) const {
      switch (dir) {
        case Direction::UP: return Direction::RIGHT;
        case Direction::RIGHT: return Direction::DOWN;
        case Direction::DOWN: return Direction::LEFT;
        case Direction::LEFT: return Direction::UP;
        default: return Direction::NONE;
      }
    }

    // Metodo per ottenere la direzione a sinistra rispetto a quella attuale
    Direction turnLeft(Direction dir) const {
      switch (dir) {
          case Direction::UP: return Direction::LEFT;
          case Direction::LEFT: return Direction::DOWN;
          case Direction::DOWN: return Direction::RIGHT;
          case Direction::RIGHT: return Direction::UP;
          default: return Direction::NONE;
      }
    }

    // Metodo per ottenere la direzione opposta a quella attuale
    Direction oppositeDirection(Direction dir) const {
      switch (dir) {
          case Direction::UP: return Direction::DOWN;
          case Direction::DOWN: return Direction::UP;
          case Direction::LEFT: return Direction::RIGHT;
          case Direction::RIGHT: return Direction::LEFT;
          default: return Direction::NONE;
      }
    }
};

#endif // ROBOT_H