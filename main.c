/*
 Machine Problem #1
 Daniel Frazee & Edgardo Angel
*/

/*
 Run & Compile
 gcc main.c
 ./a.out -b # -s #
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define KB *1024

int main(int argc, char *argv[]) 
{

	int b, s, c;
	char *bchar, *schar;
	extern char *optarg;

	while ((c = getopt(argc, argv, "b:s:")) != -1)
		switch (c) {
			case 'b':
				bchar = optarg;
				b = atoi(bchar);
				break;
			case 's':
				schar = optarg;
				s = atoi(schar);
				break;
		}
		
	printf("Block size: %d \n",b);
	printf("Memory Size %d \n",s);

 /*
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
	*/

	return 0;
}
