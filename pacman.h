GameSound scoreSound("audio/waka.wav", 2);  //* Звук подбирания очков
GameSound cherrySound("audio/cherry.wav", 20);  //* Звук подбирания вишни

struct Pacman : Circle {
  sf::Clock timeAbility;  //* Текущее время способности
  short int timeLimitAbility = 15;  //* Максимальное время способности
  bool isActiveAbility = false;
  bool godMode = false;
  short int direction;

  enum Direction {
    LEFT = 1,
    RIGHT = 2,
    UP = 3,
    DOWN = 4,
  };

  void setDirection(Direction direction) { this->direction = direction; }

  void resetAbilities() {
    isActiveAbility = false;
    godMode = false;
    timeAbility.restart();
    circle.setFillColor(sf::Color::Yellow);
  }

  void setActiveAbility() {
    isActiveAbility = true;
    godMode = true;
    timeAbility.restart();
  }

  Pacman(short int x, short int y, short int addGap, sf::Color color)
      : Circle(x, y, addGap, color){};

  void update() {
    setPosition(x, y, addGap);
    if (timeAbility.getElapsedTime().asSeconds() >= timeLimitAbility) {
      resetAbilities();
    }
    if (isActiveAbility) {
      const sf::Color colors[2] = { sf::Color::White, sf::Color::Yellow };
      circle.setFillColor(
          colors[(int)(timeAbility.getElapsedTime().asSeconds() *
                       (timeAbility.getElapsedTime().asSeconds() / 3)) %
                 2]);
    }
  }

  bool checkLogic() {
    if (map[y][x - 1] == 'b' && direction == 1 ||
        map[y][x + 1] == 'b' && direction == 2 ||
        map[y - 1][x] == 'b' && direction == 3 ||
        map[y + 1][x] == 'b' && direction == 4 ||
        map[y + 1][x] == 'd' && direction == 4) {
      return false;
    }

    switch (direction) {
      case LEFT:
        x -= 1;
        break;
      case RIGHT:
        x += 1;
        break;
      case UP:
        y -= 1;
        break;
      case DOWN:
        y += 1;
        break;
    }

    if (x <= 0) {
      x = 27;
    } else if (x >= 27) {
      x = 0;
    }
    if (y <= 1) {
      y = 1;
    } else if (y >= 29) {
      y = 29;
    }

    if (map[y][x] == '.') {
      score += 10 * multiplyScore;
      scoreSound.play();
    } else if (map[y][x] == '0') {
      score += 50 * multiplyScore;
      scoreSound.play();
    } else if (map[y][x] == 'c') {
      score += 150 * multiplyScore;
      setActiveAbility();
      cherrySound.play();
    }

    return true;
  }
};