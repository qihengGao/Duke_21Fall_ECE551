#include "point.hpp"

#include <cmath>
Point::Point() : x(0), y(0) {
}

void Point::move(double dx, double dy) {
  this->x += dx;
  this->y += dy;
}

double Point::distanceFrom(const Point & p) {
  double dx = p.x - this->x;
  double dy = p.y - this->y;
  return std::sqrt(dx * dx + dy * dy);
}
