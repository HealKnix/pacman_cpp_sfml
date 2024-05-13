const short int SCALE = 4;
const short int WIDTH = 224 * SCALE;
const short int HEIGHT = 288 * SCALE;
const short int CEIL_SIZE = 8 * SCALE;
const short int GAP = 2 * SCALE;
const short int GAP_FOR_TEXT = 3;

sf::RenderWindow window(sf::VideoMode(WIDTH + GAP, HEIGHT + GAP),
                        "Pac-Man",
                        sf::Style::Default);

const short int mapWidth = WIDTH / CEIL_SIZE;
const short int mapHeight = HEIGHT / CEIL_SIZE - 5;

int score = 0;
int multiplyScore = 1;

float deltaTime = 0.f;

bool isDebug = false;
