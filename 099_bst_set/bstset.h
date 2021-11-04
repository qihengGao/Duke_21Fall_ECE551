#include "set.h"

template<typename T>
class BstSet : public Set<T> {
  class Node {
   public:
    T key;
    Node * left;
    Node * right;
    Node(const T & key) : key(key), left(NULL), right(NULL) {}
  };
  Node * root;

  Node ** getNode(const T & key) const;
  void destroy(Node * curr);
  Node * copy(const Node * curr);

 public:
  BstSet() : root(NULL) {}
  BstSet(const BstSet<T> & rhs);
  BstSet<T> & operator=(const BstSet<T> & rhs);
  virtual ~BstSet<T>();
  virtual void add(const T & key);
  virtual bool contains(const T & key) const;
  virtual void remove(const T & key);
};

// getNode helper
template<typename T>
typename BstSet<T>::Node ** BstSet<T>::getNode(const T & key) const {
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
template<typename T>
BstSet<T>::BstSet(const BstSet<T> & rhs) {
  this->root = copy(rhs.root);
}

template<typename T>
typename BstSet<T>::Node * BstSet<T>::copy(const Node * curr) {
  if (curr == NULL)
    return NULL;
  Node * node = new Node(curr->key);
  node->left = copy(curr->left);
  node->right = copy(curr->right);
  return node;
}

// assignment
template<typename T>
BstSet<T> & BstSet<T>::operator=(const BstSet<T> & rhs) {
  if (this != &rhs) {
    BstSet<T> temp(rhs);
    std::swap(this->root, temp.root);
  }
  return *this;
}

// destructor
template<typename T>
BstSet<T>::~BstSet<T>() {
  destroy(this->root);
}

template<typename T>
void BstSet<T>::destroy(Node * curr) {
  if (curr == NULL)
    return;
  destroy(curr->left);
  destroy(curr->right);
  delete curr;
}

// add
template<typename T>
void BstSet<T>::add(const T & key) {
  Node ** toPut = getNode(key);
  // new node
  if (*toPut == NULL) {
    *toPut = new Node(key);
  }
}

// contains
template<typename T>
bool BstSet<T>::contains(const T & key) const {
  Node ** toRet = getNode(key);
  return (toRet != NULL);
}

// remove
template<typename T>
void BstSet<T>::remove(const T & key) {
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
