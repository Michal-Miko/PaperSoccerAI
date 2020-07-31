#include "psboard.h"

const int PSBoard::width = 9;
const int PSBoard::height = 13;

PSBoard::PSBoard() {
  // Set turn
  turn = p1;

  // Initialize empty board
  setupNodes();

  // Initialize neighbours
  setupNeighbours();

  // Put the ball in the middle
  ball_node = getNode(width / 2, height / 2);
  ball_node->setType(node_type::taken);
}

void PSBoard::setupNodes() {
  // Set up node types
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Point node_pos = Point(x, y);
      PSNode *new_node = new PSNode(node_pos);
      nodes.push_back(new_node);

      // Set board edges
      if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
        new_node->setType(node_type::edge);
      else if ((y == 1 || y == height - 2) && (x < width / 2 || x > width / 2))
        new_node->setType(node_type::edge);
    }
  }
  // Set top/bottom net edges
  getNode(width / 2 - 1, 1)->setType(node_type::edge);
  getNode(width / 2 + 1, 1)->setType(node_type::edge);
  getNode(width / 2 - 1, height - 2)->setType(node_type::edge);
  getNode(width / 2 + 1, height - 2)->setType(node_type::edge);
}

void PSBoard::setupNeighbours() {
  for (unsigned long i = 0; i < nodes.size(); i++) {
    auto node = nodes[i];
    Point pos = node->getNode_pos();
    if (node->getType() != node_type::edge) {
      // Internal nodes
      node->setNeighbour(getNode(pos.x, pos.y - 1), node_dir::n);
      node->setNeighbour(getNode(pos.x + 1, pos.y - 1), node_dir::ne);
      node->setNeighbour(getNode(pos.x + 1, pos.y), node_dir::e);
      node->setNeighbour(getNode(pos.x + 1, pos.y + 1), node_dir::se);
      node->setNeighbour(getNode(pos.x, pos.y + 1), node_dir::s);
      node->setNeighbour(getNode(pos.x - 1, pos.y + 1), node_dir::sw);
      node->setNeighbour(getNode(pos.x - 1, pos.y), node_dir::w);
      node->setNeighbour(getNode(pos.x - 1, pos.y - 1), node_dir::nw);
    }
  }
  // Corner edges
  getNode(1, 1)->setNeighbour(getNode(0, 2), node_dir::sw);
  getNode(1, height - 2)->setNeighbour(getNode(0, height - 3), node_dir::nw);
  getNode(width - 2, 1)->setNeighbour(getNode(width - 1, 2), node_dir::se);
  getNode(width - 2, height - 2)
      ->setNeighbour(getNode(width - 1, height - 3), node_dir::ne);
  // Net corner edges
  getNode(3, 1)->setNeighbour(getNode(4, 0), node_dir::ne);
  getNode(5, 1)->setNeighbour(getNode(4, 0), node_dir::nw);
  getNode(3, height - 2)->setNeighbour(getNode(4, height - 1), node_dir::se);
  getNode(5, height - 2)->setNeighbour(getNode(4, height - 1), node_dir::sw);
}

void PSBoard::resetBoard() {
  // Reset nodes
  for (unsigned long i = 0; i < nodes.size(); i++)
    nodes[i]->reset();

  // Put the ball in the middle
  getNode(width / 2, height / 2)->setType(node_type::taken);
}

void PSBoard::moveBall(node_dir dir) {
  ball_node->setEdge(dir);
  ball_node = ball_node->getNeighbour(dir);
  ball_node->setType(node_type::taken);
}

bool PSBoard::ballNeighbour(PSNode *node) {
  for (uint i = 0; i < 8; i++) {
    auto neighbour = node->getNeighbour(static_cast<node_dir>(i));
    if (neighbour == ball_node && !node->getEdge(static_cast<node_dir>(i)))
      return true;
  }
  return false;
}

void PSBoard::nextTurn() {
  if (turn == player::p1)
    turn = player::p2;
  else
    turn = player::p1;
}

void PSBoard::setNode(int x, int y, PSNode *n) { nodes[width * y + x] = n; }

PSNode *PSBoard::getNode(int x, int y) { return nodes[width * y + x]; }

PSNode *PSBoard::getNode(uint i) { return nodes[i]; }

std::vector<PSNode *> PSBoard::getNodes() const { return nodes; }

void PSBoard::setTurn(const player &value) { turn = value; }

player PSBoard::getTurn() const { return turn; }

PSNode *PSBoard::getBall_node() const { return ball_node; }
