#include "psplayer.h"

const QString PSPlayer::name = "Player";

PSPlayer::PSPlayer(PSBoard *b) : board(b), move_complete(false) {}

bool PSPlayer::playerInput(node_dir dir) {
  auto ball = board->getBall_node();
  if (ball->getNeighbour(dir)->getType() == node_type::empty)
    move_complete = true;
  // TODO: check if the player is stuck in a corner without moves

  move.push_back(dir);
  board->moveBall(dir);

  return move_complete;
}

std::vector<node_dir> PSPlayer::getMove() {
  if (move_complete == true) {
    move.clear();
    move_complete = false;
  }

  return move;
}

void PSPlayer::undoInput() {
  if (move.size() > 0) {
    move.pop_back();
    board->undo();
  }
}
