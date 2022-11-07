/*
 * linked_queue.cpp
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 2 Written.
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

#include "linked_queue.h"

using namespace std;

template <typename T>
T LinkedQueue<T>::front() const {
    return head->value;
}

template <typename T>
void LinkedQueue<T>::pop() {
    if (!head){
        return;
    }
    head = head->next;
    count--;
}

template <typename T>
void LinkedQueue<T>::push(T x) {
    Node<T> *n = new Node<T>;
    n->value = x;
    n->next = nullptr;
    if (!head){
        head = n;
    }
    else{
        tail->next = n;
    }
    tail = n;
    count++;
}

template <typename T>
LinkedQueue<T>::~LinkedQueue() {
    
    Node<T> *victim = head;
    while(victim){
        head = victim->next;
        delete victim;
        victim = head;
    }
    
}
