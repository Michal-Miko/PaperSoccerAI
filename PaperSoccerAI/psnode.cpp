#include "psnode.h"

PSNode::PSNode() {
  type = node_type::empty;
  for (int i = 0; i < 8; i++) {
    neighbours[i] = nullptr;
    set_edges[i] = false;
  }
}

PSNode::PSNode(Point pos) : PSNode() { node_pos = pos; }

void PSNode::reset() {
  if (type == node_type::taken)
    type = node_type::empty;

  for (int i = 0; i < 8; i++)
    set_edges[i] = false;
}

std::vector<node_dir> PSNode::getOpenNeighbours() {
  std::vector<node_dir> out;
  for (uint i = 0; i < 8; i++)
    if (neighbours[i] != nullptr && !set_edges[i])
      out.push_back(static_cast<node_dir>(i));
  return out;
}

node_dir PSNode::neighbourDir(PSNode *n) {
  auto pos_diff = n->node_pos - node_pos;

  // check if the given node is a neighbour
  if (pos_diff.x < -1 || pos_diff.x > 1 || pos_diff.y < -1 || pos_diff.y > 1) {
    return node_dir::invalid;
  }

  auto dir = pointToDir.at(pos_diff);
  // check if the node is blocked by an existing edge
  if (set_edges[dir])
    return node_dir::invalid;

  // check if the neighbour exists
  if (neighbours[dir] == nullptr)
    return node_dir::invalid;

  return dir;
}

void PSNode::setNeighbour(PSNode *n, node_dir dir) {
  neighbours[dir] = n;
  n->neighbours[(dir + 4) % 8] = this;
}

void PSNode::removeNeighbour(node_dir dir) {
  neighbours[dir]->neighbours[(dir + 4) % 8] = nullptr;
  neighbours[dir] = nullptr;
}

void PSNode::setEdge(node_dir dir) {
  set_edges[dir] = true;
  neighbours[dir]->set_edges[(dir + 4) % 8] = 1;
}

void PSNode::removeEdge(node_dir dir) {
  set_edges[dir] = false;
  neighbours[dir]->set_edges[(dir + 4) % 8] = 0;
}

Point PSNode::getNode_pos() const { return node_pos; }

void PSNode::setNode_pos(const Point &value) { node_pos = value; }

node_type PSNode::getType() const { return type; }

void PSNode::setType(const node_type &value) { type = value; }

PSNode *PSNode::getNeighbour(node_dir dir) const { return neighbours[dir]; }

bool PSNode::getEdge(node_dir dir) const { return set_edges[dir]; }
