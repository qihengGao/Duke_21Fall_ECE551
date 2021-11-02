#ifndef __LL_HPP__
#define __LL_HPP__

#include <assert.h>

#include <cstdlib>
#include <exception>
class IndexException : public std::exception {
  virtual const char * what() const throw() { return "Index is out of range."; }
};

template<typename T>
class LinkedList {
  class Node {
   public:
    T data;
    Node * next;
    Node * prev;
    Node(Node * prev, const T & data, Node * next) : data(data), next(next), prev(prev) {}
  };

  Node * head;
  Node * tail;
  int size;

 public:
  LinkedList() : head(NULL), tail(NULL), size(0) {}
  LinkedList(const LinkedList<T> & rhs);
  LinkedList<T> & operator=(const LinkedList<T> & rhs);
  ~LinkedList();
  void addFront(const T & val);
  void addBack(const T & val);
  bool remove(const T & val);
  T & operator[](int i);
  const T & operator[](int i) const;
  int find(const T & val);
  int getSize() const;
  friend class Tester;
};

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T> & rhs) {
  head = NULL;
  tail = NULL;
  this->size = 0;
  Node * curr = rhs.head;
  while (curr != NULL) {
    addBack(curr->data);
    curr = curr->next;
  }
}

template<typename T>
LinkedList<T> & LinkedList<T>::operator=(const LinkedList<T> & rhs) {
  if (this != &rhs) {
    LinkedList<T> temp(rhs);
    Node * thisHead = this->head;
    this->head = temp.head;
    temp.head = thisHead;
    Node * thisTail = this->tail;
    this->tail = temp.tail;
    temp.tail = thisTail;
    size = temp.size;
  }
  return *this;
}

template<typename T>
LinkedList<T>::~LinkedList() {
  while (head != NULL) {
    Node * next = head->next;
    delete head;
    head = next;
  }
  head = NULL;
  tail = NULL;
}

template<typename T>
void LinkedList<T>::addFront(const T & val) {
  head = new Node(NULL, val, this->head);
  if (this->tail == NULL) {
    this->tail = this->head;
  }
  else {
    head->next->prev = head;
  }
  size++;
}

template<typename T>
void LinkedList<T>::addBack(const T & val) {
  tail = new Node(tail, val, NULL);
  if (head == NULL) {
    head = tail;
  }
  else {
    this->tail->prev->next = this->tail;
  }
  size++;
}

template<typename T>
bool LinkedList<T>::remove(const T & val) {
  Node ** curr = &(this->head);
  while (*curr != NULL) {
    if ((*curr)->data != val) {
      curr = &((*curr)->next);
    }
    else {
      Node * toDel = *curr;
      *curr = (*curr)->next;
      // head or midlle
      if (toDel->next != NULL) {
        toDel->next->prev = toDel->prev;
      }
      // tail
      if (toDel->next == NULL) {
        this->tail = toDel->prev;
      }
      delete toDel;
      this->size--;
      return true;
    }
  }
  return false;
}

template<typename T>
T & LinkedList<T>::operator[](int i) {
  Node * p = this->head;
  if (i < 0) {
    throw IndexException();
  }
  while ((p != NULL) && i > 0) {
    p = p->next;
    i--;
  }
  if (p == NULL) {
    throw IndexException();
  }
  return p->data;
}

template<typename T>
const T & LinkedList<T>::operator[](int i) const {
  Node * p = this->head;
  if (i < 0) {
    throw IndexException();
  }
  while ((p != NULL) && i > 0) {
    p = p->next;
    i--;
  }
  if (p == NULL) {
    throw IndexException();
  }
  return p->data;
}

template<typename T>
int LinkedList<T>::find(const T & val) {
  if (head == NULL) {
    return -1;
  }
  int i = 0;
  Node * curr = this->head;
  while (curr != NULL) {
    if (curr->data == val) {
      return i;
    }
    curr = curr->next;
    i++;
  }
  return -1;
}

template<typename T>
int LinkedList<T>::getSize() const {
  return this->size;
}
#endif
