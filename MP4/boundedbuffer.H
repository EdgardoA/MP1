#ifndef _boundedbuffer_H_
#define _boundedbuffer_H_

#include <queue>
#include "semaphore.H"
#include "item.H"
#include <iostream>

using namespace std;

class Boundedbuffer {
private:
    Semaphore* Full;
	Semaphore* Empty;
    Semaphore* Lock;
	int MAX_SIZE;
    
	queue<Item*> que;

public:
    
    Boundedbuffer(int size);
    Boundedbuffer() {}
    
    bool empty();
	int size();
    
    void putItem(Item* _item);
	Item* getItem();
    
    ~Boundedbuffer() {}
    
    void print();

};

#endif