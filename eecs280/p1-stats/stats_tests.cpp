/* stats_tests.cpp
 *
 * Unit tests for the simple statistics library
 * Project UID 5366c7e2b77742d5b2142097e51561a5
 *
 * EECS 280 Project 1
 *
 * Protip #1: Write tests for the functions BEFORE you implement them!  For
 * example, write tests for median() first, and then write median().  It sounds
 * like a pain, but it helps make sure that you are never under the illusion
 * that your code works when it's actually full of bugs.
 *
 * Protip #2: Instead of putting all your tests in main(),  put each test case
 * in a function!
 */


#include "stats.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

using namespace std;

void test_sum_small_data_set();
void test_summarize();
void test_count();
void test_sum();
void test_mean();
void test_median_even();
void test_median_odd();
void test_mode();
void test_min();
void test_max();
void test_stdev();
void test_percentile();

// Add prototypes for you test functions here.

int main() {

  test_sum_small_data_set();
  test_summarize();
  test_count();
  test_sum();
  test_mean();
  test_median_even();
  test_median_odd();
  test_mode();
  test_min();
  test_max();
  test_stdev();
  test_percentile();
  // Call your test functions here

  return 0;
}

void test_sum_small_data_set() {
  cout << "test_sum_small_data_set" << endl;

  vector<double> data;
  data.push_back(1);
  data.push_back(2);
  data.push_back(3);

  assert(sum(data) == 6);

  cout << "PASS!" << endl;
}

void test_summarize(){
  cout << "test_summarize" << endl;

  vector<double> data {1, 2, 1, 5, -1.1, 7, 91, 2, -1.1};
  vector<vector<double> > answer = {{-1.1, 2}, {1, 2}, {2, 2}, {5, 1}, {7, 1}, {91, 1}};

  assert(summarize(data) == answer);

  cout << "PASS!" <<endl;
}

void test_sum(){
  cout << "test_summarize" << endl;
  vector<double> data{1, 2, 1, 5, -1.1, 7, 91, 2, -1.1};
  double answer = 1+2+1+5-1.1+7+91+2-1.1;

  assert(sum(data) == answer);

  cout << "PASS!" <<endl;
}

void test_count(){
  cout << "test_count" << endl;

  vector<double> data{1, 2, 1, 5, -1.1, 7, 91, 2, -1.1};

  assert(count(data) == 9);

  cout << "PASS!" <<endl;
}

void test_mean(){
  cout << "test_count" << endl;
  vector<double> data{1, 2, 1, 5, -1.1, 7, 91, 2, -1.1};
  double answer = (1+2+1+5-1.1+7+91+2-1.1)/9;

  assert(mean(data) == answer);

  cout << "PASS!" << endl;
}

void test_median_odd(){
  cout << "test_median" << endl;

  vector<double> data{1, 3, 1, 5, -1.1, 7, 91, 2, -10};
  //-10, -1.1  1  1  2  3  5  7  91
  double answer = 2;

  assert(median(data) == answer);

  cout << "PASS!" << endl;
}

void test_median_even(){
  cout << "test_median" << endl;

  vector<double> data{1, 5, 1, 5, -1.1, 7, 91, 2};
  //-1.1  1  1  2  5  5  7  91
  double answer = 3.5;

  assert(median(data) == answer);

  cout << "PASS!" << endl;
}


void test_mode(){
  cout << "test_mode" << endl;

  vector<double> data{1, 2, 1, 5, -1.1, 7, 91, 2, -1.1, -10, -10};

  assert(mode(data) == -10);

  cout << "PASS!" << endl;
}

void test_min(){
  cout << "test_min" << endl;

  vector<double> data{1, 2, 1, 5, -1.1, 7, 91, 2, -1.1};

  assert(min(data) == -1.1);

  cout << "PASS!" << endl;
}

void test_max(){
  cout << "test_max" << endl;

  vector<double> data{1, 2, 1, 5, -1.1, 7, 91, 2, -1.1};

  assert(max(data) == 91);

  cout << "PASS!" << endl;
}

const double epsilon = 0.00001;

void test_stdev(){
  cout << "test_stdev" << endl;

  vector<double> data{1, 2, 1, 5, -1.1, 7, 91, 2, -1.1};
  double answer = 29.79106409647;

  assert(fabs(stdev(data) - answer) < epsilon);

  cout << "PASS!" << endl;
}

void test_percentile(){
  cout << "test_percentile" << endl;

  vector<double> data{1, 2, 1, 5, -1.1, 7, 91, 2, -1.1};
  double answer = 0.412;

  assert(fabs(percentile(data, 0.215) - answer) < 0.01);

  cout << "PASS!" << endl;
}







