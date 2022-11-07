// stats.cpp
// Project UID 5366c7e2b77742d5b2142097e51561a5

#include "stats.h"
#include "p1_library.h"
#include <cassert>
#include <vector>
#include <cmath>
using namespace std;

vector<vector<double> > summarize(vector<double> v) {
  vector<vector<double> > output_v;
  sort(v);
  double curr_freq = 1;
  size_t i = 0;
  while (i < v.size()-1){
    if (i == (v.size()-2)){
      if (v[i] == v[i+1]){
        curr_freq++;
        output_v.push_back({v[i], curr_freq});
      }
      else{
        output_v.push_back({v[i], curr_freq});
        output_v.push_back({v[i+1], curr_freq});
      }
      i++;
    }
    else if (v[i] == v[i+1]){
      curr_freq++;
      i++;
    }
    else{
      output_v.push_back({v[i], curr_freq});
      curr_freq = 1;
      i++;
    }
  }
  return output_v;
}

int count(vector<double> v) {
  return v.size();
}

double sum(vector<double> v) {
  size_t i  = 0;
  double sum = 0.0;
  while (i < v.size()){
    sum += v[i];
    ++i;
  }
  return sum;
}

double mean(vector<double> v) {
  return (sum(v)/count(v));
}

double median(vector<double> v) {
  /*
  sorting process--turns out p1_library.h has a prewritten function
  while (i = v.size()-1){
    double min = v[i];
    for (int j = i; j < v.size(); ++j){
      if(v[j]<min){
        min = v[j];
        idx = j;
      }
    }
    v[i] = v[idx];
    v[idx] = min;
    i++;
  }*/

  sort(v);
  int median_idx = round(v.size()/2.0);
  double median = v[median_idx-1];
  if (v.size() % 2 == 0){
    median = (v[median_idx] + v[median_idx-1])/2.0;
  }
  return median;
}

double mode(vector<double> v){
  double i = 1;
  vector<vector<double> > vec = summarize(v);
  double max_freq = vec[0][1];//2
  double mode = vec[0][0]; //2
  while (i < vec.size()){ //are double []s allowed lol
    if (vec[i][1] > max_freq){
      max_freq = vec[i][1];//
      mode = vec[i][0];//
      i++;
    } 
    else{
      i++;
    }
  }
  return mode;
}

double min(vector<double> v) {
  double min = v[0];
  for (size_t i = 0; i < v.size(); ++i){
    if(v[i] < min){
      min = v[i];
    }
    else{}
  }
  return min;
}

double max(vector<double> v) {
  double max = v[0];
  for (size_t i = 0; i < v.size(); ++i){
    if (v[i]>max){
      max = v[i];
    }
    else{}
  }
  return max;
}

double stdev(vector<double> v) {
  assert(v.size() >= 2);
  size_t i = 0;
  double sum = 0;
  double avg = mean(v);
  while (i < v.size()){
    double val = pow((v[i] - avg),2);
    sum+=val;
    i++;
  }
  return (sqrt(sum/(count(v)-1)));

  //USE epsilon????
}

double percentile(vector<double> v, double p) {
  assert(p>=0 && p<=1);
  sort(v);
  if (p == 0){
    return v[0];
  }
  else if(p == 1){
    return v[v.size()-1];
  }
  else{
    double n = (p*(count(v) - 1)) + 1;
    double k, d;
    d = modf(n, &k);
    double Vp = v[k-1] + (d*(v[k]-v[k-1]));
    return Vp;
  }
}

