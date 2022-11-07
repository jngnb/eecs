// Project UID af1f95f547e44c8ea88730dfb185559d

#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include "Matrix.h"
//#include "processing.h"

using namespace std;

/* 
*********NOTE!!!***********
index < WIDTH runs through columns
index < HEIGHT runs through rows
 */

void Matrix_init(Matrix* mat, int width, int height) {

  assert(width > 0 && width <= MAX_MATRIX_WIDTH);
  assert(height > 0 && height <= MAX_MATRIX_HEIGHT);

  mat->width = width;
  mat->height = height;

}

void Matrix_print(const Matrix* mat, std::ostream& os) {

  os << mat->width << " " << mat->height << endl;
  
  for (int i = 0; i < mat->height; i++){
    for (int j = 0; j < mat->width; j++){
      os << *Matrix_at(mat, i, j) << " ";
    }
    os << endl;
  }

}

int Matrix_width(const Matrix* mat) {
  return mat->width;
}

int Matrix_height(const Matrix* mat) {
  return mat->height;
}

int Matrix_row(const Matrix* mat, const int* ptr) {

  int idx = ptr - mat->data; 
  //idx = idx/4;
  int row = idx/(mat->width);
  return row;

}

int Matrix_column(const Matrix* mat, const int* ptr) {

  int idx = ptr - mat->data; 
  int row_num = idx / (mat -> width);
  int column_num = idx - (row_num * mat -> width);
  return column_num;

}

int* Matrix_at(Matrix* mat, int row, int column) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));

  int idx = (mat->width * row) + column;

  int* ptr = &(mat->data[idx]);

  return ptr;
}

const int* Matrix_at(const Matrix* mat, int row, int column) {

  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));
  int idx = (mat -> width * row ) + column;
  const int* ptr = &(mat->data[idx]);
  return ptr;

}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  for(int c = 0; c < Matrix_width(mat); ++c){
    for(int r = 0; r < Matrix_height(mat); ++r){
      *Matrix_at(mat,r,c) = value;
    }
  }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  int height = Matrix_height(mat) - 1;
  for(int c = 0; c < Matrix_width(mat); ++c){
    *Matrix_at(mat,0,c) = value;
    *Matrix_at(mat,height,c) = value;
  }
  for(int r = 0; r < Matrix_height(mat); ++r){
    int width = Matrix_width(mat) - 1;
    *Matrix_at(mat,r,0) = value;
    *Matrix_at(mat,r,width) = value;
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  int cur_max = *(mat->data);
  
  for(int c = 0; c < Matrix_width(mat); ++c){
    for(int r = 0; r < Matrix_height(mat); ++r){
      if (cur_max < *Matrix_at(mat,r,c)){
        cur_max = *Matrix_at(mat,r,c);
      }
    }
  }
  return cur_max;
  
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, int row,
                                      int column_start, int column_end) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column_start && column_end <= Matrix_width(mat));
  assert(column_start < column_end);

  int cur_min = *Matrix_at(mat,row,column_start);
  int col = column_start;

  for(int c = column_start; c < column_end; ++c){
    if (cur_min > *Matrix_at(mat,row,c)){
        cur_min = *Matrix_at(mat,row,c);
        col = c;
      }
  }
  return Matrix_column(mat, Matrix_at(mat, row, col));
  
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row,
                            int column_start, int column_end) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column_start && column_end <= Matrix_width(mat));
  assert(column_start < column_end);
  int cur_min = *Matrix_at(mat,row,column_start);

  for(int c = column_start; c < column_end; ++c){
    if (cur_min > *Matrix_at(mat,row,c)){
        cur_min = *Matrix_at(mat,row,c);
      }
  }
  return cur_min;
}
