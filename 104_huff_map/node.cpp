#include "node.h"
void Node::buildMap(BitString b, std::map<unsigned, BitString> & theMap) {
  if (this->sym != NO_SYM) {
    assert(this->left == NULL && this->right == NULL);
    theMap[this->sym] = b;
    return;
  }
  this->left->buildMap(b.plusZero(), theMap);
  this->right->buildMap(b.plusOne(), theMap);
}
