#include "circle.hpp"

#include <cmath>

Circle::Circle(Point p_init, double r_init) : p(p_init), r(r_init) {
}

void Circle::move(double dx, double dy) {
  this->p.move(dx, dy);
}

double Circle::intersectionArea(const Circle & c) {
  double d = this->p.distanceFrom(c.p);
  if (d > (this->r + c.r))
    return 0;
  if (d <= std::abs(this->r - c.r)) {
    double r_min = (this->r < c.r) ? this->r : c.r;
    return M_PI * r_min * r_min;
  }
  double d1 = this->r * this->r - c.r * c.r + d * d / 2 * d;
  double d2 = d - d1;
  double acos_1 = std::acos(d1 / this->r);
  double acos_2 = std::acos(d2 / c.r);
  double squared_diff_1 = std::sqrt(this->r * this->r - d1 * d1);
  double squared_diff_2 = std::sqrt(c.r * c.r - d2 * d2);
  return this->r * this->r * acos_1 - d1 * squared_diff_1 + c.r * c.r * acos_2 -
         d2 * squared_diff_2;
}
