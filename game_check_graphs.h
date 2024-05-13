template <typename T>
struct GraphNeighbour {
  T neighbour;
  short int distance;

  GraphNeighbour(T graph, short int distance) {
    neighbour = graph;
    this->distance = distance;
  }
};

struct Graph : Rect {
  int index = 0;
  bool isPassed = false;
  std::vector<GraphNeighbour<Graph>> neighbours;
  int length = 0;

  Graph() : Rect() {}

  Graph(short int x, short int y, short int index)
      : Rect(x, y, sf::Color(0, 255, 255, 50)) {
    this->index = index;
  }
};

/*
 *  ?
 * ???
 *  ?
 */
bool checkGraph1(int i, int j) {
  return map[i][j - 1] != 'b' && map[i][j + 1] != 'b' && map[i - 1][j] != 'b' &&
         map[i + 1][j] != 'b';
}

/*
 *  ?
 * ???
 */
bool checkGraph2(int i, int j) {
  return map[i][j - 1] != 'b' && map[i][j + 1] != 'b' && map[i - 1][j] == 'b' &&
         map[i + 1][j] != 'b';
}

/*
 * ???
 *  ?
 */
bool checkGraph3(int i, int j) {
  return map[i][j - 1] != 'b' && map[i][j + 1] != 'b' && map[i - 1][j] != 'b' &&
         map[i + 1][j] == 'b';
}

/*
 * ?
 * ??
 * ?
 */
bool checkGraph4(int i, int j) {
  return map[i][j - 1] == 'b' && map[i][j + 1] != 'b' && map[i - 1][j] != 'b' &&
         map[i + 1][j] != 'b';
}

/*
 *  ?
 * ??
 *  ?
 */
bool checkGraph5(int i, int j) {
  return map[i][j - 1] != 'b' && map[i][j + 1] == 'b' && map[i - 1][j] != 'b' &&
         map[i + 1][j] != 'b';
}

/*
 * ??
 * ?
 */
bool checkGraphAngle1(int i, int j) {
  return map[i][j - 1] == 'b' && map[i][j + 1] != 'b' && map[i - 1][j] == 'b' &&
         map[i + 1][j] != 'b';
}

/*
 * ??
 *  ?
 */
bool checkGraphAngle2(int i, int j) {
  return map[i][j - 1] != 'b' && map[i][j + 1] == 'b' && map[i - 1][j] == 'b' &&
         map[i + 1][j] != 'b';
}

/*
 * ?
 * ??
 */
bool checkGraphAngle3(int i, int j) {
  return map[i][j - 1] == 'b' && map[i][j + 1] != 'b' && map[i - 1][j] != 'b' &&
         map[i + 1][j] == 'b';
}

/*
 *  ?
 * ??
 */
bool checkGraphAngle4(int i, int j) {
  return map[i][j - 1] != 'b' && map[i][j + 1] == 'b' && map[i - 1][j] != 'b' &&
         map[i + 1][j] == 'b';
}

int graphsIDLength = 0;
GameText graphsID[100] = GameText();
std::vector<Graph> graphs;
std::vector<Rect> shortPath;

void clearPassedGraph() {
  shortPath.clear();
  for (int i = 0; i < graphs.size(); i++) {
    graphs[i].isPassed = false;
  }
}

void findGraphs() {
  int index = 0;
  for (int i = 1; i < mapHeight - 1; i++) {
    for (int j = 1; j < mapHeight - 1; j++) {
      if (map[i][j] != 'e' && map[i][j] != 'b' && map[i][j] != 'd' &&
          map[i + 1][j] != 'd') {
        if (checkGraph1(i, j) || checkGraph2(i, j) || checkGraph3(i, j) ||
            checkGraph4(i, j) || checkGraph5(i, j) || checkGraphAngle1(i, j) ||
            checkGraphAngle2(i, j) || checkGraphAngle3(i, j) ||
            checkGraphAngle4(i, j)) {
          graphs.emplace_back(Graph(j, i, index));
          graphsID[index].setText(std::to_string(graphs[index].index));
          graphsID[index].setSize(8);
          graphsID[index].setPosition(graphs[index].x, graphs[index].y);
          index++;
        }
      }
    }
  }
  graphsIDLength = index;
}

bool isGraphExistByPos(short int x, short int y) {
  for (int i = 0; i < graphs.size(); i++)
    if (graphs[i].x == x && graphs[i].y == y) return true;
  return false;
}

Graph getGraphByPos(short int x, short int y) {
  for (int i = 0; i < graphs.size(); i++)
    if (graphs[i].x == x && graphs[i].y == y) return graphs[i];
  return Graph(-1, -1, -1);
}

Graph getGraphById(short int ID) {
  for (int i = 0; i < graphs.size(); i++)
    if (graphs[i].index == ID) return graphs[i];
  return Graph(-1, -1, -1);
}

int getDistanceToNeigbour(short int originID, short int neighbourID) {
  for (int i = 0; i < graphs[originID].neighbours.size(); i++) {
    if (graphs[originID].neighbours[i].neighbour.index == neighbourID) {
      return graphs[originID].neighbours[i].distance;
    }
  }
  return -1;
}

