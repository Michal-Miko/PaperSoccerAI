#ifndef PSGAME_H
#define PSGAME_H

#include "psaiplayer.h"
#include "psboard.h"
#include "psnode.h"
#include "psplayer.h"
#include "psplayokplayer.h"
#include "psrandomplayer.h"
#include <vector>

class PSGame {
public:
  PSGame();
  ~PSGame();

  // Handle player input
  void clickedOnNode(uint index);

  // Reset the game
  void reset();

  // Undo the last part of a move if it's not yet complete
  void undo();

  // Process next turn
  void nextTurn();

  // Check if the game is over, return the winner
  PlayerID gameOver();

  // ===============
  // Accessors
  // ===============

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
