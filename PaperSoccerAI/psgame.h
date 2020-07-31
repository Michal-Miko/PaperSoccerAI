#ifndef PSGAME_H
#define PSGAME_H

#include "psboard.h"
#include "psnode.h"
#include "psplayer.h"
#include <vector>

class PSGame {
public:
  PSGame();

  void clickedOnField(uint field);

  PSBoard *getBoard() const;

  PSPlayer *getP1() const;
  void setP1(PSPlayer *value);

  PSPlayer *getP2() const;
  void setP2(PSPlayer *value);

private:
  PSBoard *board;

  PSPlayer *p1;
  PSPlayer *p2;
};

#endif // PSGAME_H
