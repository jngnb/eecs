/* 
 * cycle.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 9 Written.
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

#include "cycle.h"

using namespace std;

bool is_graph_cyclic(const vector<vector<int>> &adj_list) {
    
    //depth first
    vector<bool> disc;
    
    for (int v = 0; v < adj_list.size(); ++v){
        disc[v] = true;
        
        for (int u : adj_list[v]){

            if (!disc[u]){
                disc[u] = true;
            }
            else if (disc[u] && u != v){
                return true;
            }
        }
    }
    return false;
}
