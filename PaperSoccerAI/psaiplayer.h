#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "psplayer.h"

class PSAIPlayer : public PSPlayer {
public:
  PSAIPlayer(PSBoard *b);
  virtual std::vector<NodeDir> getMove() override;

private:
};

#endif // AIPLAYER_H
