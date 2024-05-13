struct GameText {
  sf::Font font;
  sf::Text text;

  enum { LEFT = 1, CENTER = 2, RIGHT = 3 };

  void setText(std::string stringText) { text.setString(stringText); }

  void setSize(int size) { text.setCharacterSize(size); }

  void setPosition(short int x, short int y) {
    text.setPosition(x * CEIL_SIZE + GAP, (y + GAP_FOR_TEXT) * CEIL_SIZE + GAP);
  }

  void setAlign(short int textAlign) {
    switch (textAlign) {
      case LEFT:
        text.setPosition(0, CEIL_SIZE);
        break;
      case CENTER:
        text.setPosition(WIDTH / 2 - text.getCharacterSize() * 3, CEIL_SIZE);
        break;
      case RIGHT:
        text.setPosition(WIDTH - text.getCharacterSize() * CEIL_SIZE / 5,
                         CEIL_SIZE);
        break;
      default:
        text.setPosition(0, CEIL_SIZE);
        break;
    }
  }

  void setFont(std::string fileName) {
    if (!font.loadFromFile(fileName)) {
    };
    text.setFont(font);
  }

  GameText() {
    setFont("fonts/better-vcr_0.ttf");
    setSize(CEIL_SIZE);
    text.setFillColor(sf::Color::White);
  }

  GameText(std::string stringText, short int textSize, short int textAlign) {
    setFont("fonts/better-vcr_0.ttf");
    setSize(CEIL_SIZE);
    text.setFillColor(sf::Color::White);
    setText(stringText);
    setAlign(textAlign);
  }

  GameText(std::string stringText, short int textSize, int x, int y) {
    setFont("fonts/better-vcr_0.ttf");
    setSize(CEIL_SIZE);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(x, y));
    setText(stringText);
  }

  void draw() { window.draw(text); }
};