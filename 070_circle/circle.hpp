#include "point.hpp"

class Circle {
 private:
  Point p;
  const double r;

 public:
  Circle(Point p_init, double r_init);
  void move(double dx, double dy);
  double intersectionArea(const Circle & c);
};
