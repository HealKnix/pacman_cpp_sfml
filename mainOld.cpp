#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <string>
#include <time.h>
#include <vector>

using namespace sf;

bool stop = false;
bool debug = false;

const short int WIDTH = 1920;
const short int HEIGHT = 1080;

float zoom = 1;
float zoomMouse = 1;
float deltaTime = 0;

View view(FloatRect(0.f, 0.f, WIDTH, HEIGHT));
RenderWindow window(VideoMode(WIDTH, HEIGHT), "SFML works!", Style::Default);

namespace mySTD {
template <typename T> T abs(T x) {
  if (x >= 0)
    return x;
  return -x;
}
} // namespace mySTD

uint32_t PCG_Hahs(uint32_t input) {
  uint32_t state = input * 747796405u + 2891336453u;
  uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 27803737u;
  return (word >> 22u) ^ word;
}

float getRandomFloat(uint32_t &seed) {
  seed = PCG_Hahs(seed);
  return (float)seed / (float)UINT32_MAX;
}

struct {
  float x, y;

  void update() {
    this->x = sf::Mouse::getPosition(window).x +
              (window.getView().getCenter().x - window.getSize().x / 2);
    this->y = sf::Mouse::getPosition(window).y +
              (window.getView().getCenter().y - window.getSize().y / 2);
  }
} mouse;

struct Circle {
  CircleShape shape;
  float dx = 0;
  float dy = 0;
  float vx = 0;
  float vy = 0;
  float speed = 2.f;
  float gravity = 9.8f;
  float prevX = 0;
  float prevY = 0;

  sf::Font font;
  Text textInfo;

  Circle(float x, float y, float r) {
    shape.setRadius(r);
    shape.setPosition(x + shape.getRadius(), y + shape.getRadius());
    shape.setFillColor(Color::Green);
    shape.setOutlineColor(Color(15, 25, 35));
    shape.setOutlineThickness(5);

    if (!font.loadFromFile("fonts/better-vcr_0.ttf")) {
    };
    textInfo.setFont(font);
    textInfo.setFillColor(Color::White);
    textInfo.setCharacterSize(shape.getRadius() / 4);
  }

  void setPosition(float x, float y) {
    shape.setPosition(x - shape.getRadius(), y - shape.getRadius());
  }

  Vector2f getPosition() {
    return Vector2f(shape.getPosition().x + shape.getRadius(),
                    shape.getPosition().y + shape.getRadius());
  }

  void positionToMouse() { setPosition(mouse.x, mouse.y); }
  void positionToMouse(float x, float y) {
    setPosition(mouse.x + x, mouse.y + y);
  }

  void setColor(Color color) { shape.setFillColor(color); }

  void checkWall() {
    uint32_t seed = rand() % 11859275 * deltaTime;
    if (getPosition().x - shape.getRadius() <= 0) {
      vx /= -1.125f + getRandomFloat(seed) / 10;
      setPosition(shape.getRadius(), getPosition().y);
    } else if (getPosition().x + shape.getRadius() >= window.getSize().x) {
      vx /= -1.125f + getRandomFloat(seed) / 10;
      setPosition(window.getSize().x - shape.getRadius(), getPosition().y);
    }
    if (getPosition().y - shape.getRadius() <= 0) {
      vy *= -1;
      setPosition(getPosition().x, shape.getRadius());
    } else if (getPosition().y + shape.getRadius() >= window.getSize().y) {
      vy /= -1.35f + getRandomFloat(seed) / 10;
      vx /= 1.015f;
      if (abs(vy) <= 0.01f) {
        vy = 0;
      }
      if (mySTD::abs(vx) <= 0.001f) {
        vx = 0;
      }
      setPosition(getPosition().x, window.getSize().y - shape.getRadius());
    }
  }

  void update() {
    const float length = sqrtf(prevX * prevX + prevY * prevY);

    if (length == 0.f) {
      dx = 0;
      dy = 0;
    } else {
      dx = prevX / length;
      dy = prevY / length;
    }

    vx += 0 * speed * deltaTime;
    vy += gravity * speed * deltaTime;

    setPosition(getPosition().x + vx, getPosition().y + vy);
    checkWall();
  }

  void checkCollision(std::vector<Circle> &circles, int index) {
    for (int i = 0; i < circles.size(); i++) {
      if (i != index) {
        float diffX =
            circles[index].getPosition().x - circles[i].getPosition().x;
        float diffY =
            circles[index].getPosition().y - circles[i].getPosition().y;
        float length = sqrtf(diffX * diffX + diffY * diffY);

        if (length <=
            circles[index].shape.getRadius() + circles[i].shape.getRadius()) {
          circles[index].vx /= -1.5;
          circles[index].vy /= -1.5;
          circles[i].vx /= -1.5;
          circles[i].vy /= -1.5;
          circles[index].setPosition(
              circles[index].getPosition().x +
                  (diffX / length) * (circles[index].shape.getRadius() +
                                      circles[i].shape.getRadius()),
              circles[index].getPosition().y +
                  (diffY / length) * (circles[index].shape.getRadius() +
                                      circles[i].shape.getRadius()));
          circles[index].setColor(Color::Red);
          circles[i].setColor(Color::Red);
          return;
        }
      }
    }
    circles[index].setColor(Color::Green);
  }

