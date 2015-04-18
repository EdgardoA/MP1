#ifndef _item_H_
#define _item_H_

using namespace std;

class Item {
public:
	int id;
	string data;
	
	Item(): id(-1), data("") { }
	Item(int _id, string _data_string, int _req_num): id(_id), data(_data_string) { }
	
	virtual void Print();
	
	~Item() { }
};

#endif