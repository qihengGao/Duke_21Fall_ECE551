#include "IntArray.h"

#include <assert.h>

#include <ostream>

IntArray::IntArray() : data(NULL), numElements(0) {
}
IntArray::IntArray(int n) : data(new int[n]()), numElements(n) {
}

IntArray::IntArray(const IntArray & rhs) :
    data(new int[rhs.numElements]()),
    numElements(rhs.numElements) {
  for (int i = 0; i < rhs.numElements; i++) {
    this->data[i] = rhs.data[i];
  }
}
IntArray::~IntArray() {
  delete[] this->data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
  int * tempData = new int[rhs.numElements]();
  if (this != &rhs) {
    for (int i = 0; i < rhs.numElements; i++) {
      tempData[i] = rhs.data[i];
    }
    delete[] this->data;
    this->data = tempData;
    this->numElements = rhs.numElements;
  }
  return *this;
}
const int & IntArray::operator[](int index) const {
  assert(0 <= index && index < this->numElements);
  return this->data[index];
}
int & IntArray::operator[](int index) {
  assert(0 <= index && index < this->numElements);
  return this->data[index];
}

int IntArray::size() const {
  return this->numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
  for (int i = 0; i < this->numElements; i++) {
    if (this->data[i] != rhs.data[i]) {
      return false;
    }
  }
  return (this->numElements == rhs.numElements);
}

bool IntArray::operator!=(const IntArray & rhs) const {
  return !(*this == rhs);
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
  s << "{";
  for (int i = 0; i < rhs.size() - 1; i++) {
    s << rhs[i];
    s << ", ";
  }
  s << rhs[rhs.size() - 1];
  s << "}";
  return s;
}