  void printInfo() {
    textInfo.setPosition(getPosition().x + shape.getRadius() + 5,
                         getPosition().y - shape.getRadius() / 2);
    textInfo.setString("dx: " + std::to_string(dx) + '\n' +
                       "dy: " + std::to_string(dy) + '\n' +
                       "coord: " + std::to_string(getPosition().x) + " : " +
                       std::to_string(getPosition().y) + '\n' +
                       "delta: " + std::to_string(deltaTime));
    Vertex dirLine[] = {
        Vertex(Vector2f(getPosition()), Color::Blue),
        Vertex(Vector2f(getPosition().x + dx * shape.getRadius(),
                        getPosition().y + dy * shape.getRadius()),
               Color::Blue)};
    window.draw(textInfo);
    window.draw(dirLine, 2, Lines);
  }

  void draw(bool debug) {
    window.draw(shape);
    if (debug) {
      printInfo();
    }
  }
};

int main() {
  srand(time(NULL));
  window.setFramerateLimit(170);

  RectangleShape background;
  background.setPosition(0, 0);
  background.setSize(Vector2f(WIDTH, HEIGHT));
  background.setFillColor(Color(15, 25, 35));

  Font fontStopText;
  if (!fontStopText.loadFromFile("fonts/ocra(RUS BY LYAJKA).ttf")) {
  }
  Text stopText;

  Clock clock;
  Clock clockDeltaTime;

  float prevDeltaTime = 0;

  Vector2f prevCoordForMoveView(0, 0);

  std::vector<Circle> circles;
  const short int countOfCircles = 50;

  for (int i = 0; i < countOfCircles; i++) {
    circles.push_back(*new Circle(rand() % window.getSize().x,
                                  rand() % window.getSize().y,
                                  rand() % 50 + 50));
    circles[i].checkWall();
  }

  while (window.isOpen()) {
    deltaTime = clockDeltaTime.getElapsedTime().asSeconds() - prevDeltaTime;
    prevDeltaTime = clockDeltaTime.getElapsedTime().asSeconds();

    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) {
        window.close();
      }
      if (event.type == Event::MouseMoved) {
        mouse.update();
      }
      if (Mouse::isButtonPressed(Mouse::Right)) {
        view.setCenter(view.getCenter().x + (prevCoordForMoveView.x - mouse.x),
                       view.getCenter().y + (prevCoordForMoveView.y - mouse.y));
        window.setView(view);
      }
      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
          if (event.mouseWheelScroll.delta > 0) {
            zoom = 0.85f;
            zoomMouse -= 0.15f;
          } else {
            zoom = 1.15f;
            zoomMouse += 0.15f;
          }
          view.zoom(zoom);
          window.setView(view);
        }
      }
    }

    window.clear(Color(5, 15, 25));
    window.draw(background);

    if (clock.getElapsedTime().asSeconds() >= 0.3) {
      if (Keyboard::isKeyPressed(Keyboard::Space)) {
        stop = !stop;
        clock.restart();
      }
      if (Keyboard::isKeyPressed(Keyboard::D)) {
        debug = !debug;
        clock.restart();
      }
      if (Keyboard::isKeyPressed(Keyboard::R)) {
        circles.clear();
        for (int i = 0; i < countOfCircles; i++) {
          circles.push_back(*new Circle(rand() % window.getSize().x,
                                        rand() % window.getSize().y, 15));
          circles[i].vx = (rand() - 1) / 1000;
          circles[i].vy = (rand() - 1) / 1000;
          circles[i].checkWall();
        }
        clock.restart();
      }
    }

    for (int i = 0; i < circles.size(); i++) {
      Vector2f prevCoord;
      if (!stop) {
        prevCoord = circles[i].getPosition();
        // circles[i].checkCollision(circles, i);
        circles[i].update();
        circles[i].prevX = circles[i].getPosition().x - prevCoord.x;
        circles[i].prevY = circles[i].getPosition().y - prevCoord.y;
      }
      if (Mouse::isButtonPressed(Mouse::Left)) {
        circles[i].positionToMouse();
        circles[i].vx = circles[i].getPosition().x - prevCoord.x;
        circles[i].vy = circles[i].getPosition().y - prevCoord.y;
      }
      circles[i].draw(debug);
    }

    ///////////////////////////////////////////////
    if (stop) {
      stopText.setString("PAUSE");
      stopText.setFont(fontStopText);
      stopText.setFillColor(Color::White);
      stopText.setCharacterSize(100);
      stopText.setPosition(window.getSize().x - stopText.getCharacterSize() * 4,
                           0);
      window.draw(stopText);
    }
    ///////////////////////////////////////////////

    prevCoordForMoveView.x = mouse.x;
    prevCoordForMoveView.y = mouse.y;

    window.display();
  }

  return 0;
}