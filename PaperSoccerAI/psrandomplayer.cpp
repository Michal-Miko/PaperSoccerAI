#include "psrandomplayer.h"
#include <QRandomGenerator>

PSRandomPlayer::PSRandomPlayer(PSBoard *b)
    : PSPlayer(b, "Random", PlayerType::ai_random) {}

std::vector<NodeDir> PSRandomPlayer::getMove() {
  auto chosen_dir = getRandomDir();
  move_complete = board->getBall_node()->getNeighbour(chosen_dir)->endsMove();
  move.push_back(chosen_dir);
  board->moveBall(chosen_dir);

  while (!move_complete) {
    chosen_dir = getRandomDir();
    move_complete = board->getBall_node()->getNeighbour(chosen_dir)->endsMove();
    move.push_back(chosen_dir);
    board->moveBall(chosen_dir);
  }

  return move;
}

NodeDir PSRandomPlayer::getRandomDir() {
  auto ball = board->getBall_node();
  std::vector<NodeDir> possible_dirs = ball->getOpenDirections();
  std::vector<NodeDir> good_dirs;

  for (const auto &dir : possible_dirs) {
    // Move wins the game - return it
    if (board->moveWins(dir))
      return dir;
    // Move doesn't lose the game - add it to good choices
    else if (!board->moveLoses(dir))
      good_dirs.push_back(dir);
  }

  // Found some non-losing moves
  if (good_dirs.size() > 0)
    return good_dirs[QRandomGenerator::global()->bounded(
        static_cast<int>(good_dirs.size()))];

  // Couldn't find any non-losing moves
  return possible_dirs[QRandomGenerator::global()->bounded(
      static_cast<int>(possible_dirs.size()))];
}
