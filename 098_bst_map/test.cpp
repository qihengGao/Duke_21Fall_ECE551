#include <assert.h>

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "bstmap.h"
#include "map.h"

int main() {
  BstMap<int, int> bstmap;
  try {
    bstmap.add(1, 1);
    bstmap.add(-10, 2);
    bstmap.add(9, 8);
    bstmap.add(10, 10);
    bstmap.add(10, 23);
    bstmap.add(11, 9);
    bstmap.add(9, 42);

    assert(bstmap.lookup(10) == 23);
    bstmap.remove(9);
    bstmap.remove(111);
    bstmap.lookup(120);
  }
  catch (std::exception & e) {
    std::cout << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
