#include <stdlib.h>
#define KB *1024
int main(int argc, char ** argv) 
{
	int M = 1024;
	int b = 128;
	Init (M,b); // initialize
	// test operations
	int testnums [] = {100, 5, 200, 7, 39, 25, 400, 50, 200, 300};
	int i = 0;
	for (i=0; i< 10; i ++)
	{
		Insert (testnums [i]);
	}
	PrintList ();
	Delete (7);
	Insert (13);
	Delete (55);
	Delete (3);
	PrintList ();
	// end test operations	
	Destroy ();
	
}
