#include "psgame.h"

PSGame::PSGame() : board(new PSBoard()), p1(nullptr), p2(nullptr) {}

PSGame::~PSGame() {
  delete board;
  delete p1;
  delete p2;
}

void PSGame::clickedOnNode(uint index) {
  auto clicked_node = board->getNode(index);
  auto clicked_node_dir = board->getBall_node()->neighbourDir(clicked_node);
  bool move_complete = false;

  // Block input if the game is over
  if (gameOver() != PlayerID::none)
    return;

  // Check if the clicked field is next to the ball and isn't blocked
  if (clicked_node_dir == NodeDir::invalid)
    return;

  if (board->getTurn() == PlayerID::p1 && p1->type == PlayerType::player) {
    move_complete = p1->playerInput(clicked_node_dir);
  } else if (board->getTurn() == PlayerID::p2 &&
             p2->type == PlayerType::player) {
    move_complete = p2->playerInput(clicked_node_dir);
  }

  if (move_complete)
    nextTurn();
}

void PSGame::reset() {
  board->resetBoard();
  p1->reset();
  p2->reset();
  if (board->getTurn() == PlayerID::p1 && p1->type != PlayerType::player) {
    auto move = p1->getMove();
    nextTurn();
  } else if (board->getTurn() == PlayerID::p2 &&
             p2->type != PlayerType::player) {
    auto move = p2->getMove();
    nextTurn();
  }
}

void PSGame::undo() {
  if (board->getTurn() == PlayerID::p1 && p1->type == PlayerType::player)
    p1->undoInput();
  else if (board->getTurn() == PlayerID::p2 && p2->type == PlayerType::player)
    p2->undoInput();
}

void PSGame::nextTurn() {
  board->updateTurn();
  if (gameOver() == PlayerID::none) {
    if (board->getTurn() == PlayerID::p1 && p1->type != PlayerType::player) {
      auto move = p1->getMove();
      nextTurn();
    } else if (board->getTurn() == PlayerID::p2 &&
               p2->type != PlayerType::player) {
      auto move = p2->getMove();
      nextTurn();
    }
  }
}

PlayerID PSGame::gameOver() {
  auto ball = board->getBall_node();

  // Ball is stuck (getting the ball stuck ends the turn)
  if (ball->getOpenDirections().size() == 0)
    return board->getTurn();

  // Ball is in a net
  if (ball->getNode_pos() == PSBoard::p1_goal)
    return PlayerID::p2;
  if (ball->getNode_pos() == PSBoard::p2_goal)
    return PlayerID::p1;

  return PlayerID::none;
}

PSBoard *PSGame::getBoard() const { return board; }

PSPlayer *PSGame::getP1() const { return p1; }

void PSGame::setP1(PSPlayer *value) {
  if (p1 != nullptr)
    delete p1;
  p1 = value;
}

PSPlayer *PSGame::getP2() const { return p2; }

void PSGame::setP2(PSPlayer *value) {
  if (p2 != nullptr)
    delete p2;
  p2 = value;
}
