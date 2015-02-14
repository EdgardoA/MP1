#include <stdio.h>
#include "linked_list.h"

struct node *listHead;  //head pointer
int node_length;
int number_of_nodes;

struct node {
    struct node *next;
    int k;
    int length;

};

//Initializes list
void 	Init (int M, int b){
    listHead = (struct node *) malloc(M*sizeof(*listHead)); //allocates memory
    listHead->next = 0;
    node_length = b;        //sets each node to b bytes
    number_of_nodes = M/b;  //global variable for max number of nodes
} 

//frees up memory taken by list
void 	Destroy (){
    free(listHead);
} 	

//inserts node
int 	Insert (int key, char * value_ptr, int value_len){
    struct node *conductor;
    conductor = listHead;
    
    if (number_of_nodes == 0) {
        printf("List is full\n");
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
        printf("String too long for node\n");
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
    conductor = listHead;
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
    printf("%d is not in the list.\n", key); //not found
}

//traverses list for key
char* 	Lookup (int key){
    struct node *conductor;
    conductor = listHead;
    if(conductor->k == key) {
        return (char *) conductor + 8; //moves 8 bytes forward where the key begins in the node
    }
    while (conductor->next != 0) { 
        conductor = conductor->next;
        if(conductor->k == key) {
            return (char *) conductor + 8; //moves 8 bytes forward where the key begins in the node
        }
    }
    return NULL;
}

//traverses list and prints required elements
void 	PrintList (){
    struct node *conductor;
    conductor = listHead;
    printf("Printed List: \n");
    while (conductor->next != 0) {
            conductor = conductor->next;
            printf("Key = %d : ", conductor->k);
            printf("Value Length = %d\n", conductor->length);
    }
    printf("\n\n");
}


