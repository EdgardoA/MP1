#include <stdio.h>
#include "linked_list2.h"

struct node *listHead;  //head pointer
int node_length;
int number_of_nodes;
int tiers, tier_size;
int Q; //Entire tiered memory size
int val = 2147483647; //2^31-1 signed values

struct node {
    struct node *next;
    int k;
    int length;

};

void 	Init (int M, int b, int t){
    Q = M*t;
    listHead = (struct node *) malloc(Q*sizeof(*listHead));
    listHead->next = 0;
    node_length = b;
    number_of_nodes = M/b;
    tiers = t;
    tier_size=val/tiers;
    printf("Tier size: %d\n",tier_size);

    struct node *aop[t]; //array of pointers
    //Creating array of pointers, each pointing to the start of their tier
    int i;
    struct node *strnode;
    strnode = listhead;
    for (i=0; i<=t; i++)
    {
    	if (strnode == listhead){
    		aop[t] = strnode;
    	} else {
    		//Move strnode # of nodes up to the start of the next tier

    	}

    	/*

		probably best to create # of instances of memory allocation
		this will give the head pointer to each one of them.

    	*/


    	if (strnode != 0) {
        	while (strnode->next != 0) {
            	strnode = strnode->next;
        	}
    	}
    }
} 

void 	Destroy (){
    free(listHead);
} 	
	 
int 	Insert (int key, char * value_ptr, int value_len){
    struct node *conductor;
    conductor = listHead;
    
    if (number_of_nodes == 0) {
        printf("List is full\n");
        return 0;
    }

    //Does this move to the next available node??
    // If so,
    // When inserting it will have to shift conductor up to the next tier
    if (conductor != 0) {
        while (conductor->next != 0) {
            conductor = conductor->next;
        }
    }
    
    if(16 + value_len > node_length) {
        printf("String too long for node\n");
        return 0;
    } 

    //Adds Node
    conductor->next = (char *) conductor + node_length;
    conductor = conductor->next;
    conductor->next = 0;
    conductor->k = key;
    conductor->length = value_len;
    memcpy((char *) conductor + 16, value_ptr, value_len);
    number_of_nodes--;
}

int 	Delete (int key){
    struct node *conductor;
    conductor = listHead;
    if(conductor->k == key) {
        listHead = (char *) listHead + node_length;
        return 0;
    }
    while (conductor->next != 0) { 
        conductor = conductor->next;
        if(conductor->k == key) {
            conductor = (char *) conductor - node_length;
            conductor->next = (char *) conductor + (2*node_length);
            return 0;
        }
    }
    printf("%d is not in the list.\n", key);
}

char* 	Lookup (int key){
    struct node *conductor;
    conductor = listHead;
    if(conductor->k == key) {
        return (char *) conductor + 8;
    }
    while (conductor->next != 0) { 
        conductor = conductor->next;
        if(conductor->k == key) {
            return (char *) conductor + 8;
        }
    }
    return NULL;
}

void 	PrintList (){
    struct node *conductor;
    conductor = listHead;
    printf("Printed List: \n");
    while (conductor->next != 0) {
            conductor = conductor->next;
            printf("Key = %d : ", conductor->k);
            printf("Value Length = %d\n", conductor->length);
            printf("Message: \"%s\"\n\n", (char *) conductor + 16);
    }
    printf("\n\n");
}

int tier_alloc(int key, int tiers) {



	return 0;
}

