#include "IntMatrix.h"

IntMatrix::IntMatrix() : numRows(0), numColumns(0), rows(NULL) {
}
IntMatrix::IntMatrix(int r, int c) : numRows(r), numColumns(c), rows(NULL) {
  rows = new IntArray *[r];
  for (int i = 0; i < r; i++) {
    this->rows[i] = new IntArray(c);
  }
}

IntMatrix::IntMatrix(const IntMatrix & rhs) :
    numRows(rhs.numRows),
    numColumns(rhs.numColumns),
    rows(NULL) {
  this->rows = new IntArray *[this->numRows];
  for (int i = 0; i < this->numRows; i++) {
    this->rows[i] = new IntArray(*rhs.rows[i]);
  }
}
IntMatrix::~IntMatrix() {
  for (int i = 0; i < this->numRows; i++) {
    delete this->rows[i];
  }
  delete[] this->rows;
}
IntMatrix & IntMatrix::operator=(const IntMatrix & rhs) {
  if (this != &rhs) {
    for (int i = 0; i < this->numRows; i++) {
      delete this->rows[i];
    }
    delete[] this->rows;

    this->numRows = rhs.numRows;
    this->numColumns = rhs.numColumns;
    this->rows = new IntArray *[this->numRows];
    for (int i = 0; i < this->numRows; i++) {
      this->rows[i] = new IntArray(*rhs.rows[i]);
    }
  }
  return *this;
}
int IntMatrix::getRows() const {
  return this->numRows;
}
int IntMatrix::getColumns() const {
  return this->numColumns;
}
const IntArray & IntMatrix::operator[](int index) const {
  assert(index >= 0 && index < this->numRows);
  return *this->rows[index];
}
IntArray & IntMatrix::operator[](int index) {
  assert(index >= 0 && index < this->numRows);
  return *this->rows[index];
}

bool IntMatrix::operator==(const IntMatrix & rhs) const {
  if (this->numRows != rhs.numRows || this->numColumns != rhs.numColumns)
    return false;
  for (int r = 0; r < this->numRows; r++) {
    if (*this->rows[r] != *rhs.rows[r]) {
      return false;
    }
  }
  return true;
}

IntMatrix IntMatrix::operator+(const IntMatrix & rhs) const {
  assert(this->numRows == rhs.numRows && this->numColumns == rhs.numColumns);
  IntMatrix result(this->numRows, this->numColumns);
  for (int i = 0; i < this->numRows; i++) {
    IntArray & result_i = *result.rows[i];
    IntArray & left = *this->rows[i];
    IntArray & right = *rhs.rows[i];
    for (int j = 0; j < this->numColumns; j++) {
      result_i[j] = left[j] + right[j];
    }
  }
  return result;
}

std::ostream & operator<<(std::ostream & s, const IntMatrix & rhs) {
  s << "[ ";
  for (int r = 0; r < rhs.getRows(); r++) {
    s << rhs[r];
    if (r != rhs.getRows() - 1)
      s << ",\n";
  }
  s << " ]";
  return s;
}
