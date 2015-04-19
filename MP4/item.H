#ifndef _item_H_
#define _item_H_

#include <iostream>


using namespace std;

class Item {
public:
	int Id;
	string info;
	Item(): Id(-1), info("") { }
	Item(int _id, string _info, int _request_num): Id(_id), info(_info) { }
	
    
	virtual void Print() {
		cout << "ID: " << Id << endl;
        cout << "Info: " << info << endl;
    }
	
	~Item() { }
};

#endif