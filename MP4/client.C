/* 
    File: simpleclient.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2013/01/31

    Simple client main program for MP3 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

#include <errno.h>

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <cstdlib>
#include <sys/time.h>
#include <sys/select.h>


#include "item.H"
#include "reqchannel.H"
#include "semaphore.H"
#include "boundedbuffer.H"

using namespace std;

// GLOBALS VARIABLES -------------------------------------------------------*

//RequestChannel* CHAN_CONTROL;

int n_requests;
int b_size;
int w_threads;

pthread_t joeThread;
pthread_t janeThread;
pthread_t johnThread;

Boundedbuffer reqBuffer;
Boundedbuffer resBuffer;

const int NUM_PERSONS = 3;
const string PERSONS[NUM_PERSONS] = { "Joe Smith", "Jane Smith", "John Doe" };
const int STATISTICS_SIZE = 100;



//Thread Functions

void* reqThreadRoutine(void* _person) {
  
  int person = *(int*)_person;
  
  for (int i = 0; i < n_requests; i ++) {
    Item* item = new Item;
    item->id = person;
    item->data = "data " + PERSONS[person];
    cout << endl << "\t* * Request thread is depositing " << PERSONS[person] << "'s item in bb" << endl;
    cout << "\t* * i = " << i << endl;
    reqBuffer.depositItem(item);
  }
  
  delete (int*)_person;
  pthread_exit(NULL);
}


/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {


  // Command Line
  int c, n, b, w;
   
    while((c = getopt(argc, argv, "n:b:w:")) != -1) {
        switch(c) {
        case 'n':
          n_requests = atoi(optarg);
          break;
        case 'b':
          b_size = atoi(optarg);
          break;
        case 'w':
          w_threads = atoi(optarg);
          break;
        case '?':
          break;
        default:
          exit(0);
      }
    } 


  cout << "CLIENT STARTED:" << endl;

  cout<<"\nData Requests per Person: " << n_requests << "\nSize of Buffer: " << b_size << "\nWorker Threads: " << w_threads ;

  cout << "\nCreating Data Server Process" << endl;
  pid_t child_pid = fork();

  // Child creates Data Server
  if (child_pid == 0) {
    execl("dataserver.o", 0);
  } 
  // Parent
  else {
  
    usleep(100000);

    //Create Bounded Buffers
    cout << "\nCreating Bounded Buffers" << endl;

    //Request Buffer
    reqBuffer = Boundedbuffer(b_size);
    //Response Buffer
    resBuffer = Boundedbuffer(b_size);

    cout << "\nConnect to Data Server" << endl;
    RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
    string REPLY =  chan.send_request("hello");
    cout << "\nSERVER: " << REPLY << endl;

    cout << "\nCreating Request Threads" << endl;

    void* joe = new int(0);
    pthread_create(&joeThread, NULL, reqThreadRoutine, joe);
  
    void* jane = new int(1);
    pthread_create(&janeThread, NULL, reqThreadRoutine, jane);

    void* john = new int(2);
    pthread_create(&johnThread, NULL, reqThreadRoutine, john);

  }

  usleep(1000000);
}

