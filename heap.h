#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>

template <
         typename T,
         typename KComparator = std::equal_to<T>,
         typename PComparator = std::less<T>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const KComparator& kcomp = KComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item
    void push(const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey - Finds key matching old object &
    /// updates its location in the heap based on the new value
    void decreaseKey(const T& old, const T& newVal);

  int size();
  T const & get(int index) const;

 private:
    /// Add whatever helper functions and data members you need below
  void swap(int first, int second);
  void trickleUp(int loc);
  void heapify(int loc);

 private:
    /// Data members - These should be sufficient
    std::vector< T > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, KComparator> keyToLocation_;


};

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const KComparator& kcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, kcomp)
{

}

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::~Heap()
{

}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::push( const T& item)
{
    store_.push_back(item);
    // keyToLocation_.insert(std::make_pair<T,size_t>(item, store_.size()-1));

    trickleUp(store_.size()-1);
}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::decreaseKey(const T& old, const T& newVal)
{
  
  if(keyToLocation_.find(old)!=keyToLocation_.end())
  {
    //if found
    store_[keyToLocation_.find(old)->second] = newVal;
    trickleUp(keyToLocation_.find(old)->second);
  }else
  {
    return;
  }

}

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
T const & Heap<T,KComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
      throw std::logic_error("can't top an empty heap");
    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element
    return store_[0];
}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }

  store_[0] = store_[store_.size()-1];
  store_.pop_back();
  heapify(0);
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::trickleUp(int loc)
{
  int parent = (loc-1)/m_;
  //comparator
  while(parent>=0 && c_(store_[loc],store_[parent]) )
  {
    this->swap(parent, loc);
    loc = parent;
    parent = (loc-1)/m_;
  }

  // keyToLocation_.insert({store_[loc], static_cast<unsigned int>(store_.size()-1)});
  keyToLocation_.insert(std::make_pair(store_[loc], static_cast<unsigned int>(store_.size()-1)));

  //make_pair doesnt work
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::heapify(int loc)
{
  //if it's leaf node
  if(loc*m_>store_.size()-2)
  {
    return;
  }
  
  int leftChild = m_*loc+1;
  int smallerChild = leftChild;

  for(int i=1;i<m_;i++)
  {
    //if the next right node exists
    if(leftChild+i<=store_.size()-1)
    {
      // std::cout<<"right exists"<<std::endl;
      int rChild = leftChild+i;
      //comparator
      if(c_(store_[rChild], store_[smallerChild]) )
      {
        // std::cout<<"swap child"<<std::endl;
        smallerChild = rChild;
      }
    }else{
      break;
    }
  }

  if(c_(store_[smallerChild], store_[loc]))
  {
    swap(loc, smallerChild);
    // std::cout<<"smallerChild is: "<<smallerChild<<std::endl;
    heapify(smallerChild);
  }
}

/// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
bool Heap<T,KComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::swap(int first, int second)
{
  T temp = store_[first];
  store_[first] = store_[second];
  store_[second] = temp;

  keyToLocation_[temp] = second;
  keyToLocation_[store_[first]] = first;
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
int Heap<T,KComparator,PComparator,Hasher>::size()
{
  return store_.size();
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
T const & Heap<T,KComparator,PComparator,Hasher>::get(int index) const
{
  return store_[index];
}

#endif
