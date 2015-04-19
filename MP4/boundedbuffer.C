#include "boundedbuffer.H"
#include "item.H"

Boundedbuffer::Boundedbuffer(int size) {
	Lock = new Semaphore(1);
	Full = new Semaphore(0);
	Empty = new Semaphore(size);	
	MAX_SIZE = size;
}


bool Boundedbuffer::empty() {
	if (que.size() == 0) return true;
    
	return false;
}

int Boundedbuffer::size() {
	return que.size();
}

Item* Boundedbuffer::getItem() {
	Item* new_item;
	Full->P();
	Lock->P();
	new_item = que.front();
	que.pop();
	Lock->V();
	Empty->V();
	return new_item;
}

void Boundedbuffer::putItem(Item* _item) {
	Empty->P();
	Lock->P();
	que.push(_item);
    
	Lock->V();
	Full->V();
}