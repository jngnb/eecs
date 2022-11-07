/* 
 * two_pair_sums.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 6 Written.
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

#include "two_pair_sums.h"

using namespace std;
using intpair = pair<int, int>;

void two_pair_sums(const vector<int>& nums, ostream& os) {
    
    intpair empty = {0, 0};
    
    unordered_map<int, pair<intpair, intpair>> hash;
    
    for (size_t i = 0; i < nums.size(); ++i){
        for (size_t j = i+1; j < nums.size(); ++j){
            int x = nums[i];
            int y = nums[j];
            
            if (hash[x+y].first == empty){
                hash[x+y].first = {x, y};
            }
            else if (hash[x+y].second == empty){
                hash[x+y].second = {x, y};
            }
        }
    }
    
    for (auto const& [key, val] : hash){
        intpair f = val.first;
        intpair s = val.second;
        if (f != empty && s != empty){
            os << "(" << f.first << ", " << f.second
                 << ") and (" << s.first << ", " <<s.second << ")\n";
        }
    }
    
}
