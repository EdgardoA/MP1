/*
 Machine Problem #1
 Daniel Frazee & Edgardo Angel
*/

/*
 Run & Compile
 gcc *.c
 ./a.out -b 128 -s 1408
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "linked_list2.h"

int main(int argc, char ** argv) 
{
	
	int b, M, c, t, r, maxnodes;
	int tier_alloc;
	int val = 2147483647; //2^31-1 signed values = 2,147,483,647
	char *bchar, *schar, *tchar;
	extern char *optarg;

	while ((c = getopt(argc, argv, "b:s:t:")) != -1)
		switch (c) {
			case 'b':
				bchar = optarg;
				b = atoi(bchar);
				break;
			case 's':
				schar = optarg;
				M = atoi(schar);
				break;
			case 't':
				tchar = optarg;
				t = atoi(tchar);
				break;
		}

		Init (M, b, t); // initialize tiers of memory pools

		maxnodes = M/b;

		printf("\n*** \nFilling a Tiered Memory Pool \nMemory Size: %d\nBlock Size: %d\nTiers: %d\nInserting %d elements\n***\n\n",M,b,t,maxnodes);

		srand(time(NULL)); //Initialize random seed

		//Fills the tiered memory pool with random numbers.
		int i=0;
		for (i=0; i< maxnodes; i ++)
		{
			r = rand() % val ; //Random signed value
			Insert (r, "test", 50);
		}

		PrintList ();

		//All Functions Testing

		// Inserting element
		printf("\n*** \nInserting Element with Key: 1 \n***\n");
		Insert (1, "test", 50);

		PrintList ();

		// Looking for an existing element
		printf("\n*** \nLooking for an existing element (Key: 1) \n***\n");
		char* kv = Lookup (1);
		if (kv) {
			printf("\n*** \n Element Found \n*** \n");
			printf ("\n Key = %d, Value Len = %d, Value = %s\n\n", *(int *) kv, *(int *) (kv+4), kv + 8);
		} else {
			printf("\n*** \n Element NOT Found \n*** \n");
		}
			
		// Deleting existing element
		printf("\n*** \nDeleting an existing element (Key: 1) \n***\n");
		Delete (1);

		PrintList ();

		// Looking for non-existing element - this will fail and will not print anything
		printf("\n*** \nLooking for a non existing element (Key: 5) \n***\n");
		kv = Lookup (5);
		if (kv) {
			printf("\n*** \n Element Found \n*** \n");
			printf ("\nKey = %d, Value Len = %d, Value = %s\n\n", *(int *) kv, *(int *) (kv+4), kv + 8);
		} else {
			printf("\n*** \n Element NOT Found \n*** \n");
		}

		// Deleting a non-existing element
		printf("\n*** \nDeleting a non existing element (Key: 5) \n***\n");
		Delete (5);

		//Kill Tiered Memory Pool
		printf("\n*** \nFreeing Tiered Memory Pool\n***\n");
		Destroy();
	
}
