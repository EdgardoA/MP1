#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/*
ssh eangel@unix.cs.tamu.edu
*/

int main(int argc, char ** argv) 
{
	/*
	M(memory size) = s
	b(block size) = b
	*/
	
	int b, M, c, t, r;
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

		Init (M, b, t); // initialize

		srand(time(NULL)); //Initialize random seed
		

		int i=0;
		for (i=0; i< 10; i ++)
		{
			r = rand() % val ; //Random signed value
			printf("Random number: %d\n",r);
			Insert (r, "test", 50);
		}


		//PrintList ();










/*
	int b = 128;
	int M = b * 4;  // so we have space for 4 items in each of the lists
	int t = 4;		// 4 tiers
	
	char buf [1024];
	memset (buf, 1, 1024);		// set each byte to 1
	
	char * msg = "a sample message";
	
	Init (M, b, t); // initialize
	// test operations
	int testnums [] = {0, 1<<29 , (1<<29) + 5 , 50, (1<<30) + 5, (1<<30) - 500};
	int i = 0;
	// some sample insertions
	for (i=0; i< 10; i ++)
	{
		Insert (testnums [i], buf, 50);   // insert 50 bytes from the buffer as value for each of the insertions
	}
	PrintList ();
		
	// end test operations	
	Destroy ();
	*/
	
}
