//Project Identifier: 5AE7C079A8BF493DDDB6EF76D42136D183D8D7A8
/*
 * anagram.cpp
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 3 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convenience.
// No need to use them.
// Remove any of them if you want.
#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
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

#include "anagram.h"

using namespace std;

bool isAnagram(const string &s1, const string &s2) {
    int count[256] = {0};
    
    for (int i = 0; s1[i] || s2[i]; ++i){
        if (s1[i] != ' ')
            count[s1[i]]++;
        if (s2[i] != ' ')
            count[s2[i]]--;

    }
    for (size_t i = 97; i < 256; ++i){
        if (count[i] != 0)
            return false;
    }
    return true;
}

