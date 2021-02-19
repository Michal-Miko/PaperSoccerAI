#include "psaiplayer.h"

PSAIPlayer::PSAIPlayer(PSBoard *b) : PSPlayer(b, "AI", PlayerType::ai) {}

std::vector<NodeDir> PSAIPlayer::getMove() {
  // TODO
  return std::vector<NodeDir>{};
}
