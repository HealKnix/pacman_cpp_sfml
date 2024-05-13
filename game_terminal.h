void printSuccessCommand(std::string text) {
  std::cout << "========>" << std::endl;
  std::cout << text << std::endl;
  std::cout << "========>" << std::endl << std::endl;
}

struct Terminal {
  sf::RectangleShape rect;
  GameText consoleText;
  bool isOpen = false;

  std::string command;
  std::string symbolForCurrentPosition = "_";
  std::string stringCurrentPosition = symbolForCurrentPosition;

  std::vector<std::string> slicedCommands;
  std::vector<std::string> pastCommands;

  sf::Clock textInputClock;
  sf::Clock terminalClock;

  float inputDelay = 0.01f;

  short int caretIndex = 0;
  std::map<std::string, std::function<void()>> scripts;

  enum Switch {
    UP = -1,
    DOWN = 1,
  };

  void open() { isOpen = true; }
  void close() { isOpen = false; }

  bool findScriptName(std::map<std::string, std::function<void()>> scriptsName,
                      std::string name) {
    if (scriptsName.find(name) != scriptsName.end()) {
      scriptsName[name]();
      return true;
    }
    return false;
  }
  void script_setDebug() {
    isDebug = !isDebug;
    if (isDebug)
      printSuccessCommand("Режим отладки включен");
    else
      printSuccessCommand("Режим отладки выключен");
    close();
  }
  void script_setMultScore() {
    try {
      multiplyScore = stoi(slicedCommands.at(2));
      printSuccessCommand("Множитель очков выставлен на " +
                          std::to_string(multiplyScore));
      close();
    } catch (std::exception& e) {
      std::cout << "Неполная команда" << std::endl << std::endl;
    }
  }
  void script_changeScore() {
    std::map<std::string, std::function<void()>> innerScripts;
    try {
      auto operation = [&](int value) {
        score += value;
        close();
      };

      innerScripts["add"] = [&]() { operation(stoi(slicedCommands.at(3))); };
      innerScripts["rem"] = [&]() { operation(-stoi(slicedCommands.at(3))); };

      if (!findScriptName(innerScripts, slicedCommands.at(2))) {
        std::cout << "Неполная команда (add/rem)" << std::endl << std::endl;
      } else {
        if (slicedCommands.at(2) == "add") {
          printSuccessCommand("Было прибавлено " + slicedCommands.at(3) +
                              " очков");
        } else if (slicedCommands.at(2) == "rem") {
          printSuccessCommand("Было убавлено " + slicedCommands.at(3) +
                              " очков");
        }
      }
    } catch (std::exception& e) {
      std::cout << "Неполная команда" << std::endl << std::endl;
    }
  }

