#include <assert.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "function.h"
#include "search.cpp"
int binarySearchForZero(Function<int, int> * f, int low, int high);

class MyFunc : public Function<int, int> {
  virtual int invoke(int arg) { return arg; }
};

class CountedIntFn : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),
      f(fn),
      mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};

void check(Function<int, int> * f,
           int low,
           int high,
           int expected_ans,
           const char * mesg) {
  int invokeAllowed = (high > low) ? log2(high - low) + 1 : 1;
  CountedIntFn count(invokeAllowed, f, mesg);
  int ans = binarySearchForZero(&count, low, high);
  assert(ans == expected_ans);
}

int main() {
  MyFunc mf;
  check(&mf, -1, 23, 0, "0 is included.");
  check(&mf, -8, -2, -3, "All nums are negative.");
  check(&mf, 23, 42, 23, "All nums are positive.");
  check(&mf, 0, 4, 0, "0 is on left.");
  check(&mf, -9, 0, -1, "0 is on right.");
  check(&mf, -1, 0, -1, "singleton.");
}
