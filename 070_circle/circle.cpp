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
  double d1 = (this->r * this->r - c.r * c.r + d * d) / (2 * d);
  double d2 = d - d1;
  double acos_1 = std::acos(d1 / this->r);
  double acos_2 = std::acos(d2 / c.r);
  return this->r * this->r * acos_1 + c.r * c.r * acos_2 -
         0.5 * std::sqrt((-d + this->r + c.r) * (d + this->r - c.r) *
                         (d - this->r + c.r) * (d + this->r + c.r));
}
