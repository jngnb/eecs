/* 
 * palindrome.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 1 Written.
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

#include "palindrome.h"

using namespace std;

bool isPalindrome(Node* start, Node* end) {
  //assigning temporary nodes to start and end nodes
  Node* n1 = start;
  Node* n2 = end;

  //increments n1 and decrements n2 until they meet at middle
  while (n1 != n2){ //runs while nodes don't overlap (odd # of nodes)

    if (n1->value != n2->value)
      return false;

    n1 = n1->next;
    if (n1 == n2) //prevents nodes passing by w/o overlap (even # of nodes)
      return true;

    n2 = n2->prev;

  }//while

  return true;
}
