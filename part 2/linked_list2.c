#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list2.h"

struct node *listHead;  //head pointer
int node_length;
int number_of_nodes;
int tiers, tier_size;
int val = 2147483647; //2^31-1 signed values
struct node *aop[16]; //array of pointers

struct node {
    struct node *next;
    int k;
    int length;

};

void 	Init (int M, int b, int t){
    //Create t memory pools
    int i;

    struct node *temp; //temporary pointer to head of new tier memory pool
    for (i=0;i<t;i++)
    {
        
        temp = (struct node *) malloc(M*sizeof(*listHead));
        aop[i] = temp;
        temp=NULL;

    }

    node_length = b;    //sets each node to b bytes
    number_of_nodes = (M*t)/b; //total number of nodes

    tiers = t;
    tier_size = val/t;
    printf("Tier Size: %d\n",tier_size);

} 

void 	Destroy (){
    struct node *conductor;
    int i;
    for (i=0;i<tiers;i++)
    {
        //conductor = aop[i];
        free(aop[i]);
    }

    printf("\nMemory Pool Empty\n");
} 

int getTier(int k){
    
    int t, thash = 0;

    int lmin = 0;
    int lmax = tier_size;

    for (t=0;t<tiers;t++)
    {
       // printf("Lmin: %d \n",lmin);
        //printf("Lmax: %d \n",lmax);

        if (k>lmin && k<lmax){
            thash=t;
            return thash;
        } else {
            lmin = lmax;
            lmax += tier_size;
        }
    }

}

	 
int 	Insert (int key, char * value_ptr, int value_len){
    struct node *conductor;

    int i,t;
    t = getTier(key);
    for (i=0;i<tiers;i++){
        if (i==t){
            conductor = aop[i];
        }

    }
    
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
    
    int i,t;
    t = getTier(key);

    for (i=0;i<tiers;i++){
        if (i==t){
            conductor = aop[i];
        }

    }
    
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

    int i,t;
    t = getTier(key);

    for (i=0;i<tiers;i++){
        if (i==t){
            conductor = aop[i];
        }

    }

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
    
    int i;

    for (i=0;i<tiers;i++){

        conductor = aop[i];

        printf("Tier #: %d \n",i);
        printf("Printed List: \n");
        while (conductor->next != 0) {
            conductor = conductor->next;
            printf("Key = %d : ", conductor->k);
            printf("Value Length = %d\n", conductor->length);
            printf("Message: \"%s\"\n\n", (char *) conductor + 16);
        }
        printf("\n\n");

    }

/*
    printf("Printed List: \n");
    while (conductor->next != 0) {
            conductor = conductor->next;
            printf("Key = %d : ", conductor->k);
            printf("Value Length = %d\n", conductor->length);
            printf("Message: \"%s\"\n\n", (char *) conductor + 16);
    }
    printf("\n\n");
    */
}
