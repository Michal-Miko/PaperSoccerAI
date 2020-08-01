#include "psplayer.h"

const QString PSPlayer::name = "Player";

PSPlayer::PSPlayer(PSBoard *b) : board(b), move_complete(false) {}

bool PSPlayer::playerInput(node_dir dir) {
  auto ball = board->getBall_node();
  auto dest = ball->getNeighbour(dir);

  // A move is complete when player gets stuck, ends on an empty node or ends on
  // a net node
  if (dest->getType() == node_type::empty ||
      dest->getOpenNeighbours().size() == 1 ||
      dest->getNode_pos() == PSBoard::p1_goal ||
      dest->getNode_pos() == PSBoard::p2_goal)
    move_complete = true;

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

void PSPlayer::reset() {
  move_complete = false;
  move.clear();
}
