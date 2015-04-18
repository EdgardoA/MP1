#include "boundedbuffer.H"

Boundedbuffer::boundedbuffer(int _size) {
	lock = new Semaphore(1);
	full = new Semaphore(0);
	empty = new Semaphore(_size);	
	MAX_SIZE = _size;
}

//Boundedbuffer::~boundedbuffer() {}

void Boundedbuffer::depositItem(Item* _item) {
	empty->P();
	lock->P();
	que.push(_item);
	lock->V();
	full->V();
}

Item* Boundedbuffer::RetrieveItem() {
	Item* ret_item;
	full->P();
	lock->P();
	ret_item = que.front();
	que.pop();
	lock->V();
	empty->V();
	
	return ret_item;
}

bool Boundedbuffer::empty() {
	if (que.size() == 0) return true;
	return false;
}

int Boundedbuffer::size() {
	return que.size();
}

Boundedbuffer::void print() {
    que<Item*> que2 = que;
    cout << "\tBB: " << endl;
    while(que2.empty() == false) {
        cout << "\t" << que2.front()->id << " " << que2.front()->data << endl;
        que2.pop();
    }
}