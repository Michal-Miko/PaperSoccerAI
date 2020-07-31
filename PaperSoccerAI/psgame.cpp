#include "psgame.h"

PSGame::PSGame() { board = new PSBoard(); }

void PSGame::clickedOnField(uint field) {
  auto clickedNode = board->getNode(field);
  auto clicked_node_dir = board->getBall_node()->neighbourDir(clickedNode);
  bool move_complete = false;
  std::vector<node_dir> move;

  // check if the clicked field is next to the ball and isnt blocked
  if (clicked_node_dir == node_dir::invalid)
    return;

  if (board->getTurn() == player::p1 && p1->name == "Player") {
    move_complete = p1->playerInput(clicked_node_dir);
    move = p1->getMove();
  } else if (board->getTurn() == player::p2 && p2->name == "Player") {
    move_complete = p2->playerInput(clicked_node_dir);
    move = p2->getMove();
  }

  if (move_complete)
    board->nextTurn();
}

PSBoard *PSGame::getBoard() const { return board; }

PSPlayer *PSGame::getP1() const { return p1; }

void PSGame::setP1(PSPlayer *value) { p1 = value; }

PSPlayer *PSGame::getP2() const { return p2; }

void PSGame::setP2(PSPlayer *value) { p2 = value; }