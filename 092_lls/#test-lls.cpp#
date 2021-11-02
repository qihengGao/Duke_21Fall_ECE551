#include <assert.h>

#include <cstdlib>
#include <iostream>

#include "ll.hpp"

class Tester {
 public:
  // testing for default constructor is done for you
  void testDefCtor() {
    LinkedList<int> il;
    assert(il.head == NULL);
    assert(il.tail == NULL);
    assert(il.getSize() == 0);
  }
  // example of another method you might want to write
  void testAddFront() {
    // WRITE ME
    LinkedList<int> il;
    il.addFront(23);
    assert(il.head == il.tail);
    assert(il.getSize() == 1);
    assert(il.head->data == 23);
    assert(il.tail->data == 23);
    assert(il.head->prev == NULL);
    assert(il.head->next == NULL);
    assert(il.tail->prev == NULL);
    assert(il.tail->next == NULL);

    il.addFront(42);
    assert(il.head->next == il.tail);
    assert(il.head->prev == NULL);
    assert(il.tail->next == NULL);
    assert(il.tail->prev == il.head);
    assert(il.head->data == 42);
    assert(il.tail->data == 23);
    assert(il.getSize() == 2);
  }
  // many more tester methods
  void testAddBack() {
    LinkedList<int> il;
    il.addBack(23);
    assert(il.head == il.tail);
    assert(il.getSize() == 1);
    assert(il.head->data == 23);
    assert(il.tail->data == 23);
    assert(il.head->prev == NULL);
    assert(il.head->next == NULL);
    assert(il.tail->prev == NULL);
    assert(il.tail->next == NULL);

    il.addBack(42);
    assert(il.head->next == il.tail);
    assert(il.head->next->prev == il.head);
    assert(il.head->next->next == NULL);
    assert(il.tail->prev->prev == NULL);
    assert(il.head->prev == NULL);
    assert(il.tail->next == NULL);
    assert(il.tail->prev->next == il.tail);
    assert(il.tail->prev == il.head);
    assert(il.head != il.tail);
    assert(il.head->data == 23);
    assert(il.tail->data == 42);
    assert(il.getSize() == 2);
  }

  void testRuleThree() {
    LinkedList<int> il1;
    il1.addBack(1);
    il1.addBack(3);
    il1.addBack(7);
    LinkedList<int> il2;
    il2 = il1;
    il2.remove(3);
    assert(il1.getSize() == 3);
    il2.addBack(3);
    assert(il1.getSize() == 3);
    il1.remove(1);
    assert(il2.getSize() == 3);
    il2.remove(1);
    il2.remove(3);
    il2 = il1;
    assert(il2.getSize() == 2);
    il1.addFront(1);
    LinkedList<int> il3(il1);
    il3.remove(3);
    assert(il1.getSize() == 3);

    il1.addBack(8);
  }

  void testRemove() {
    LinkedList<int> il1;
    assert(!il1.remove(1));
    il1.addFront(1);  // 1
    assert(!il1.remove(2));
    assert(il1.head->data == 1);
    assert(il1.tail->data == 1);
    assert(il1.remove(1));
    assert(il1.getSize() == 0 && il1.head == NULL && il1.tail == NULL);
    assert(!il1.remove(1));

    il1.addFront(1);
    il1.addBack(2);
    il1.addBack(3);
    il1.addBack(4);
    // remove head
    il1.remove(1);  // 2 3 4
    assert(il1.getSize() == 3 && il1.head->data == 2 && il1.head->prev == NULL);
    assert(il1.head->next->data == 3);
    assert(!il1.remove(1));

    LinkedList<int> il3;
    il3.addFront(1);
    il3.addFront(2);
    il3.addFront(3);
    il3.addFront(4);  // 4 3 2 1
    // remove tail
    assert(il3.remove(1));
    assert(il3.getSize() == 3 && il3.tail->data == 2 && il3.tail->next == NULL);

    LinkedList<int> il2;
    il2.addBack(1);
    il2.addBack(1);
    il2.addBack(1);
    assert(il2.remove(1));
    assert(il2.getSize() == 2);
    assert(il2.head->prev == NULL);
    assert(il2.tail->next == NULL);
    assert(il2.remove(1));

    LinkedList<int> il4;
    il4.addBack(1);
    il4.addBack(2);
    il4.addBack(3);
    assert(il4.remove(2));
    assert(il4.getSize() == 2 && il4.head->data == 1 && il4.tail->data == 3);
    assert(il4.head->next == il4.tail && il4.tail->prev == il4.head);
  }
};

int main(void) {
  Tester t;
  t.testDefCtor();
  t.testAddFront();
  t.testAddBack();
  t.testRuleThree();
  t.testRemove();
  return EXIT_SUCCESS;
}
