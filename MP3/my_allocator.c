/* 
    File: my_allocator.c

    Author: <Daniel Frazee>
            Department of Computer Science
            Texas A&M University
    Date  : <3/22/2015>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "my_allocator.h"
#include <math.h>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

int number_of_blocks;
int list_size;
struct head *address;

struct head {
    struct head *next;
    int key;
    int free_byte;     //whether the block is full or empty
    int block_size;

};

unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length) {
/* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. If an error occurred, 
   it returns 0. 
*/ 
    unsigned int temp;
    unsigned int temp2;
    
    temp = log2(_length);
    if (_length == pow(2, temp)) {
        temp = log2(_length);
        number_of_blocks = (pow(2, temp))/_basic_block_size;
    } else {
        temp = log2(_length) + 1;
        number_of_blocks = (pow(2, temp))/_basic_block_size;
    }
    printf("Power of length = %d\n", temp);
    printf("Number of blocks = %d\n", number_of_blocks);
    
    /* Initial memory allocation */
    address = (struct node *) malloc(pow(2, temp)); //allocates memory
    address->next = 0;
    address->key = 5555;
    address->free_byte = 0;
    address->block_size = pow(2, temp);
    list_size = pow(2, temp);
    return address;
    
    

}

int release_allocator() {
    free(address);
}

extern Addr my_malloc(unsigned int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
    struct head *new_block;
    unsigned int temp;
    new_block = address;
    
    temp = log2(_length+20);
    if ((_length+20) == pow(2, temp)) {
        temp = log2(_length+20);
    } else {
        temp = log2(_length+20) + 1;
    }
    
    if (new_block->block_size == pow(2, temp) && new_block->free_byte == 0) {
        new_block->free_byte = 1;
        return (char *) new_block + 20;
    } 
    
    while (new_block->free_byte == 1 || new_block->block_size < pow(2, temp)) {
        if (new_block->next == 0) {
            printf("List is full\n");
            break;
        }
        //printf("key = %d", new_block->block_size);
        new_block = new_block->next;
        //printf("test2");
    }
    
    while (new_block->block_size >= pow(2, temp) && new_block->free_byte == 0) {
                
        if (new_block->block_size == pow(2, temp) && new_block->free_byte == 0) {
            new_block->free_byte = 1;
            return (char *) new_block + 20; 
        }
    
        int checker = 0;
        if (new_block->next == 0) {
            checker = 1;
        }
        //printf("test");
        new_block->block_size = new_block->block_size/2;
        new_block->next = (char *) new_block + new_block->block_size;
        int temp_block_size = new_block->block_size;
        new_block = new_block->next;
        if (checker == 1) {        
            new_block->next = 0;
        } else {
            new_block->next = (char *) new_block + temp_block_size;
        }
        new_block->key = 1234;
        new_block->free_byte = 0;
        new_block->block_size = temp_block_size;
        new_block = (char *) new_block - temp_block_size;
    }
    
  
  return malloc((size_t)_length);
}

extern int my_free(Addr _a) {
  /* Same here! */
  struct head *free_block;
  struct head *temp_block;
  int temp_block_size;
  int temp_address1;
  int temp_address2;
  
  free_block = _a;
  free_block = (char *) free_block - 20;
  temp_block_size = free_block->block_size;
  //printf("Block size: %d\n", free_block->block_size);
  
  temp_address1 = free_block;
  //printf("First address: %d\n", temp_address);
  
  temp_address2 = temp_address1 ^ (1 << (int)floor (log2(temp_block_size)));
  //printf("Second address: %d\n", temp_address);
  
  temp_block = temp_address2;
  
  while ((temp_block->free_byte) == 0 && temp_block->block_size == free_block->block_size) {
    if (free_block->next == temp_address2) {
        free_block->block_size = (free_block->block_size) * 2;
        
        if (temp_block->next == 0) {
            free_block->next = 0;
        } else {
            free_block->next = (char *) free_block + free_block->block_size;
        }
        temp_address2 = free_block->next;
        free_block->free_byte = 0;
    } else {
        free_block = temp_address2;
        free_block->block_size = (free_block->block_size) * 2;
        if (temp_block->next == 0) {
            free_block->next = 0;
        } else {
            free_block->next = (char *) free_block + free_block->block_size;
        }
        temp_address2 = free_block->next;
        free_block->free_byte = 0;
    }
    temp_block = temp_address2;
  }
  
  free_block->free_byte = 0;
  
  //free(_a);
  return 0;
}






























