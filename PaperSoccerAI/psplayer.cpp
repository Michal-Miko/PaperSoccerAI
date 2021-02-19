#include "psplayer.h"

PSPlayer::PSPlayer(PSBoard *b)
    : name("Player"), type(PlayerType::player), board(b), move_complete(false) {
}

PSPlayer::~PSPlayer() {}

bool PSPlayer::playerInput(NodeDir dir) {
  auto ball = board->getBall_node();
  auto dest = ball->getNeighbour(dir);

  // A move is complete when player gets stuck, ends on an empty node or ends on
  // a net node
  move_complete = dest->endsMove();
  move.push_back(dir);
  board->moveBall(dir);

  return move_complete;
}

std::vector<NodeDir> PSPlayer::getMove() {
  if (move_complete == true) {
    move.clear();
    move_complete = false;
  }

  return move;
}

PSPlayer::PSPlayer(PSBoard *b, QString name, PlayerType type)
    : name(name), type(type), board(b), move_complete(false) {}

void PSPlayer::undoInput() {
  if (move.size() > 0) {
    move.pop_back();
    board->undo();
  }
}

void PSPlayer::reset() {
  move_complete = false;
  move.clear();
}
