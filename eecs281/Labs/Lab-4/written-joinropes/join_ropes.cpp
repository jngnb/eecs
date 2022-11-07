// Project Identifier: 15C1680EE94C640EC35E1694295A3625C3254CBA

/*
 * join_ropes.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 4 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convience.
// No need to use them. 
// Remove any of them if you want.
#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "join_ropes.h"

using namespace std;

int join_ropes(const vector<int>& rope_lengths) {
    if (rope_lengths.empty()){
        return 0;
    }
    
    if (rope_lengths.size() == 1){
        return rope_lengths[0];
    }
    
    if (rope_lengths.size() == 2){
        return rope_lengths[0] + rope_lengths[1];
    }
    
    vector<int> temp = rope_lengths;
    sort(temp.begin(), temp.end(), greater<int>());
    
    int min = temp.back();
    temp.pop_back();
    min += temp.back();
    temp.pop_back();
    
    temp.push_back(min);
    
    return min + join_ropes(temp);

}

