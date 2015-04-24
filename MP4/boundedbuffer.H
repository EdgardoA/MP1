#ifndef _boundedbuffer_H_
#define _boundedbuffer_H_

#include <queue>
#include "semaphore.H"
#include "Item.H"

class BoundedBuffer {
private:
  Semaphore* lock;
  Semaphore* full;
  Semaphore* empty; 
  int MAX_SIZE;
  queue<Item*> list;
  
public:
  
  BoundedBuffer() { }
  BoundedBuffer(int _size);
  
  void depositItem(Item* _item);
  Item* RetrieveItem();
  bool isEmpty();
  int size();
  
  ~BoundedBuffer() { }
  
};

#endif