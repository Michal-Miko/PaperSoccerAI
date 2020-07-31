#include "psplayer.h"

const QString PSPlayer::name = "Player";

PSPlayer::PSPlayer(PSBoard *b) : board(b) {}

bool PSPlayer::playerInput(node_dir dir) {
  bool move_complete = false;

  auto ball = board->getBall_node();
  if (ball->getNeighbour(dir)->getType() == node_type::empty)
    move_complete = true;
  // TODO: check if the player is stuck in a corner without moves

  move.push_back(dir);
  board->moveBall(dir);

  return move_complete;
}

std::vector<node_dir> PSPlayer::getMove() { return move; }

void PSPlayer::undoInput() { move.pop_back(); }
