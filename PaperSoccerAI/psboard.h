#ifndef PSBOARD_H
#define PSBOARD_H

#include "psnode.h"
#include <vector>

// p1 - top, p2 - bottom
enum PlayerID { none = 0, p1 = 1, p2 = 2 };

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
  void moveBall(NodeDir dir);
  // Check if the given node is a reachable neighbour of the ball node
  bool ballNeighbour(PSNode *node);
  // Check if a move in the given direction results in a direct victory for the
  // player
  bool moveWins(NodeDir dir, PlayerID p = none);
  // Check if a move in the given direction results in a direct loss for the
  // player
  bool moveLoses(NodeDir dir, PlayerID p = none);
  // Update the turn after a move
  void updateTurn();
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

  void setTurn(const PlayerID &value);
  PlayerID getTurn() const;

  PSNode *getBall_node() const;

  PlayerID getFirst_player() const;
  void setFirst_player(const PlayerID &value);

  bool getAlternate_first() const;
  void setAlternate_first(bool value);

private:
  PlayerID first_player;
  PlayerID turn;
  bool alternate_first;
  std::vector<NodeDir> history;
  std::vector<PSNode *> nodes;
  PSNode *ball_node;
};

#endif // PSBOARD_H
