#include "include/Maze.h"
#include "include/Robot.h"
#include <iostream>
#include <memory>

int main() {
  try {
    Maze maze("maze.txt"); // Creo un labirinto e lo carico da file

    // Controllo se il labirinto è stato caricato correttamente
    if (!maze.loadFromFile("maze.txt")) {
      std::cerr << "Errore: impossibile caricare il labirinto" << std::endl;
      return 1;
    }

    std::unique_ptr<Robot> robot; // Dichiaro un puntatore unico a Robot
    
    // Faccio scegliere all'utente la tipologia di robot da utilizzare
    std::cout << "Scegli la tipologia di robot da utilizzare (1: RandomRobot, 2: RightHandRuleRobot): ";
    int choice;
    std::cin >> choice;

    // In base alla scelta dell'utente creo un oggetto di tipo RandomRobot o RightHandRuleRobot
    switch (choice) {
      case 1:
        robot = std::make_unique<RandomRobot>(maze.getStart());
        break;
      case 2:
        robot = std::make_unique<RightHandRuleRobot>(maze.getStart());
        break;
      default:
        std::cerr << "Scelta non valida" << std::endl;
        return 1;
    }

    int moveCount = 0;

    // Inizio a muovere il robot finché non esce dal labirinto
    while(true) {
      ++moveCount;
      auto pos = robot->getPosition();
      // Mostro graficamente la posizione del robot nel labirinto
      maze.print(pos);
      std::cout << "Il robot si trova in posizione (" << pos.first << ", " << pos.second << ") dopo " << moveCount << " mosse." << std::endl;

      if (maze.isExit(pos)) {
        std::cout << "Il robot è uscito dal labirinto!" << std::endl;
        break;
      }

      if (!robot->move(maze)) {
        std::cout << "Il robot è bloccato!" << std::endl;
        break;
      }
    }

  } catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}