/*
 * replace_words.cpp
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 7 Written.
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

#include "replace_words.h"

using namespace std;

vector<string> replace_words(const vector<string>& prefixes,
                             const vector<string>& sentence) {

    vector<string> output(sentence.size());
    
    vector<string> tempprefixes = prefixes;
    sort(tempprefixes.begin(), tempprefixes.end());

    for (size_t i = 0; i < sentence.size(); ++i){
        bool found = false;
        for (const string &prefix : tempprefixes){
            if (sentence[i].find(prefix) != string::npos){
                output[i] = prefix;
                found = true;
                break;
            }
        }
        if (!found){
            output[i] = sentence[i];
        }
    }
    return output;
        
}
