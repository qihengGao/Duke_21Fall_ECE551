#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>

#include <cstdlib>
#include <iostream>
#include <vector>

template<typename T>
class Matrix {
 private:
  int nRows, nCols;
  std::vector<std::vector<T> > rows;

 public:
  Matrix();
  Matrix(int r, int c);
  Matrix(const Matrix & rhs);
  //Matrix & operator=(const Matrix & rhs);
  int getRows() const;
  int getColumns() const;
  std::vector<T> & operator[](int i);
  const std::vector<T> & operator[](int i) const;
  bool operator==(const Matrix & rhs) const;
  Matrix operator+(const Matrix & rhs) const;
  template<typename S>
  friend std::ostream & operator<<(std::ostream & s, const Matrix<S> & m);
};

template<typename T>
Matrix<T>::Matrix() : nRows(0), nCols(0), rows(0) {
}

template<typename T>
Matrix<T>::Matrix(int r, int c) : nRows(r), nCols(c), rows(r, std::vector<T>(c)) {
}

template<typename T>
Matrix<T>::Matrix(const Matrix & rhs) :
    nRows(rhs.nRows),
    nCols(rhs.nCols),
    rows(rhs.rows) {
}

template<typename T>
int Matrix<T>::getRows() const {
  return this->nRows;
}

template<typename T>
int Matrix<T>::getColumns() const {
  return this->nCols;
}

template<typename T>
std::vector<T> & Matrix<T>::operator[](int i) {
  assert(i >= 0 && i < this->nRows);
  return rows[i];
}

template<typename T>
const std::vector<T> & Matrix<T>::operator[](int i) const {
  assert(i >= 0 && i < this->nRows);
  return rows[i];
}

template<typename T>
bool Matrix<T>::operator==(const Matrix<T> & rhs) const {
  return this->nRows == rhs.nRows && this->nCols == rhs.nCols && this->rows == rhs.rows;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> & rhs) const {
  assert(this->nRows == rhs.nRows && this->nCols == rhs.nCols);
  Matrix<T> ans(this->nRows, this->nCols);
  // typename std::vector<std::vector<T> >::iterator a = ans.rows.begin();
  // typename std::vector<std::vector<T> >::const_iterator t = this->rows.begin(),
  //                                                       r = rhs.rows.begin();
  // for (; a != ans.rows.end(); ++a, ++t, ++r) {
  //   typename std::vector<T>::iterator a_row = a->begin();
  //   typename std::vector<T>::const_iterator t_row = t->begin(), r_row = r->begin();
  //   for (; a_row != a->end(); ++a_row, ++t_row, ++r_row) {
  //     *a_row = *t_row + *r_row;
  //   }
  // }
  for (size_t i = 0; i < ans.rows.size(); i++) {
    for (size_t j = 0; j < ans.rows[i].size(); j++) {
      ans.rows[i][j] = this->rows[i][j] + rhs.rows[i][j];
    }
  }
  return ans;
}

template<typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & m) {
  s << "[ ";
  for (size_t i = 0; i < m.rows.size(); i++) {
    if (i == 0) {
      s << "{";
    }
    else {
      s << ",\n{";
    }
    for (size_t j = 0; j < m.rows[i].size(); j++) {
      if (j > 0) {
        s << ", ";
      }
      s << m.rows[i][j];
    }
    s << "}";
  }
  s << " ]";
  return s;
}

#endif
