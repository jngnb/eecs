// main.cpp
// Project UID 5366c7e2b77742d5b2142097e51561a5
#include "stats.h"
#include "p1_library.h"
#include <iostream>
using namespace std;

int main() {
  string file;
  string column;

  cout << "enter a filename" << endl;
  cin >> file;
  cout << "enter a column name" << endl;
  cin >> column;

  cout << "reading column " << column << " from " << file <<endl;

  vector<double> input_v = extract_column(file, column);

  //summary of data
  vector<vector<double> > output_v = summarize(input_v);
  cout << "Summary (value: frequency)" << endl;
  size_t i = 0;
  while (i < output_v.size()){
    cout << output_v[i][0] << ": " << output_v[i][1] << endl;
    i++;
  }

  //rest of statistics
  cout << endl << "count = " << count(input_v) << endl;
  cout << "sum = " << sum(input_v) << endl;
  cout << "mean = " << mean(input_v) << endl;
  cout << "stdev = " << stdev(input_v) << endl;
  cout << "median = " << median(input_v) << endl;
  cout << "mode = " << mode(input_v) << endl;
  cout << "min = " << min(input_v) << endl;
  cout << "max = " << max(input_v) << endl;

  //percentiles
  cout << "  0th percentile = " << percentile(input_v, 0) << endl;
  cout << " 25th percentile = " << percentile(input_v, 0.25) << endl;
  cout << " 50th percentile = " << percentile(input_v, 0.50) << endl;
  cout << " 75th percentile = " << percentile(input_v, 0.75) << endl;
  cout << "100th percentile = " << percentile(input_v, 1) << endl;

}