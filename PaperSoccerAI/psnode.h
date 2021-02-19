#ifndef NODE_H
#define NODE_H

#include "point.h"
#include <unordered_map>

enum NodeType { empty, taken, edge };
enum NodeDir {
  n = 0,
  ne = 1,
  e = 2,
  se = 3,
  s = 4,
  sw = 5,
  w = 6,
  nw = 7,
  invalid = 8
};
const std::unordered_map<Point, NodeDir> pointToDir{
    {{0, -1}, NodeDir::n}, {{1, -1}, NodeDir::ne},  {{1, 0}, NodeDir::e},
    {{1, 1}, NodeDir::se}, {{0, 1}, NodeDir::s},    {{-1, 1}, NodeDir::sw},
    {{-1, 0}, NodeDir::w}, {{-1, -1}, NodeDir::nw}, {{0, 0}, NodeDir::invalid}};

class PSNode {
public:
  PSNode();
  PSNode(Point pos);

  void reset();

  // Get directions not blocked by an existing edge
  std::vector<NodeDir> getOpenDirections();
  // Get the direction of a neighbouring node (this->neighbour)
  NodeDir neighbourDir(PSNode *n);
  // Check if moving to this node ends a move
  bool endsMove();

  // ===============
  // Accessors
  // ===============

  void setNeighbour(PSNode *n, NodeDir dir);
  void removeNeighbour(NodeDir dir);

  void setEdge(NodeDir);
  void removeEdge(NodeDir);

  Point getNode_pos() const;
  void setNode_pos(const Point &value);

  NodeType getType() const;
  void setType(const NodeType &value);

  PSNode *getNeighbour(NodeDir dir) const;
  bool getEdge(NodeDir dir) const;

private:
  Point node_pos;
  NodeType type;
  PSNode *neighbors[8];
  bool set_edges[8];
};

#endif // NODE_H
