/* 
    File: dataserver.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/16

    Modified by Daniel Frazee & Edgardo Angel
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
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "NetworkRequestChannel.H"
#include "semaphore.H"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const int MAX_MESSAGE = 255;

/*--------------------------------------------------------------------------*/
/* VARIABLES */
/*--------------------------------------------------------------------------*/

static int nthreads = 0;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

void handle_process_loop(int fd);

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

// NEW
string server_read(int * fd) {
  char buf[MAX_MESSAGE];
  

  if (read(*fd, buf, MAX_MESSAGE) < 0) {
    cout << "Error reading";
    exit(-1);
  }
  
  string s = buf;
  return s;
}

int server_write(int * fd, string message){
  if (message.length() >= MAX_MESSAGE) {
    cout << "Message too long";
    exit(-1);
  }
  
  if (write(*fd, message.c_str(), message.length()+1) < 0) {
    cout << "Error writing.";
    exit(-1);
  }
  
}

// END NEW

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- INDIVIDUAL REQUESTS */
/*--------------------------------------------------------------------------*/

// NEW

void process_hello(int * fd, const string & _request)  {
  server_write(fd, "hello to you too");
}

void process_data(int * fd, const string &  _request)  {
  usleep(1000 + (rand() % 5000));
 
  server_write(fd, int2string(rand() % 100));
}

// END NEW

/*
void process_hello(RequestChannel & _channel, const string & _request) {
  _channel.cwrite("hello to you too");
}

void process_data(RequestChannel & _channel, const string &  _request) {
  usleep(1000 + (rand() % 5000));
  //_channel.cwrite("here comes data about " + _request.substr(4) + ": " + int2string(random() % 100));
  _channel.cwrite(int2string(rand() % 100));
}

void process_newthread(RequestChannel & _channel, const string & _request) {
  int error;
  nthreads ++;

  // -- Name new data channel

  string new_channel_name = "data" + int2string(nthreads) + "_";
  //  cout << "new channel name = " << new_channel_name << endl;

  // -- Pass new channel name back to client

  _channel.cwrite(new_channel_name);

  // -- Construct new data channel (pointer to be passed to thread function)
  
  RequestChannel * data_channel = new RequestChannel(new_channel_name, RequestChannel::SERVER_SIDE);

  // -- Create new thread to handle request channel

  pthread_t thread_id;
  //  cout << "starting new thread " << nthreads << endl;
  if (error = pthread_create(& thread_id, NULL, handle_data_requests, data_channel)) {
    fprintf(stderr, "p_create failed: %s\n", strerror(error));
  }  

}
*/

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THE PROCESS REQUEST LOOP */
/*--------------------------------------------------------------------------*/

// NEW

void process_request(int * fd, const string & _request) 
{
  if (_request.compare(0, 5, "hello") == 0) 
    process_hello(fd, _request);
  
  else if (_request.compare(0, 4, "data") == 0) 
    process_data(fd, _request);
  
  else 
    server_write(fd, "unknown request");

}


void * connection_handler(void * arg) 
{ 
  int * fd = (int*)arg;
  
  if (fd == NULL)
    cout << "No file descriptor!" << endl;
    
  for(;;)  {    
    string request = server_read(fd);
    cout << "New request is " << request << endl;

    if (request.compare("quit") == 0) 
    {
      server_write(fd, "bye");
      usleep(10000);          // give the other end a bit of time.
      break;                  // break out of the loop;
    }

    process_request(fd, request);
  }
  
  close(*fd);
  
}

// END NEW

/*

void process_request(RequestChannel & _channel, const string & _request) {

  if (_request.compare(0, 5, "hello") == 0) {
    process_hello(_channel, _request);
  }
  else if (_request.compare(0, 4, "data") == 0) {
    process_data(_channel, _request);
  }
  else if (_request.compare(0, 9, "newthread") == 0) {
    process_newthread(_channel, _request);
  }
  else {
    _channel.cwrite("unknown request");
  }

}

void handle_process_loop(RequestChannel & _channel) {

  for(;;) {

    //cout << "Reading next request from channel (" << _channel.name() << ") ..." << flush;
    string request = _channel.cread();
    //cout << " done (" << _channel.name() << ")." << endl;
    //cout << "New request is " << request << endl;

    if (request.compare("quit") == 0) {
      _channel.cwrite("bye");
      usleep(10000);          // give the other end a bit of time.
      break;                  // break out of the loop;
    }

    process_request(_channel, request);
  }
  
}

*/

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

// NEW

int main(int argc, char * argv[]) { 
  unsigned short PORT = 12357; 
  int BACKLOG_BUF = 200;  
  
  int c; 
  unsigned short _port = 0;
  int _backlog = 0;
  
  while ((c = getopt(argc, argv, "p:b:")) != -1) {
    switch(c){
      case 'p':
        PORT = atoi(optarg);
        break;
      case 'b':
        BACKLOG_BUF = atoi(optarg);
        break;
    }
  }
  
  cout << "port: " << PORT << "\t buffer: " << BACKLOG_BUF << endl; 

  NetworkRequestChannel server(PORT, connection_handler, BACKLOG_BUF);

  cout << "Server closing..." << endl;
  server.~NetworkRequestChannel();

}

// END NEW

/*
int main(int argc, char * argv[]) {

  //  cout << "Establishing control channel... " << flush;
  RequestChannel control_channel("control", RequestChannel::SERVER_SIDE);
  //  cout << "done.\n" << flush;

  handle_process_loop(control_channel);

}
*/

