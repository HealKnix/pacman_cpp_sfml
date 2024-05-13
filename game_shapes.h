struct Rect {
  short int x = 0;
  short int y = 0;
  short int width = CEIL_SIZE;
  short int height = CEIL_SIZE;
  sf::RectangleShape rect;
  sf::Color color = sf::Color::White;

  Rect() {}

  Rect(short int x, short int y, sf::Color color) {
    this->x = x;
    this->y = y;
    rect.setPosition(x * CEIL_SIZE + GAP, (y + GAP_FOR_TEXT) * CEIL_SIZE + GAP);
    rect.setSize(sf::Vector2f(width - GAP, height - GAP));
    this->color = color;
    rect.setFillColor(this->color);
  }

  Rect(short int x, short int y, short int addGap, sf::Color color) {
    this->x = x;
    this->y = y;
    rect.setPosition(x * CEIL_SIZE + GAP + (addGap / 2),
                     (y + GAP_FOR_TEXT) * CEIL_SIZE + GAP + (addGap / 2));
    rect.setSize(sf::Vector2f(width - GAP - addGap, height - GAP - addGap));
    this->color = color;
    rect.setFillColor(this->color);
  }

  Rect(short int x, short int y, float widthScale, float heightScale,
       short int addGap, sf::Color color) {
    this->x = x;
    this->y = y;
    this->width *= widthScale;
    this->height *= heightScale;
    rect.setPosition(x * CEIL_SIZE + GAP + (addGap / 2),
                     (y + GAP_FOR_TEXT) * CEIL_SIZE + GAP + (addGap / 2));
    rect.setSize(sf::Vector2f(width - GAP - addGap, height - GAP - addGap));
    this->color = color;
    rect.setFillColor(this->color);
  }

  void draw() { window.draw(rect); }
};

struct Circle {
  int x = 0;
  int y = 0;
  float r = CEIL_SIZE / 2;
  short int addGap = 0;
  sf::CircleShape circle;
  sf::Color color = sf::Color::White;

  Circle(){};

  Circle(short int x, short int y, short int addGap, sf::Color color) {
    this->x = x;
    this->y = y;
    this->color = color;
    this->addGap = addGap;
    this->circle.setPosition(x * CEIL_SIZE + GAP + addGap,
                             (y + GAP_FOR_TEXT) * CEIL_SIZE + GAP + addGap);
    circle.setRadius(this->r - (GAP / 2) - addGap);
    circle.setFillColor(this->color);
  }

  void setPosition(int x, int y, short int addGap) {
    this->circle.setPosition(x * CEIL_SIZE + GAP + addGap,
                             (y + GAP_FOR_TEXT) * CEIL_SIZE + GAP + addGap);
    circle.setRadius(this->r - (GAP / 2) - addGap);
  }

  virtual void update() { setPosition(x, y, addGap); }

  void draw() { window.draw(circle); }
};