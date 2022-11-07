/* 
 * knapsack.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 10 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convience.
// No need to use them. 
// Remove any of them if you want.
#include <algorithm>
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
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "knapsack.h"

using namespace std;

int knapsack(int C, const vector<int>& value, const vector<int>& weight) {

    const size_t n = value.size();
    vector<vector<int>> memo(n + 1, vector<int>(C + 1, 0));
    
    for (size_t i = 0; i < n; ++i){
        for(size_t j = 0; j < C + 1; ++j){
            
            if (j < weight[i]){
                memo[i + 1][j] = memo[i][j];
            }
            else{
                memo[i + 1][j] = max(memo[i][j],
                                     memo[i][j - weight[i]] + value[i]);
            }
        }
    }
    return memo[n][C];
    
}
