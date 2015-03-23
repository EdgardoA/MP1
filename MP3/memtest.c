#include "ackerman.h"
#include "my_allocator.h"
#include <math.h>

int main(int argc, char ** argv) {

  // input parameters (basic block size, memory length)
  int b, M, c;
	char *bchar, *schar;
	extern char *optarg;
    
    //takes the standard input and sets b and M
	while ((c = getopt(argc, argv, "b:s:")) != -1)
		switch (c) {
			case 'b':
				bchar = optarg;
				b = atoi(bchar);
				break;
			case 's':
				schar = optarg;
				M = atoi(schar);
				break;
		}

  // init_allocator(basic block size, memory length)
  init_allocator(b,M);

  ackerman_main();
    /*
    int x = 0;
    int y = 0;
    int z = 0;
    int q = 0;
    int test = 0;
    x = my_malloc(10);
    y = my_malloc(40);
    z = my_malloc(200);
    q = my_malloc(200);
    
    volatile unsigned int *a = (volatile unsigned int *)(x-4);
    printf("%u\n", *a);
    volatile unsigned int *s = (volatile unsigned int *)(y+64-4);
    printf("%u\n", *s);
    volatile unsigned int *t = (volatile unsigned int *)(z-4);
    printf("%u\n", *t);
    volatile unsigned int *d = (volatile unsigned int *)(q-4);
    printf("%u\n", *d);
    
    printf("X = %d Y = %d Z = %d Q = %d\n", x, y, z, q);
    
    my_free(y);
    my_free(x);
    my_free(z);
    my_free(q);
    //test = my_malloc(400);
    a = (volatile unsigned int *)(x-4);
    printf("%u\n", *a);
    //q = my_malloc(1000);
    //printf("X = %d Y = %d Z = %d Q = %d Test = %d\n", x, y, z, q, test);
    */
    
   release_allocator();
}
