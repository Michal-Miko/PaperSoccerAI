#ifndef NODE_H
#define NODE_H

#include "point.h"
#include <unordered_map>

enum node_type { empty, taken, edge };
enum node_dir {
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
const std::unordered_map<Point, node_dir> pointToDir{
    {{0, -1}, node_dir::n},     {{1, -1}, node_dir::ne},
    {{1, 0}, node_dir::e},      {{1, 1}, node_dir::se},
    {{0, 1}, node_dir::s},      {{-1, 1}, node_dir::sw},
    {{-1, 0}, node_dir::w},     {{-1, -1}, node_dir::nw},
    {{0, 0}, node_dir::invalid}};

class PSNode {
public:
  PSNode();
  PSNode(Point pos);

  void reset();

  std::vector<node_dir> getOpenNeighbours();
  node_dir neighbourDir(PSNode *n);

  void setNeighbour(PSNode *n, node_dir dir);
  void removeNeighbour(node_dir dir);

  void setEdge(node_dir);
  void removeEdge(node_dir);

  Point getNode_pos() const;
  void setNode_pos(const Point &value);

  node_type getType() const;
  void setType(const node_type &value);

  PSNode *getNeighbour(node_dir dir) const;
  bool getEdge(node_dir dir) const;

private:
  Point node_pos;
  node_type type;
  PSNode *neighbours[8];
  bool set_edges[8];
};

#endif // NODE_H
