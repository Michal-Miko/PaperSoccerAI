#ifndef PSRANDOMPLAYER_H
#define PSRANDOMPLAYER_H

#include "psplayer.h"

class PSRandomPlayer : public PSPlayer {
public:
  PSRandomPlayer(PSBoard *b);
  virtual std::vector<NodeDir> getMove() override;

private:
  // Get a random direction that won't directly result in a loss (if possible)
  NodeDir getRandomDir();
};

#endif // PSRANDOMPLAYER_H
