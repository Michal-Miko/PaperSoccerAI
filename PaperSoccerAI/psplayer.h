#ifndef PLAYER_H
#define PLAYER_H

#include "psboard.h"
#include "psnode.h"
#include <QString>
#include <vector>

class PSPlayer {
public:
  PSPlayer(PSBoard *b);

  // Returns true if the move is complete -- the ball is in the final position,
  // false otherwise
  bool playerInput(node_dir dir);
  // Undoes the last input if the move is not complete
  void undoInput();

  std::vector<node_dir> getMove();

  static const QString name;

private:
  PSBoard *board;
  std::vector<node_dir> move;
  bool move_complete;
};

#endif // PLAYER_H
