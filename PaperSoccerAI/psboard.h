#ifndef PSBOARD_H
#define PSBOARD_H

#include "psnode.h"
#include <vector>

// p1 - top, p2 - bottom
enum player { none = 0, p1 = 1, p2 = 2 };

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
  void undo();

  static const int width;
  static const int height;
  static const Point p1_goal;
  static const Point p2_goal;

  void setNode(int x, int y, PSNode *n);
  PSNode *getNode(int x, int y);
  PSNode *getNode(uint i);

  std::vector<PSNode *> getNodes() const;

  void setTurn(const player &value);
  player getTurn() const;

  PSNode *getBall_node() const;

  player getFirst_player() const;
  void setFirst_player(const player &value);

  bool getAlternate_first() const;
  void setAlternate_first(bool value);

private:
  player first_player;
  player turn;
  bool alternate_first;
  std::vector<node_dir> history;
  std::vector<PSNode *> nodes;
  PSNode *ball_node;
};

#endif // PSBOARD_H
