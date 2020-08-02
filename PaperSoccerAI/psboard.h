#ifndef PSBOARD_H
#define PSBOARD_H

#include "psnode.h"
#include <vector>

// p1 - top, p2 - bottom
enum player { none = 0, p1 = 1, p2 = 2 };

class PSBoard {
public:
  PSBoard();
  ~PSBoard();

  // Create all the board nodes
  void setupNodes();
  // Add neighbours to the board nodes
  void setupNeighbours();
  // Reset the board nodes, set turn
  void resetBoard();
  // Move the ball, adding the edges it creates to the graph and save the move
  // to history
  void moveBall(node_dir dir);
  // Check if the given node is a reachable neighbour of the ball node
  bool ballNeighbour(PSNode *node);
  // Update the turn after a move
  void nextTurn();
  // Undo the last move in history
  void undo();

  // ===============
  // Constants
  // ===============

  // Board dimensions
  static const int width;
  static const int height;
  // Net positions
  static const Point p1_goal;
  static const Point p2_goal;

  // ===============
  // Accessors
  // ===============

  // Modify node at the given position
  void setNode(int x, int y, PSNode *n);
  // Retrieve node at the given position
  PSNode *getNode(int x, int y);
  // Retrieve node with the given index
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
