// Project UID af1f95f547e44c8ea88730dfb185559d

#include "Matrix.h"
#include "Matrix_test_helpers.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory

  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(mat, 3, 5);
  Matrix_fill(mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(mat, r, c), value);
    }
  }

  delete mat; // delete the Matrix
}


// ADD YOUR TESTS HERE
TEST(test_init_width_height){
  Matrix *mat = new Matrix;

  Matrix_init(mat, 3, 5);

  //ASSERT_EQUAL(mat->width, 3);
  //ASSERT_EQUAL(mat->height, 5);

  ASSERT_EQUAL(Matrix_width(mat), 3);
  ASSERT_EQUAL(Matrix_height(mat), 5);

  //ASSERT_EQUAL(Matrix_width(mat),mat->width);
  //ASSERT_EQUAL(Matrix_height(mat), mat->height);

  delete mat;
}

TEST(test_print_basic){
  Matrix *mat = new Matrix;
  Matrix_init(mat, 2, 3);
  Matrix_fill(mat, 1);

  ostringstream expected;
  expected << "2 3\n" << "1 1 \n"<<"1 1 \n"<<"1 1 \n";
  
  ostringstream actual;

  Matrix_print(mat, actual);
  
  ASSERT_EQUAL(expected.str(), actual.str());
  
  delete mat;
}

//Following two tests also should be testing Matrix_at
TEST(test_row_column_basic){
  Matrix *mat = new Matrix;
  Matrix_init(mat, 2, 3);
  Matrix_fill(mat, 1);

  const int* ptr = Matrix_at(mat, 0, 1);

  int row = Matrix_row(mat, ptr);
  int column = Matrix_column(mat, ptr);

  ASSERT_TRUE(row = 1);
  ASSERT_TRUE(column = 2);

  delete mat;
}

TEST(test_row_column_advanced){
  Matrix *mat = new Matrix;
  Matrix_init(mat, 3, 5);
  Matrix_fill(mat, 1);

  for (int i = 0; i < Matrix_height(mat); ++i){
    for (int j = 0; j < Matrix_width(mat); ++j){
      int* ptr = Matrix_at(mat, i, j);

      int row = Matrix_row(mat, ptr);
      int column = Matrix_column(mat, ptr);

      ASSERT_TRUE(row = (i+1));
      ASSERT_TRUE(column = (j+1));
    }
  }
  delete mat;
}

TEST(matrix_min_value){

  Matrix *mat = new Matrix;

  Matrix_init(mat,4,6);
  Matrix_fill(mat,4);
  Matrix_fill_border(mat,3);

  int max = Matrix_max(mat);
  int min_column = Matrix_column_of_min_value_in_row(mat,3,0,3);
  int min = Matrix_min_value_in_row(mat,4,0,3);

  ASSERT_TRUE(max == 4);
  ASSERT_TRUE(min_column == 0); 
  ASSERT_TRUE(min == 3);

  delete mat;

}

TEST(matrix_max_value){
  Matrix *mat = new Matrix;
  Matrix *mat1 = new Matrix;

  Matrix_init(mat,1,1); // edge case of 1x1
  Matrix_fill(mat,8);

  int max = Matrix_max(mat);
  ASSERT_TRUE(max == 8);

  Matrix_init(mat1,1,2); // edge case of 1x2
  Matrix_fill(mat1,8);
  *Matrix_at(mat1,1,0) = 10;
  Matrix_max(mat1);

  max = Matrix_max(mat1);

  ASSERT_TRUE(max == 10);

  delete mat;
  delete mat1;

}

TEST(matrix_first_and_last_max){
  Matrix *mat = new Matrix;

  Matrix_init(mat,4,3);
  Matrix_fill(mat, 31); // edge case of 1x1
  *Matrix_at(mat,0,0) = 32;

  int max = Matrix_max(mat);
  ASSERT_TRUE(max == 32);

  *Matrix_at(mat,2,3) = 33;

  max = Matrix_max(mat);
  ASSERT_TRUE(max == 33);

  delete mat;
}

TEST(matrix_at_test){
  Matrix *mat = new Matrix;

  Matrix_init(mat,15,1);
  Matrix_fill_border(mat,1);

  int location = *Matrix_at(mat,0,14);

  ASSERT_TRUE(location == 1)

  delete mat;

}


TEST(matrix_print_test){
  Matrix *mat = new Matrix;

  Matrix_init(mat,3,3);
  Matrix_fill(mat,2);
  Matrix_fill_border(mat,4);
  
  ostringstream actual;
  Matrix_print(mat, actual);

  ostringstream expected;
  expected << "3 3\n" << "4 4 4 \n"<< "4 2 4 \n"<<"4 4 4 \n";

  ASSERT_EQUAL(expected.str(), actual.str());
  
  delete mat;

}

TEST(test_out_of_bounds){
  Matrix *mat = new Matrix;

  Matrix_init(mat, 1, 1);
  Matrix_fill(mat, 1);
  ASSERT_TRUE(Matrix_width(mat) == 1 && Matrix_height(mat) == 1);
  
  int * ptr = Matrix_at(mat, 0, 0);
  ASSERT_TRUE(Matrix_column(mat, ptr)==0);
  ASSERT_TRUE(Matrix_row(mat, ptr)==0);

  Matrix *mat1 = new Matrix;

  Matrix_init(mat1,8,3);
  int *ptr2 = Matrix_at(mat1,2,6);

  int row = Matrix_row(mat1,ptr2);
  ASSERT_TRUE(row == 2)

  delete mat;
  delete mat1;

}

TEST(matrix_fill_borders){
  Matrix *mat = new Matrix;
  Matrix_init(mat,2,3);
  Matrix_fill_border(mat,5);
  int height = Matrix_height(mat);
  int width = Matrix_width(mat);

  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      ASSERT_TRUE(*Matrix_at(mat,r,c) == 5);
    }
  }

  delete mat;
}

TEST(matrix_at_again){
  Matrix *mat = new Matrix;
  Matrix_init(mat,8,3);
  Matrix_fill(mat,7);
  Matrix_fill_border(mat,1);

  int * rdm = Matrix_at(mat,2,5);

  ASSERT_EQUAL(*(rdm + 1),*Matrix_at(mat,2,6));
  ASSERT_EQUAL(*(rdm - 6),*Matrix_at(mat,1,7));

  delete mat;
} 





// You are encouraged to use any functions from Matrix_test_helpers.h as needed.


// NOTE: The unit test framework tutorial in Lab 2 originally
// had a semicolon after TEST_MAIN(). Although including and
// excluding the semicolon are both correct according to the c++
// standard, the version of g++ we are using reports a warning
// when there is a semicolon. Therefore do NOT add a semicolon
// after TEST_MAIN()
TEST_MAIN() // Do NOT put a semicolon here

