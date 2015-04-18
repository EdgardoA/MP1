#ifndef _boundedbuffer_H_
#define _boundedbuffer_H_

#include <queue>
#include "semaphore.H"

using namespace std;

class Boundedbuffer {
private:
    Semaphore* Lock;
	Semaphore* Full;
	Semaphore* Empty;	
	int MAX_SIZE;
	queue<Item*> que;

public:
    Boundedbuffer(int _size);
    Boundedbuffer() {}
    
    void depositItem(Item* _item);
	Item* RetrieveItem();
	bool empty();
	int size();
    
    
    ~Boundedbuffer() {}
    
    void print();

};

#endif