void findNeighbours() {
  for (int i = 0; i < graphs.size(); i++) {
    if (map[graphs[i].y][graphs[i].x - 1] != 'b') {  // LEFT DIRECTION
      short int dirWayX = 1;
      while (map[graphs[i].y][graphs[i].x - dirWayX] != 'b' && dirWayX <= 11) {
        if (isGraphExistByPos(graphs[i].x - dirWayX, graphs[i].y)) {
          graphs[i].neighbours.emplace_back(GraphNeighbour<Graph>(
              getGraphByPos(graphs[i].x - dirWayX, graphs[i].y),
              abs(graphs[i].x - (graphs[i].x - dirWayX))));
          break;
        }
        dirWayX++;
      }
    }
    if (map[graphs[i].y][graphs[i].x + 1] != 'b') {  // RIGHT DIRECTION
      short int dirWayX = 1;
      while (map[graphs[i].y][graphs[i].x + dirWayX] != 'b' && dirWayX <= 11) {
        if (isGraphExistByPos(graphs[i].x + dirWayX, graphs[i].y)) {
          graphs[i].neighbours.emplace_back(GraphNeighbour<Graph>(
              getGraphByPos(graphs[i].x + dirWayX, graphs[i].y),
              abs(graphs[i].x - (graphs[i].x + dirWayX))));
          break;
        }
        dirWayX++;
      }
    }
    if (map[graphs[i].y - 1][graphs[i].x] != 'b') {  // TOP DIRECTION
      short int dirWayY = 1;
      while (map[graphs[i].y - dirWayY][graphs[i].x] != 'b' && dirWayY <= 11) {
        if (isGraphExistByPos(graphs[i].x, graphs[i].y - dirWayY)) {
          graphs[i].neighbours.emplace_back(GraphNeighbour<Graph>(
              getGraphByPos(graphs[i].x, graphs[i].y - dirWayY),
              abs(graphs[i].y - (graphs[i].y - dirWayY))));
          break;
        }
        dirWayY++;
      }
    }
    if (map[graphs[i].y + 1][graphs[i].x] != 'b') {  // DOWN DIRECTION
      short int dirWayY = 1;
      while (map[graphs[i].y + dirWayY][graphs[i].x] != 'b' && dirWayY <= 11) {
        if (isGraphExistByPos(graphs[i].x, graphs[i].y + dirWayY)) {
          graphs[i].neighbours.emplace_back(GraphNeighbour<Graph>(
              getGraphByPos(graphs[i].x, graphs[i].y + dirWayY),
              abs(graphs[i].y - (graphs[i].y + dirWayY))));
          break;
        }
        dirWayY++;
      }
    }
  }
}

void getShortPathBetweenGraphsById(short int startID, short int endID) {
  Graph startGraph = graphs[startID];
  Graph endGraph = graphs[endID];
  float diffX = endGraph.x - startGraph.x;
  float diffY = endGraph.y - startGraph.y;
  float distance = sqrtf(diffX * diffX + diffY * diffY);
  Graph neighbourWithMinDistance;
  int count = 0;
  while (distance > 0 && count <= 15) {
    graphs[startGraph.index].isPassed = true;

    // std::cout << "Текущий индекс графа: " << graphs[startGraph.index].index
    //           << std::endl;

    shortPath.emplace_back(Rect(graphs[startGraph.index].x,
                                graphs[startGraph.index].y, 0,
                                sf::Color(0, 255, 255, 150)));

    float minDistance = 10000;
    for (int i = 0; i < graphs[startGraph.index].neighbours.size(); i++) {
      // std::cout << "\tИндексы соседей: "
      //           << graphs[startGraph.neighbours[i].neighbour.index].index
      //           << " Пройден? - "
      //           << graphs[startGraph.neighbours[i].neighbour.index].isPassed
      //           << std::endl;

      float tempDiffX =
          endGraph.x - graphs[startGraph.neighbours[i].neighbour.index].x;
      float tempDiffY =
          endGraph.y - graphs[startGraph.neighbours[i].neighbour.index].y;
      float tempDistance = sqrtf(tempDiffX * tempDiffX + tempDiffY * tempDiffY);

      if (tempDistance <= minDistance &&
          !graphs[startGraph.neighbours[i].neighbour.index].isPassed) {
        minDistance = tempDistance;
        neighbourWithMinDistance =
            graphs[startGraph.neighbours[i].neighbour.index];
      }
    }

    diffX = endGraph.x - neighbourWithMinDistance.x;
    diffY = endGraph.y - neighbourWithMinDistance.y;
    distance = sqrtf(diffX * diffX + diffY * diffY);

    startGraph = graphs[neighbourWithMinDistance.index];

    count++;
  }

  shortPath.emplace_back(
      Rect(endGraph.x, endGraph.y, 0, sf::Color(255, 255, 0, 150)));
}

Graph findClosestGraph(short int x, short int y) {
  Graph tempGraph;
  short int minDistance = 10000;
  for (int i = 0; i < graphs.size(); i++) {
    short int diffX = graphs[i].x - x;
    short int diffY = graphs[i].y - y;
    short int distance = round(sqrt(diffX * diffX + diffY * diffY));
    if (distance <= minDistance) {
      minDistance = distance;
      tempGraph = getGraphByPos(graphs[i].x, graphs[i].y);
    }
  }
  return graphs[tempGraph.index];
}