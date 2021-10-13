class Point {
 private:
  double x, y;

 public:
  Point();
  void move(double dx, double dy);
  double distanceFrom(const Point & p);
};
