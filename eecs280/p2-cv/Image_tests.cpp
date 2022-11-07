// Project UID af1f95f547e44c8ea88730dfb185559d

#include "Matrix.h"
#include "Image_test_helpers.h"
#include "unit_test_framework.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;


// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Sets various pixels in a 2x2 Image and checks
// that Image_print produces the correct output.
TEST(test_print_basic) {
  Image *img = new Image; // create an Image in dynamic memory

  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(img, 2, 2);
  Image_set_pixel(img, 0, 0, red);
  Image_set_pixel(img, 0, 1, green);
  Image_set_pixel(img, 1, 0, blue);
  Image_set_pixel(img, 1, 1, white);

  // Capture our output
  ostringstream s;
  Image_print(img, s);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());

  delete img; // delete the Image
}

TEST(test_init_width_height_basic){
  Image *img = new Image;
  Image_init(img, 2, 3);

  ASSERT_TRUE(Image_width(img) == 2);
  ASSERT_TRUE(Image_height(img) == 3);

  delete img;
}

TEST(test_init_from_file_basic){
  Image *actual = new Image;
  Image *expected = new Image;

  Image_init(expected, 3, 2);
  Pixel black = {0, 0, 0};
  Pixel white = {255, 255, 255};

  for (int i = 0; i < 3; ++i){
    Image_set_pixel(expected, 0, i, black);
  }
  for (int i = 0; i < 3; ++i){
    Image_set_pixel(expected, 1, i, white);
  }

  istringstream fin("P3\n3 2\n255\n0 0 0 0 0 0 0 0 0 \n255 255 255 255 255 255 255 255 255 ");
  
  Image_init(actual, fin);

  ASSERT_TRUE(Image_equal(expected, actual));

  delete actual;
  delete expected;

}

TEST(test_get_set_pixel){

  Image *img = new Image;

  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(img, 3, 2);
  
  for (int i = 0; i < Image_height(img); ++i){
    for (int j = 0; j < Image_width(img); ++j){
      Image_set_pixel(img, i, j, white);
    }
  }
  
  for (int j = 0; j < Image_width(img); j+=2){
    Image_set_pixel(img, 0, j, red);    
  }
  
  Image_set_pixel(img, 1, 1, red);
  Image_set_pixel(img, 0, 0, blue);
  Image_set_pixel(img, 1, 2, green);

  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, 0, 0), blue));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, 0, 1), white));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, 0, 2), red));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, 1, 0), white));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, 1, 1), red));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, 1, 2), green));

  delete img;
}

TEST(test_init_width_height_edge){
  Image *img = new Image;
  Image_init(img, 1, 1);

  ASSERT_TRUE(Image_width(img) == 1);
  ASSERT_TRUE(Image_height(img) == 1);

  delete img;
}

TEST(test_image_fill_rdm){
  Image *img = new Image;
  Image *img2 = new Image;

  Image_init(img, 6, 7);
  Image_init(img2, 1, 1);


  const Pixel purple = {230,230,250};
  const Pixel orange = {255,215,0};
  const Pixel pink = {222,49,99};

  Image_fill(img,orange);
  Image_fill(img2,pink);
  int height = Image_height(img);
  int width = Image_width(img);
  int height2 = Image_height(img2);
  int width2 = Image_width(img2);


  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, r, c), orange));
    }
  }

  for (int r2 = 0; r2 < height2; ++r2) {
    for (int c2 = 0; c2 < width2; ++c2) {
      ASSERT_TRUE(Pixel_equal(Image_get_pixel(img2, r2, c2), pink));
    }
  } 


  Image_set_pixel(img,3,2,purple);
  
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, 3, 2), purple));

  delete img;
  delete img2;
}


// IMPLEMENT YOUR TEST FUNCTIONS HERE
// You are encouraged to use any functions from Image_test_helpers.h as needed.

// NOTE: The unit test framework tutorial in Lab 2 originally
// had a semicolon after TEST_MAIN(). Although including and
// excluding the semicolon are both correct according to the c++
// standard, the version of g++ we are using reports a warning
// when there is a semicolon. Therefore do NOT add a semicolon
// after TEST_MAIN()
TEST_MAIN() // Do NOT put a semicolon here
