#ifndef POINT_H
#define POINT_H

#include "utilities.h"
#include <functional>
#include <ostream>

class Point {
public:
  int x, y;

  Point();
  Point(int _x, int _y);
  Point(int i, int w, int h);

  Point &operator+=(const Point &rhs);
  Point operator+(const Point &rhs) const;
  Point operator-(const Point &rhs) const;
  Point operator*(const int &rhs) const;
  Point operator%(const Point &rhs) const;
  bool operator==(const Point &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const Point &p);

  // Convert a 2d array index to a 1d index
  int toIndex(int width) const;

  // Distance between two points
  float distance(const Point &p) const;
  // Manhattan distance between two points
  int distanceMan(const Point &p) const;
};

namespace std {

template <> struct hash<Point> {
  std::size_t operator()(const Point &p) const {
    std::hash<int> hasher;
    std::size_t seed = (167 + hasher(p.x)) * 167 + hasher(p.y);
    return seed;
  }
};

} // namespace std
#endif // POINT_H
