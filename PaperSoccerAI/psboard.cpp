#include "psboard.h"

const int PSBoard::width = 9;
const int PSBoard::height = 13;
const Point PSBoard::p1_goal{PSBoard::width / 2, 0};
const Point PSBoard::p2_goal{PSBoard::width / 2, PSBoard::height - 1};

PSBoard::PSBoard()
    : first_player(p1), turn(first_player), alternate_first(true) {
  // Initialize empty board
  setupNodes();

  // Initialize neighbours
  setupNeighbours();

  // Put the ball in the middle
  ball_node = getNode(width / 2, height / 2);
  ball_node->setType(NodeType::taken);
}

PSBoard::~PSBoard() {
  for (auto *node : nodes)
    delete node;
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
        new_node->setType(NodeType::edge);
      else if ((y == 1 || y == height - 2) && (x < width / 2 || x > width / 2))
        new_node->setType(NodeType::edge);
    }
  }
  // Set top/bottom net edges
  getNode(width / 2 - 1, 1)->setType(NodeType::edge);
  getNode(width / 2 + 1, 1)->setType(NodeType::edge);
  getNode(width / 2 - 1, height - 2)->setType(NodeType::edge);
  getNode(width / 2 + 1, height - 2)->setType(NodeType::edge);
}

void PSBoard::setupNeighbours() {
  for (unsigned long i = 0; i < nodes.size(); i++) {
    auto node = nodes[i];
    Point pos = node->getNode_pos();
    if (node->getType() != NodeType::edge) {
      // Internal nodes
      node->setNeighbour(getNode(pos.x, pos.y - 1), NodeDir::n);
      node->setNeighbour(getNode(pos.x + 1, pos.y - 1), NodeDir::ne);
      node->setNeighbour(getNode(pos.x + 1, pos.y), NodeDir::e);
      node->setNeighbour(getNode(pos.x + 1, pos.y + 1), NodeDir::se);
      node->setNeighbour(getNode(pos.x, pos.y + 1), NodeDir::s);
      node->setNeighbour(getNode(pos.x - 1, pos.y + 1), NodeDir::sw);
      node->setNeighbour(getNode(pos.x - 1, pos.y), NodeDir::w);
      node->setNeighbour(getNode(pos.x - 1, pos.y - 1), NodeDir::nw);
    }
  }
  // Corner edges
  getNode(1, 1)->setNeighbour(getNode(0, 2), NodeDir::sw);
  getNode(1, height - 2)->setNeighbour(getNode(0, height - 3), NodeDir::nw);
  getNode(width - 2, 1)->setNeighbour(getNode(width - 1, 2), NodeDir::se);
  getNode(width - 2, height - 2)
      ->setNeighbour(getNode(width - 1, height - 3), NodeDir::ne);
  // Net corner edges
  getNode(3, 1)->setNeighbour(getNode(4, 0), NodeDir::ne);
  getNode(5, 1)->setNeighbour(getNode(4, 0), NodeDir::nw);
  getNode(3, height - 2)->setNeighbour(getNode(4, height - 1), NodeDir::se);
  getNode(5, height - 2)->setNeighbour(getNode(4, height - 1), NodeDir::sw);
}

void PSBoard::resetBoard() {
  // Set turn
  turn = first_player;

  // Alternate first_player;
  if (alternate_first && first_player == p1)
    first_player = p2;
  else if (alternate_first && first_player == p2)
    first_player = p1;

  // Reset nodes
  for (unsigned long i = 0; i < nodes.size(); i++)
    nodes[i]->reset();

  // Put the ball in the middle
  ball_node = getNode(width / 2, height / 2);
  ball_node->setType(NodeType::taken);
}

void PSBoard::moveBall(NodeDir dir) {
  ball_node->setEdge(dir);
  ball_node = ball_node->getNeighbour(dir);
  if (ball_node->getType() == NodeType::empty)
    ball_node->setType(NodeType::taken);
  history.push_back(dir);
}

bool PSBoard::ballNeighbour(PSNode *node) {
  for (uint i = 0; i < 8; i++) {
    auto neighbour = node->getNeighbour(static_cast<NodeDir>(i));
    if (neighbour == ball_node && !node->getEdge(static_cast<NodeDir>(i)))
      return true;
    else if (neighbour == ball_node)
      return false;
  }
  return false;
}

bool PSBoard::moveWins(NodeDir dir, PlayerID p) {
  // Default player
  if (p == none)
    p = turn;

  // Enemy gets stuck
  if (turn != p &&
      ball_node->getNeighbour(dir)->getOpenDirections().size() == 1)
    return true;

  // Ball in enemy's net
  if (p == p1 &&
      ball_node->getNeighbour(dir)->getNode_pos() == PSBoard::p2_goal)
    return true;
  if (p == p2 &&
      ball_node->getNeighbour(dir)->getNode_pos() == PSBoard::p1_goal)
    return true;

  return false;
}

bool PSBoard::moveLoses(NodeDir dir, PlayerID p) {
  // Default player
  if (p == none)
    p = turn;

  // Player gets stuck
  if (turn == p &&
      ball_node->getNeighbour(dir)->getOpenDirections().size() == 1)
    return true;

  // Ball in player's net
  if (p == p1 &&
      ball_node->getNeighbour(dir)->getNode_pos() == PSBoard::p1_goal)
    return true;
  if (p == p2 &&
      ball_node->getNeighbour(dir)->getNode_pos() == PSBoard::p2_goal)
    return true;

  return false;
}

void PSBoard::updateTurn() {
  if (turn == p1)
    turn = p2;
  else if (turn == p2)
    turn = p1;
}

void PSBoard::undo() {
  NodeDir last_dir = history.back();
  NodeDir reverse_dir = static_cast<NodeDir>((last_dir + 4) % 8);
  history.pop_back();

  auto old_ball_node = ball_node;
  ball_node = ball_node->getNeighbour(reverse_dir);
  ball_node->removeEdge(last_dir);

  // Change node type if the undo causued it to become disconnected
  if (old_ball_node->getType() == NodeType::taken) {
    bool disconnected = true;
    for (int i = 0; i < 8; i++) {
      if (old_ball_node->getEdge(static_cast<NodeDir>(i))) {
        disconnected = false;
        break;
      }
    }
    if (disconnected)
      old_ball_node->setType(NodeType::empty);
  }
}

void PSBoard::setNode(int x, int y, PSNode *n) {
  if (nodes[width * y + x] != nullptr)
    delete nodes[width * y + x];
  nodes[width * y + x] = n;
}

PSNode *PSBoard::getNode(int x, int y) { return nodes[width * y + x]; }

PSNode *PSBoard::getNode(uint i) { return nodes[i]; }

std::vector<PSNode *> PSBoard::getNodes() const { return nodes; }

void PSBoard::setTurn(const PlayerID &value) { turn = value; }

PlayerID PSBoard::getTurn() const { return turn; }

PSNode *PSBoard::getBall_node() const { return ball_node; }

PlayerID PSBoard::getFirst_player() const { return first_player; }

void PSBoard::setFirst_player(const PlayerID &value) { first_player = value; }

bool PSBoard::getAlternate_first() const { return alternate_first; }

void PSBoard::setAlternate_first(bool value) { alternate_first = value; }
