#include <cstdio>
#include <cstdlib>

#include "bstset.h"
#include "set.h"

int main() {
  BstSet<int> set;
  set.add(1);
  assert(set.contains(1));
}
