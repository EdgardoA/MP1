/* 
 
 MP4 - Dealing with a Reluctant Data Server

 Daniel Frazee & Edgardo Angel

*/

/* INCLUDES -----------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "item.H"
#include "reqchannel.H"
#include "semaphore.H"
#include "boundedbuffer.H"

using namespace std;

// GLOBALS VARIABLES -------------------------------------------------------*/

int n_requests;
int b_size;
int w_threads;

pthread_t joeThread;
pthread_t janeThread;
pthread_t johnThread;

pthread_t joeStat;
pthread_t janeStat;
pthread_t johnStat;

pthread_t workerThread;

Boundedbuffer reqBuffer;
Boundedbuffer resBuffer;

const int MESSAGE_SIZE = 255;
char READ_BUFFER[MESSAGE_SIZE];

const int MAX_CHANNELS = 155;

// file descriptors
int * reader;
int * writer;
int * ids;

const int NUM_PERSONS = 3;
const string PERSONS[NUM_PERSONS] = { "Joe Smith", "Jane Smith", "John Doe" };
const int STATISTICS_SIZE = 100;

long stats[NUM_PERSONS][STATISTICS_SIZE] = {0}; // initialize to zero

// Other Functions ---------------------------------------------------------*/

void write_initial_items() {
  for (int i = 0; i < w_threads; i ++) {
    Item* item;
    item = reqBuffer.getItem();
    
    string data = item->info;
    ids[i] = item->Id;
    
    write(writer[i], data.c_str(), strlen(data.c_str())+1);
  }
}

void printHorizontal(int l) {
  for (int j = 0; j < l; j ++) cout << "-";
  cout << endl;
}

void printStatistics() {
  cout << endl << endl;
  
  for (int i = 0; i < NUM_PERSONS; i ++) {
    int seg_total = 0;
    
    string header = "Statistics for " + PERSONS[i];
    printHorizontal(header.length());
    cout << header << endl;
    printHorizontal(header.length());
    
    for (int j = 0; j <= STATISTICS_SIZE; j ++) {
      if (j % 10 == 0 && j != 0) {
        cout << j - 10 << " - " << j - 1 << "\t\t" << seg_total << " times\t" << endl;
        
        seg_total = 0;
      } else {
        if (j != 100)
          seg_total += stats[i][j];
      }
    }
    printHorizontal(header.length());
    cout << "Total requests: " << n_requests << endl;
    printHorizontal(header.length());
    cout << endl;
  }     
}




// Thread Functions ---------------------------------------------------------*/

void* reqThreadRoutine(void* _person) {
  
  int person = *(int*)_person;
  
  for (int i = 0; i < n_requests; i ++) {
    Item* item = new Item;
    item->Id = person;
    item->info = "Info " + PERSONS[person];
    //cout << endl << "\t* * Request thread is depositing " << PERSONS[person] << "'s item in bb" << endl;
    //cout << "\t* * i = " << i << endl;
    
    reqBuffer.putItem(item);
  }
  
  delete (int*)_person;
  pthread_exit(NULL);
}

void* workerThreadRoutine(void* _nothing) {
  write_initial_items(); // write the initial items
  
  bool done = false;
  
  while(!done) {
    fd_set read_set;
    
    // zero em out
    FD_ZERO(&read_set);
    
    // set initially
    for (int i = 0; i < w_threads; i++) {
      FD_SET(reader[i], &read_set);
    }
    
    int ready = select(w_threads, &read_set, NULL, NULL, NULL);
    
    if (ready == -1) {
      cout << "Error calling select.";
      exit(1);
    }
    
    else {
      for (int i = 0; i < w_threads; i ++) {
        // if isset
        if(FD_ISSET(reader[i], &read_set)) {
          Item * item_r;
          read(reader[i], READ_BUFFER, MESSAGE_SIZE); // read the response
          
          string reply = READ_BUFFER;
          item_r = new Item(ids[i], reply, 0);
          
          resBuffer.putItem(item_r); // deposit that item in the statistics buffer
          
          if (!reqBuffer.empty()) {
            Item * item_w;
            
            item_w = reqBuffer.getItem();
            ids[i] = item_w->Id;
            string data = item_w->info;
            
            write(writer[i], data.c_str(), strlen(data.c_str())+1); // send the next item to the server now a spots opened up
          } else {
            usleep(10000); // make sure we're finished
            done = true;
          }
        }
      }
    }
    
    
  }
  
  pthread_exit(NULL);
}

void* statRoutine(void* nothing) {
  while (!reqBuffer.empty()) {
    while (!resBuffer.empty()) {
      Item* sta_item = resBuffer.getItem();
      
      int id = sta_item->Id;
      int value = atoi(sta_item->info.c_str());
      if (value < STATISTICS_SIZE - 1) {
        stats[id][value] += 1;
      } else {
        // max statistics exceeded
      }
    
      delete sta_item; // finally clean up this item
    }
  }
  
  pthread_exit(NULL);
}

// MAIN FUNCTION -----------------------------------------------------------*/

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

    // set arrays
  reader =  new int[w_threads];
  writer =  new int[w_threads];
  ids =   new int[w_threads];

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

    cout << "Creating Request Channels" << endl;
    for (int i = 0; i < w_threads; i ++) {
      string channel_name = chan.send_request("newthread");
      RequestChannel * channel = new RequestChannel(channel_name, RequestChannel::CLIENT_SIDE);
      
      reader[i] = channel->read_fd();
      writer[i] = channel->write_fd();
    }

    cout << "Creating Worker Threads" << endl;
    pthread_create(&workerThread, NULL, workerThreadRoutine, NULL);

    cout << "Creating Statistic Threads" << endl;
    pthread_create(&joeStat, NULL, statRoutine, NULL);
    pthread_create(&janeStat, NULL, statRoutine, NULL);
    pthread_create(&johnStat, NULL, statRoutine, NULL);
    
    cout << "Creating Histogram" << endl;
    pthread_join(workerThread, NULL);
    
    printStatistics();

    cout << "Closing..." << endl;
    REPLY = chan.send_request("quit");
    cout << "\tSERVER: " << REPLY << endl;

  }

  usleep(1000000);
}

