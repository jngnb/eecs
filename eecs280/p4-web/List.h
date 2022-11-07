#ifndef LIST_H
#define LIST_H
/* List.h
 *
 * doubly-linked, double-ended list with Iterator interface
 * Project UID c1f28c309e55405daf00c565d57ff9ad
 * EECS 280 Project 4
 */

#include <iostream>
#include <cassert> //assert
#include <cstddef> //NULL


template <typename T>
class List {
  //OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:
  List(): first(nullptr), last(nullptr){}

  ~List(){ clear(); }

  List(const List<T> & other):first(nullptr), last(nullptr){
    copy_all(other);
  }

  List<T> & operator=(const List<T> &rhs){
    if(this == &rhs){
      return *this;
    }
    clear();
    copy_all(rhs);
    return *this;
  }

  //EFFECTS:  returns true if the list is empty
  bool empty() const;

  //EFFECTS: returns the number of elements in this List
  //HINT:    Traversing a list is really slow.  Instead, keep track of the size
  //         with a private member variable.  That's how std::list does it.
  int size() const;

  //REQUIRES: list is not empty
  //EFFECTS: Returns the first element in the list by reference
  T & front();

  //REQUIRES: list is not empty
  //EFFECTS: Returns the last element in the list by reference
  T & back();

  //EFFECTS:  inserts datum into the front of the list
  void push_front(const T &datum);

  //EFFECTS:  inserts datum into the back of the list
  void push_back(const T &datum);

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the front of the list
  void pop_front();

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the back of the list
  void pop_back();

  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes all items from the list
  void clear();

  void push_back();

  // You should add in a default constructor, destructor, copy constructor,
  // and overloaded assignment operator, if appropriate. If these operations
  // will work correctly without defining these, you can omit them. A user
  // of the class must be able to create, copy, assign, and destroy Lists

private:
  //a private type
  int list_size = 0;

  struct Node {
    T datum;
    Node *next;
    Node *prev;
  };

  //REQUIRES: list is empty
  //EFFECTS:  copies all nodes from other to this
  void copy_all(const List<T> &other);

  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty

public:
  ////////////////////////////////////////
  class Iterator {
    //OVERVIEW: Iterator interface to List
    
    // You should add in a default constructor, destructor, copy constructor,
    // and overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you can omit them. A user
    // of the class must be able to create, copy, assign, and destroy Iterators.
    Iterator():node_ptr(nullptr){}
    //~Iterator(){delete node_ptr};

    // Your iterator should implement the following public operators: *,
    // ++ (prefix), default constructor, == and !=.

  public:
    // This operator will be used to test your code. Do not modify it.
    // Requires that the current element is dereferenceable.
    Iterator& operator--() {
      assert(node_ptr);
      node_ptr = node_ptr->prev;
      return *this;
    }
    T & operator*() const;
    Iterator & operator++();
    bool operator==(Iterator rhs) const;
    bool operator!=(Iterator rhs) const;

  private:
    Node *node_ptr; //current Iterator position is a List node
    // add any additional necessary member variables here

    // add any friend declarations here
    friend class List;
    // construct an Iterator at a specific position
    Iterator(Node *p);

  };//List::Iterator
  ////////////////////////////////////////

  // return an Iterator pointing to the first element
  Iterator begin() const {
    return Iterator(first);
  }

  // return an Iterator pointing to "past the end"
  Iterator end() const{
    return Iterator();
  };

  //REQUIRES: i is a valid, dereferenceable iterator associated with this list
  //MODIFIES: may invalidate other list iterators
  //EFFECTS: Removes a single element from the list container
  void erase(Iterator i);

  //REQUIRES: i is a valid iterator associated with this list
  //EFFECTS: inserts datum before the element at the specified position.
  void insert(Iterator i, const T &datum);

};//List


////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.

template<typename T>
bool List<T>::empty() const {
  return (list_size == 0);//why can't we access size?
}

template<typename T>
int List<T>::size() const {
  return list_size;
}

template<typename T>
T & List<T>::front(){
  assert(!empty());
  return (first->datum);
  
}

template<typename T>
T & List<T>::back(){
  assert(!empty());
  return (last->datum);
}

template<typename T>
void List<T>::push_front(const T &datum){
  Node *p = new Node;
  p->datum = datum; 
  p->prev = nullptr;

  if (empty()){
    first = last = p;
    p->next = nullptr;
  }
  else{
    p->next = first;
    first = p;
  }
  
  ++list_size;
}

template<typename T>
void List<T>::push_back(const T &datum){
  Node *p = new Node;
  p->datum = datum;
  p->next = nullptr;

  if (empty()){
    first = last = p;
    p->prev = nullptr;
  }
  else{
    p->prev = last;
    last->next = p;
    last = p;
  }

  ++list_size;
}

template<typename T>
void List<T>::pop_front(){
  assert(!empty());
  if (first == last){
    last = nullptr;
  }
  Node *victim = first;
    
  first = first->next;
  if(first != nullptr){
    first->prev = nullptr;
  }
  delete victim;

  --list_size;
}

template<typename T>
void List<T>::pop_back(){
  assert(!empty());
  if (first == last){
    first = nullptr;
  }
  Node *victim = last;
    
  last = victim->prev;
  if(last != nullptr){
    last->next = nullptr;//WHY WONT this work
  }
  delete victim;
    
  --list_size;
}

template<typename T>
void List<T>::clear(){
  while (!empty()){
    pop_front();
  }
}

template<typename T>
void List<T>::copy_all(const List<T> &other){
  for(Node * np = other.first; np; np = np->next){
    push_back(np->datum);
  }
}

template<typename T>
List<T>::Iterator::Iterator(Node *p){
  node_ptr = p;
}

template<typename T>
T & List<T>::Iterator::operator*() const{
  assert(node_ptr);
  return node_ptr->datum;
}

template<typename T>
typename List<T>::Iterator & List<T>::Iterator::operator++(){
  assert(node_ptr);
  node_ptr = node_ptr->next;
  return *this;
}

template<typename T>
bool List<T>::Iterator::operator==(Iterator rhs) const{
  return (node_ptr == rhs.node_ptr);
}

template<typename T>
bool List<T>::Iterator::operator!=(Iterator rhs) const{
  return (node_ptr != rhs.node_ptr);
}

template<typename T>
void List<T>::erase(Iterator i){
  Iterator f = Iterator(first);
  Iterator end = Iterator(last);
  assert(i.node_ptr);

  if (i == f){
    pop_front();
  }
  else if (i == end){
    pop_back();
  }
  else{
    Node *victim = i.node_ptr;
    victim->prev->next = victim->next;
    victim->next->prev = victim->prev;
    delete victim;
    --list_size;
  }

}

template<typename T>
void List<T>::insert(Iterator i, const T &datum){
  Node *p = i.node_ptr;

  if(p == nullptr){
    push_back(datum);
  }
  else{
    Node *new_node = new Node; //PROBLEM
    new_node->next = nullptr;
    new_node->prev = nullptr;
    
    new_node->datum = datum;
    if(p->prev != nullptr){
      p->prev->next = new_node;
      new_node->prev = p->prev;
    }
    else if(p->prev == nullptr){
        first = new_node;
    }
    p->prev = new_node;
    new_node->next = p;

    ++list_size;
  }

}

#endif // Do not remove this. Write all your code above this line.
