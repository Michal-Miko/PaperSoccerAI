#include "point.h"
#include "utilities.h"
#include <math.h>

Point::Point() : x(0), y(0) {}

Point::Point(int _x, int _y) : x(_x), y(_y) {}

Point::Point(int i, int w, int h) {
  x = i % w;
  y = i / h;
}

Point &Point::operator+=(const Point &rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}

Point Point::operator+(const Point &rhs) const {
  return Point{x + rhs.x, y + rhs.y};
}

Point Point::operator-(const Point &rhs) const {
  return Point{x - rhs.x, y - rhs.y};
}

Point Point::operator*(const int &rhs) const { return Point{x * rhs, y * rhs}; }

Point Point::operator%(const Point &rhs) const {
  return Point{x % rhs.x, y % rhs.y};
}

bool Point::operator==(const Point &rhs) const {
  return x == rhs.x && y == rhs.y;
}

std::ostream &operator<<(std::ostream &os, const Point &p) {
  os << '(' << p.x << ',' << p.y << ')';
  return os;
}

int Point::toIndex(int width) const { return y * width + x; }

float Point::distance(const Point &p) const {
  return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
}

int Point::distanceMan(const Point &p) const {
  return abs(x - p.x) + abs(y - p.y);
}
