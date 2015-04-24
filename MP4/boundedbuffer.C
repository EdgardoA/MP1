#include "BoundedBuffer.H"

BoundedBuffer::BoundedBuffer(int _size) {
	lock = new Semaphore(1);
	full = new Semaphore(0);
	empty = new Semaphore(_size);	
	MAX_SIZE = _size;
}

void BoundedBuffer::depositItem(Item* _item) {
	empty->P();
	lock->P();
	list.push(_item);
	lock->V();
	full->V();
}

Item* BoundedBuffer::RetrieveItem() {
	Item* ret_item;
	full->P();
	lock->P();
	ret_item = list.front();
	list.pop();
	lock->V();
	empty->V();
	
	return ret_item;
}

bool BoundedBuffer::isEmpty() {
	if (list.size() == 0) return true;
	return false;
}

int BoundedBuffer::size() {
	return list.size();
}


