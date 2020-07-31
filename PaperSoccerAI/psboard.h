#ifndef PSBOARD_H
#define PSBOARD_H

#include "psnode.h"
#include <vector>

// p1 - top, p2 - bottom
enum player { p1, p2 };

class PSBoard {
public:
  PSBoard();

  void setupNodes();
  void setupNeighbours();
  void resetBoard();
  void moveBall(node_dir dir);
  bool ballNeighbour(PSNode *node);
  bool gameOver();
  void nextTurn();

  static const int width;
  static const int height;

  void setNode(int x, int y, PSNode *n);
  PSNode *getNode(int x, int y);
  PSNode *getNode(uint i);

  std::vector<PSNode *> getNodes() const;

  void setTurn(const player &value);
  player getTurn() const;

  PSNode *getBall_node() const;

private:
  std::vector<PSNode *> nodes;
  PSNode *ball_node;
  player turn;
};

#endif // PSBOARD_H
