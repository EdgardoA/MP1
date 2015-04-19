#include "boundedbuffer.H"
#include "item.H"

Boundedbuffer::Boundedbuffer(int size) {
	Lock = new Semaphore(1);
	Full = new Semaphore(0);
	Empty = new Semaphore(size);	
	MAX_SIZE = size;
}


void Boundedbuffer::depositItem(Item* _item) {
	Empty->P();
	Lock->P();
	que.push(_item);
	Lock->V();
	Full->V();
}

Item* Boundedbuffer::RetrieveItem() {
	Item* ret_item;
	Full->P();
	Lock->P();
	ret_item = que.front();
	que.pop();
	Lock->V();
	Empty->V();
	
	return ret_item;
}

bool Boundedbuffer::empty() {
	if (que.size() == 0) return true;
	return false;
}

int Boundedbuffer::size() {
	return que.size();
}

void Boundedbuffer::print() {
    queue<Item*> que2 = que;
    cout << "\tBB: " << endl;
    while(que2.empty() == false) {
        cout << "\t" << que2.front()->id << " " << que2.front()->data << endl;
        que2.pop();
    }
}