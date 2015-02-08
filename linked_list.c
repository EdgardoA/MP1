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

void 	Init (int M, int b){
    listHead = (struct node *) malloc(M);
    //printf("Address: %u\n", listHead);
    listHead->next = 0;
    node_length = b;
    number_of_nodes = M/b;
} 

void 	Destroy (){
    //printf("Address: %u\n", listHead);
    //free(listHead);                       //not working for some reason
} 	
	 
int 	Insert (int key, char * value_ptr, int value_len){
    struct node *conductor;
    conductor = listHead;
    
    if (number_of_nodes == 0) {
        printf("List is full\n");
        return 0;
    }
    
    if (conductor != 0) {
        while (conductor->next != 0) {
            conductor = conductor->next;
        }
    }
    
    if(16 + value_len > node_length) {
        printf("String too long for node\n");
        return 0;
    }    
    conductor->next = (char *) conductor + node_length;
    conductor = conductor->next;
    conductor->next = 0;
    conductor->k = key;
    conductor->length = value_len;
    memcpy((char *) conductor + 16, value_ptr, value_len);
    number_of_nodes--;
    //printf("nodes: %d\n", number_of_nodes); 
    //conductor->value_pointer = value_ptr;
    //printf("\nkey = %d\n", conductor->k);
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


