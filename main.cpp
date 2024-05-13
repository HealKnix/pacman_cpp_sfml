#include <iostream>
#include <unordered_map>
#include <math.h>
#include <windows.h>
#include <functional>
#include <map>
// #include <random>
// #include <time.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// #include "floatInc.h"
// #include "floatRandom.h"
#include "map.h"

#include "game_optional.h"
#include "game_shapes.h"
#include "game_sound.h"
#include "game_text.h"
#include "game_check_graphs.h"
#include "game_terminal.h"

#include "pacman.h"

std::vector<Rect> blocks;
std::vector<Rect> coins;
std::vector<Circle> abilities;
Terminal terminal;
Pacman pacman(14, 23, -4, sf::Color::Yellow);

void setDelay(std::function<void()> func, float clock, float delay) {
  if (clock >= delay) {
    func();
  }
}

void keyboardHandle(sf::Event event) {
  sf::Keyboard::Key keyCode = sf::Keyboard::Equal;
  keyCode = event.key.code;

  if (keyCode == sf::Keyboard::Tilde) {
    terminal.isOpen = !terminal.isOpen;
    std::cout << (terminal.isOpen ? "Открыта консоль" : "Консоль закрыта")
              << std::endl
              << std::endl;
    return;
  }

  if (terminal.isOpen) {
    bool isKeyCodesMatch = (
      keyToCharMap.find(keyCode) != keyToCharMap.end()
      || keyCode == 57
      || keyCode == 52
    );

    if (isKeyCodesMatch) {
      char character;
      if (event.key.shift)
        character = keyToCharMap[keyCode] - 'a' + 'A';
      else
        character = keyToCharMap[keyCode];

      if (event.key.code == 52)
        character = '/';
      else if (event.key.code == 57)
        character = ' ';

      std::string tempLetter;
      tempLetter.push_back(character);

      terminal.addLetterForCommand(tempLetter);
      return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
      terminal.deleteLastCommand();
      return;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
      terminal.runCommand();
      return;
    }

    if (keyCode == sf::Keyboard::Up) {
      terminal.update(Terminal::Switch::UP);
      return;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      terminal.update(Terminal::Switch::DOWN);
      return;
    }
  } else {
    if (keyCode == sf::Keyboard::A || keyCode == sf::Keyboard::Left) {
      if (map[pacman.y][pacman.x - 1] != 'b') {
        pacman.setDirection(Pacman::LEFT);
        return;
      }
    } else if (keyCode == sf::Keyboard::D || keyCode == sf::Keyboard::Right) {
      if (map[pacman.y][pacman.x + 1] != 'b') {
        pacman.setDirection(Pacman::RIGHT);
        return;
      }
    } else if (keyCode == sf::Keyboard::W || keyCode == sf::Keyboard::Up) {
      if (map[pacman.y - 1][pacman.x] != 'b') {
        pacman.setDirection(Pacman::UP);
        return;
      }
    } else if (keyCode == sf::Keyboard::S || keyCode == sf::Keyboard::Down) {
      if (map[pacman.y + 1][pacman.x] != 'b') {
        pacman.setDirection(Pacman::DOWN);
        return;
      }
    }
  }
}

int main() {
  system("cls");
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);

  window.setFramerateLimit(30);

  restartMap();

  sf::Clock globalTime;
  float preventTime = 0;

  bool isTimeToUpdate = true;

  GameText textScore("0", CEIL_SIZE, GameText::CENTER);

  sf::Clock gameClock;

  findGraphs();
  findNeighbours();

  fillKeys();

  while (window.isOpen()) {
    deltaTime = globalTime.getElapsedTime().asSeconds() - preventTime;

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::KeyPressed) {
        keyboardHandle(event);
      }
    }

    window.clear();
    blocks.clear();
    coins.clear();
    abilities.clear();
    ////////////////////////////////////////////////////////////

    setDelay(
        [&]() -> void {
          if (!terminal.isOpen) {
            map[pacman.y][pacman.x] = ' ';

            clearPassedGraph();
            getShortPathBetweenGraphsById(
                findClosestGraph(pacman.x, pacman.y).index, 21);

            if (pacman.checkLogic()) {
              map[pacman.y][pacman.x] = 'p';
            }
          }

          textScore.setText("Score: " + std::to_string(score));

          gameClock.restart();
        },
        gameClock.getElapsedTime().asMicroseconds(), 0.325f * 1000000.f);

    for (int i = 0; i < mapHeight; i++) {
      for (int j = 0; j < mapWidth; j++) {
        if (map[i][j] == 'b') {
          if (i < mapHeight - 1) {
            if (map[i][j] == 'b' && map[i + 1][j] == 'b') {
              blocks.emplace_back(Rect(j, i, 1, 2, 0, sf::Color(30, 30, 232)));
            }
          }
          if (j < mapWidth - 1) {
            if (map[i][j] == 'b' && map[i][j + 1] == 'b') {
              blocks.emplace_back(Rect(j, i, 2, 1, 0, sf::Color(30, 30, 232)));
            }
          }
          continue;
        }
        if (map[i][j] == 'd') {
          if (j < mapWidth - 1) {
            if (map[i][j] == 'd' && map[i][j + 1] == 'd') {
              blocks.emplace_back(Rect(j, i, 2, 0.5f, 0, sf::Color::Magenta));
            }
          }
          continue;
        }
        if (map[i][j] == '.') {
          coins.emplace_back(Rect(j, i, 18, sf::Color(255, 185, 175)));
          continue;
        }
        if (map[i][j] == '0') {
          abilities.emplace_back(Circle(j, i, 2, sf::Color(255, 185, 175)));
        }
        if (map[i][j] == 'c') {
          abilities.emplace_back(Circle(j, i, 2, sf::Color::Red));
        }
        if (map[i][j] == 'p') {
          abilities.emplace_back(Pacman(j, i, 4, sf::Color::Magenta));
        }
      }
    }

    for (int i = 0; i < blocks.size(); i++) {
      blocks[i].draw();
    }
    for (int i = 0; i < coins.size(); i++) {
      coins[i].draw();
    }
    for (int i = 0; i < abilities.size(); i++) {
      abilities[i].draw();
    }

    if (isDebug) {
      for (int i = 0; i < shortPath.size(); i++) {
        shortPath[i].draw();
      }
      for (int i = 0; i < graphs.size(); i++) {
        graphs[i].draw();
        graphsID[i].draw();
      }
    }

    pacman.update();
    pacman.draw();

    ////////////////////////////////////////////////////////////
    preventTime = globalTime.getElapsedTime().asSeconds();
    textScore.draw();
    terminal.draw();
    window.display();
  }

  return 0;
}
