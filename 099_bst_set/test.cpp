#include <assert.h>

#include <cstdio>
#include <cstdlib>

#include "bstset.h"
#include "set.h"

int main() {
  BstSet<int> set;
  set.add(1);
  assert(set.contains(1));
  set.add(2);
  set.add(2);
  set.add(3);
  set.add(-10);
  assert(!set.contains(19));
}
