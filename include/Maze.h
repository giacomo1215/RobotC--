#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <algorithm>

class Maze {
  private:
    std::vector<std::vector<char>> maze;
    std::pair<int, int> start;
    std::vector<std::pair<int, int>> finish;

  public:
    Maze(const std::string& fname) {};

    bool loadFromFile(const std::string& fname) {
      std::ifstream file(fname);
      
      // Controllo se il file è stato aperto correttamente
      if(!file.is_open()) {
        std::cerr << "Errore: impossibile aprire il file " << fname << std::endl;
        return false;
      }

      maze.clear();     // Pulisco il labirinto
      finish.clear();   // Pulisco le uscite
      std::string line; // Variabile temporanea per leggere le righe
      int y = 0;        // Contatore per la posizione y

      // Leggo il file riga per riga
      while (std::getline(file, line)) {
        std::vector<char> row;
        
        // Controllo dove si trova il punto di partenza e le uscite
        for (int x = 0; x < line.length(); ++x) {
          if (line[x] == 'S') start = {y, x};
          if (line[x] == 'E') finish.push_back({y, x});
        }

        // Aggiungo la riga letta al labirinto
        maze.push_back(row);
        ++y;
      }

      return !maze.empty(); // Restituisco true se il labirinto non è vuoto
    }

    bool isValidMove(const std::pair<int, int>& pos) const {
      // Estraggo le coordinate di pos
      int y = pos.first, x = pos.second; 

      // Controllo se i bordi della matrice sono validi
      if (y < 0 || y >= maze.size() || x < 0 || x >= maze[y].size()) return false;

      // Restituisco true se la posizione è valida
      return maze[y][x] != '*'; 
    };

    bool isExit(const std::pair<int, int>& pos) const {
      // Restituisco true se pos è una delle uscite
      return std::find(finish.begin(), finish.end(), pos) != finish.end();
    };

    std::pair<int, int> getStart() const {
      return start;
    };

    const std::vector<std::vector<char>>& getMaze() const {
      return maze;
    };
};

#endif // MAZE_H