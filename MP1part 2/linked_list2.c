/*
 Machine Problem #1 - Part 2
 Daniel Frazee & Edgardo Angel
*/

/*
 Run & Compile
 gcc *.c
 ./a.out -b 128 -s 1408 -t 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list2.h"

struct node *listHead;  //head pointer
int node_length;
int number_of_nodes;
int tiers, tier_size; //tier variables
int val = 2147483647; //2^31-1 signed values
struct node *aop[16];

struct node {
    struct node *next;
    int k;
    int length;

};

//Initializes tiers of memory pools
void 	Init (int M, int b, int t){
    //Create t memory pools
    int i;

    struct node *temp; //temporary pointer to head of new tier memory pool

    for (i=0;i<t;i++)
    {
        //Create new memory pool and assign it to the next available pointer
        temp = (struct node *) malloc(M*sizeof(*listHead));
        aop[i] = temp;
        temp=NULL;

    }

    node_length = b;    //sets each node to b bytes
    number_of_nodes = (M*t)/b; //total number of nodes

    //set tier variables from Init function
    tiers = t;
    tier_size = val/t;

} 

//Frees up memory taken my the tiers of memory pools
void 	Destroy (){
    int i;
    for (i=0;i<16;i++)
    {
        free(aop[i]);
    }
    
} 

// Helper function to get the tier the key is located at.
struct node * getTier(int k){
    int t, thash = 0;

    int lmin = 0;
    int lmax = tier_size;

    //Traverse through the tiers to find where the key is located
    for (t=0;t<tiers;t++)
    {
        if (k>lmin && k<lmax){
            thash=t;

            int i;
            for (i=0;i<tiers;i++){
                if (i==thash){
                    return &aop[i];
                }
            }
            
        } else {
            lmin = lmax;
            lmax += tier_size;
        }
    }

}

//Inserts a node
int 	Insert (int key, char * value_ptr, int value_len){
    struct node *conductor;

    //Grabs the header pointer to the tier the key will be deleted from.
    conductor = getTier(key);
    
    if (number_of_nodes == 0) {
        printf("List is full.\n");
        return 0;
    }

    //traverses through the list until it reaches the end
    if (conductor != 0) {
        while (conductor->next != 0) {
            conductor = conductor->next;
        }
    }
    
    //checks length of value (must fit inside of node)
    if(16 + value_len > node_length) {
        printf("String too long for node.\n");
        return 0;
    } 

    conductor->next = (char *) conductor + node_length;
    conductor = conductor->next;    //sets conductor to new end node
    conductor->next = 0;            //sets new end node pointer to 0
    conductor->k = key;             //sets node's key
    conductor->length = value_len;  //sets node's value length
    //moves the pointer 16 bytes forward to where the string value should begin
    memcpy((char *) conductor + 16, value_ptr, value_len); 
    number_of_nodes--; //subtracts a node to prevent more than max nodes from being inserted

}

//searches for key then deletes
int 	Delete (int key){
    struct node *conductor;
    
    //Grabs the header pointer to the tier the key will be deleted from.
    conductor = getTier(key);
    
    //checks listhead for key before traversing
    if(conductor->k == key) {
        listHead = (char *) listHead + node_length;
        return 0;
    }

    //traverses list looking for key
    while (conductor->next != 0) { 
        conductor = conductor->next;
        if(conductor->k == key) { //if found, makes prev node pointer point two nodes ahead
            conductor = (char *) conductor - node_length;
            conductor->next = (char *) conductor + (2*node_length);
            return 0;
        }
    }
    printf("\n*** \n Cannot delete %d. It's not in the list.\n***\n", key); //Key is not found.
}

//traverses list for key
char* 	Lookup (int key){
    struct node *conductor;

    //Grabs the header pointer to the tier the key will be deleted from.
    conductor = getTier(key);

    if(conductor->k == key) {
        return (char *) conductor + 8; //Grabs the header pointer to the tier the key will be deleted from.
    }
    while (conductor->next != 0) { 
        conductor = conductor->next;
        if(conductor->k == key) {
            return (char *) conductor + 8;//moves 8 bytes forward where the key begins in the node
        }
    }
    return NULL;
}

//traverses list and prints required elements
void 	PrintList (){
    struct node *conductor;
    
    int i;

    //Traverse through all the elements on all the tiers and prints them
    for (i=0;i<tiers;i++){

        conductor = aop[i];

        printf("Tier #%d... \n",(i+1));
        printf("Printed List: \n");
        while (conductor->next != 0) {
            conductor = conductor->next;
            printf("Key = %d // ", conductor->k);
            printf("Value Length = %d // ", conductor->length);
            printf("Message: \"%s\"\n\n", (char *) conductor + 16);
        }
        printf("\n");

    }

}