  Terminal() {
    rect.setFillColor(sf::Color(33, 33, 33));
    rect.setPosition(sf::Vector2f(0, HEIGHT - 50));
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color::White);
    rect.setSize(sf::Vector2f(WIDTH + GAP, 50 + GAP));
    consoleText.text.setPosition(GAP, HEIGHT - 50 + (GAP * 2) / 1.5);
    consoleText.setFont("fonts/better-vcr_0.ttf");
  }

  void update(Switch dir) {
    if (!pastCommands.empty()) {
      if (textInputClock.getElapsedTime().asSeconds() >= inputDelay) {
        caretIndex += dir;
        if (caretIndex <= 0) {
          caretIndex = 0;
        } else if (caretIndex >= pastCommands.size() - 1) {
          caretIndex = pastCommands.size() - 1;
        }
        command.clear();
        command = pastCommands.at(caretIndex);
        textInputClock.restart();
      }
    }
  }

  void addLetterForCommand(std::string letter) {
    if (textInputClock.getElapsedTime().asSeconds() >= inputDelay) {
      this->command += letter;
      stringCurrentPosition = symbolForCurrentPosition;
      terminalClock.restart();
      textInputClock.restart();
    }
  }

  std::vector<std::string> getSlicedCommands(std::string command) {
    command += '\0';
    std::vector<std::string> slicedCommands;
    int slicePos = 0;
    for (int i = 0; i < command.size(); i++) {
      if (command.at(i) == '/') {
        std::string tempString = command.substr(slicePos, i - slicePos + 1);
        slicePos = i + 1;
        slicedCommands.push_back(tempString);
      } else if (command.at(i) == ' ' || command.at(i) == '\0') {
        std::string tempString = command.substr(slicePos, i - slicePos);
        slicePos = i + 1;
        if (tempString.empty()) continue;
        slicedCommands.push_back(tempString);
      }
    }

    return slicedCommands;
  }

  void initCommands() {
    scripts["debug"] = [&]() { script_setDebug(); };
    scripts["multScore"] = [&]() { script_setMultScore(); };
    scripts["score"] = [&]() { script_changeScore(); };
    scripts["restart"] = [&]() {
      restartMap();
      printSuccessCommand("Карта была перезапущена");
      close();
    };
    scripts["exit"] = [&]() { window.close(); };
    scripts["help"] = [&]() {
      const std::string scr_names[] = {"debug", "multScore", "score", "restart", "exit"};
      for (int i = 0; i < scripts.size()-1; i++) {
        std::cout << scr_names[i] << std::endl;
      }
      std::cout << std::endl;
    };
  }

  void runCommand() {
    if (command.empty()) return;
    if (textInputClock.getElapsedTime().asSeconds() >= inputDelay) {
      slicedCommands.clear();
      slicedCommands = getSlicedCommands(command);

      initCommands();

      if (slicedCommands.at(0) == "/" && slicedCommands.size() > 1) {
        if (!findScriptName(scripts, slicedCommands.at(1))) {
          std::cout << "Такой команды не существует" << std::endl << std::endl;
        }
      } else {
        std::cout << "Вы сказали: ";
        for (int i = 0; i < slicedCommands.size(); i++) {
          std::cout << slicedCommands.at(i);
          if (i < slicedCommands.size() - 1) {
            std::cout << " ";
          }
        }
        std::cout << std::endl << std::endl;
      }

      this->pastCommands.push_back(command);
      this->caretIndex = this->pastCommands.size();
      this->command.clear();
      textInputClock.restart();
    }
  }

  void deleteLastCommand() {
    if (command.size() <= 0) return;
    if (textInputClock.getElapsedTime().asSeconds() >= inputDelay) {
      std::string tempCommand = command.erase(command.size() - 1);
      command.clear();
      command = tempCommand;
      stringCurrentPosition = symbolForCurrentPosition;
      terminalClock.restart();
      textInputClock.restart();
    }
  }

  void draw() {
    if (isOpen) {
      window.draw(rect);
      if (terminalClock.getElapsedTime().asSeconds() >= 0.5f) {
        stringCurrentPosition = " ";
      }
      if (terminalClock.getElapsedTime().asSeconds() >= 1.f) {
        stringCurrentPosition = symbolForCurrentPosition;
        terminalClock.restart();
      }
      consoleText.setText(command + stringCurrentPosition);
      consoleText.draw();
    };
  }
};

// Таблица соответствия клавиш и символов
std::unordered_map<sf::Keyboard::Key, char> keyToCharMap;

void fillKeys() {
  // Соответствия для буквенных клавиш (A-Z)
  for (int key = sf::Keyboard::A; key <= sf::Keyboard::Z; ++key) {
    keyToCharMap[static_cast<sf::Keyboard::Key>(key)] =
        static_cast<char>('a' + key - sf::Keyboard::A);
  }

  // Соответствия для цифровых клавиш (0-9)
  for (int key = sf::Keyboard::Num0; key <= sf::Keyboard::Num9; ++key) {
    keyToCharMap[static_cast<sf::Keyboard::Key>(key)] =
        static_cast<char>('0' + key - sf::Keyboard::Num0);
  }
}
