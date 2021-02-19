#include "psnode.h"

PSNode::PSNode() {
  type = NodeType::empty;
  for (int i = 0; i < 8; i++) {
    neighbors[i] = nullptr;
    set_edges[i] = false;
  }
}

PSNode::PSNode(Point pos) : PSNode() { node_pos = pos; }

void PSNode::reset() {
  if (type == NodeType::taken)
    type = NodeType::empty;

  for (int i = 0; i < 8; i++)
    set_edges[i] = false;
}

std::vector<NodeDir> PSNode::getOpenDirections() {
  std::vector<NodeDir> out;
  for (uint i = 0; i < 8; i++)
    if (neighbors[i] != nullptr && !set_edges[i])
      out.push_back(static_cast<NodeDir>(i));
  return out;
}

NodeDir PSNode::neighbourDir(PSNode *n) {
  auto pos_diff = n->node_pos - node_pos;

  // check if the given node is a neighbour
  if (pos_diff.x < -1 || pos_diff.x > 1 || pos_diff.y < -1 || pos_diff.y > 1) {
    return NodeDir::invalid;
  }

  auto dir = pointToDir.at(pos_diff);
  // check if the node is blocked by an existing edge
  if (set_edges[dir])
    return NodeDir::invalid;

  // check if the neighbour exists
  if (neighbors[dir] == nullptr)
    return NodeDir::invalid;

  return dir;
}

bool PSNode::endsMove() {
  // A move is complete when player gets stuck, ends on an empty node,
  // or ends on a net node (net nodes have 3 neighbors, 2 of which are edge
  // nodes)
  uint neighbors_count = 0;
  uint edge_neighbors = 0;
  for (uint i = 0; i < 8; i++) {
    if (neighbors[i] != nullptr) {
      ++neighbors_count;
      if (neighbors[i]->getType() == NodeType::edge)
        ++edge_neighbors;
    }
  }

  return getType() == NodeType::empty || getOpenDirections().size() == 1 ||
         (neighbors_count == 3 && edge_neighbors == 2);
}
void PSNode::setNeighbour(PSNode *n, NodeDir dir) {
  neighbors[dir] = n;
  n->neighbors[(dir + 4) % 8] = this;
}

void PSNode::removeNeighbour(NodeDir dir) {
  neighbors[dir]->neighbors[(dir + 4) % 8] = nullptr;
  neighbors[dir] = nullptr;
}

void PSNode::setEdge(NodeDir dir) {
  set_edges[dir] = true;
  neighbors[dir]->set_edges[(dir + 4) % 8] = 1;
}

void PSNode::removeEdge(NodeDir dir) {
  set_edges[dir] = false;
  neighbors[dir]->set_edges[(dir + 4) % 8] = 0;
}

Point PSNode::getNode_pos() const { return node_pos; }

void PSNode::setNode_pos(const Point &value) { node_pos = value; }

NodeType PSNode::getType() const { return type; }

void PSNode::setType(const NodeType &value) { type = value; }

PSNode *PSNode::getNeighbour(NodeDir dir) const { return neighbors[dir]; }

bool PSNode::getEdge(NodeDir dir) const { return set_edges[dir]; }
