#include "map.h"

template<typename K, typename V>
class BstMap : public Map<K, V> {
  class Node {
   public:
    K key;
    V value;
    Node * left;
    Node * right;
    Node(const K & key, const V & value) :
        key(key),
        value(value),
        left(NULL),
        right(NULL) {}
  };
  Node * root;

  Node ** getNode(const K & key) const;
  void destroy(Node * curr);
  Node * copy(const Node * curr);

 public:
  BstMap() : root(NULL) {}
  BstMap(const BstMap<K, V> & rhs);
  BstMap<K, V> & operator=(const BstMap<K, V> & rhs);
  virtual ~BstMap();
  virtual void add(const K & key, const V & value);
  virtual const V & lookup(const K & key) const throw(std::invalid_argument);
  virtual void remove(const K & key);
};

// getNode helper
template<typename K, typename V>
typename BstMap<K, V>::Node ** BstMap<K, V>::getNode(const K & key) const {
  Node ** p = const_cast<Node **>(&(this->root));
  while (*p != NULL) {
    if (key < (*p)->key) {
      p = &((*p)->left);
    }
    else if (key > (*p)->key) {
      p = &((*p)->right);
    }
    else {
      break;
    }
  }
  return p;
}

// copy constructor
template<typename K, typename V>
BstMap<K, V>::BstMap(const BstMap<K, V> & rhs) {
  this->root = copy(rhs.root);
}

template<typename K, typename V>
typename BstMap<K, V>::Node * BstMap<K, V>::copy(const Node * curr) {
  if (curr == NULL)
    return NULL;
  Node * node = new Node(curr->key, curr->value);
  node->left = copy(curr->left);
  node->right = copy(curr->right);
  return node;
}

// assignment
template<typename K, typename V>
BstMap<K, V> & BstMap<K, V>::operator=(const BstMap<K, V> & rhs) {
  if (this != &rhs) {
    BstMap<K, V> temp(rhs);
    std::swap(this->root, temp.root);
  }
  return *this;
}

// destructor
template<typename K, typename V>
BstMap<K, V>::~BstMap() {
  destroy(this->root);
}

template<typename K, typename V>
void BstMap<K, V>::destroy(Node * curr) {
  if (curr == NULL)
    return;
  destroy(curr->left);
  destroy(curr->right);
  delete curr;
}

// add
template<typename K, typename V>
void BstMap<K, V>::add(const K & key, const V & value) {
  Node ** toPut = getNode(key);
  // new node
  if (*toPut == NULL) {
    *toPut = new Node(key, value);
  }
  else {
    (*toPut)->value = value;  // update value
  }
}

// look up
template<typename K, typename V>
const V & BstMap<K, V>::lookup(const K & key) const throw(std::invalid_argument) {
  Node ** toRet = getNode(key);
  if (*toRet == NULL) {
    throw std::invalid_argument("Key Error.");
  }
  return (*toRet)->value;
}

// remove
template<typename K, typename V>
void BstMap<K, V>::remove(const K & key) {
  Node ** prev = getNode(key);
  Node * toDel = *prev;

  if (toDel == NULL) {
    return;
  }
  if (toDel->left == NULL) {
    *prev = toDel->right;
  }
  else {
    Node ** rightMaxOnLeft = &((*prev)->left);
    while ((*rightMaxOnLeft)->right != NULL) {
      rightMaxOnLeft = &((*rightMaxOnLeft)->right);
    }
    *prev = *rightMaxOnLeft;
    *rightMaxOnLeft = (*rightMaxOnLeft)->left;
    (*prev)->left = toDel->left;
    (*prev)->right = toDel->right;
  }
  delete toDel;
}
