#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include "bst.h"

template<typename Key, typename Value>
class SplayTree : public BinarySearchTree<KeyType, ValueType>
{
 public:
  SplayTree();
  ~SplayTree();
  
  // All of the basic operations that one would perform on a map. 
  // Do not change these function definitions, but you may add others
  void insert(const std::pair<Key, Value>& keyValuePair);
  void clear();
  iterator find(const Key& k); // remember a find
                               // also requires splaying
  
 private:
  // Add any private data members and member functions as needed. 
};
