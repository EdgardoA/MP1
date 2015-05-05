/* 
MP5 - Daniel Frazee & Edgardo Angel
*/

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
#include <stdio.h>
#include <string>
#include <string.h>

#include "NetworkRequestChannel.H"
#include "semaphore.H"

using namespace std;

string itos(int num) {
   stringstream ss;
   ss << num;
   return ss.str();
}

// SERVER READ
string serverRead(int * fd) {
  char buf[255];
  
  if (read(*fd, buf, 255) < 0) {
    cout << "Error reading";
    exit(-1);
  }
  
  string s = buf;
  return s;
}

//SERVER WRITE
int serverWrite(int * fd, string info){
  if (info.length() >= 255) {
    cout << "Message too long";
    exit(-1);
  }
  
  if (write(*fd, info.c_str(), info.length()+1) < 0) {
    cout << "Error writing.";
    exit(-1);
  }
}

/* LOCAL FUNCTIONS -- INDIVIDUAL REQUESTS  --------------------------------*/

void processHello(int * fd, const string & _request)  {
  serverWrite(fd, "hello to you too");
}

void processData(int * fd, const string &  _request)  {
  usleep(1000 + (rand() % 4000));
  serverWrite(fd, itos(rand() % 100));
}

/* LOCAL FUNCTIONS -- THE PROCESS REQUEST LOOP  ----------------------------*/

void processRequest(int * fd, const string & _request) 
{
  if (_request.compare(0, 5, "hello") == 0) 
    processHello(fd, _request);
  
  else if (_request.compare(0, 4, "data") == 0) 
    processData(fd, _request);
  
  else 
    serverWrite(fd, "unknown request");

}

void * connection_handler(void * arg) 
{ 
  int * fd = (int*)arg;
  
  if (fd == NULL)
    cout << "No file descriptor!" << endl;
    
  for(;;)  {    
    string request = serverRead(fd);
    cout << "New request is " << request << endl;

    if (request.compare("quit") == 0) 
    {
      serverWrite(fd, "bye");
      usleep(100000);
      break;
    }

    processRequest(fd, request);
  }
  
  close(*fd);
  
}

/* MAIN FUNCTION  ---------------------------------------------------------*/

int main(int argc, char * argv[]) { 
  unsigned short port = 12357; 
  int backlog = 200;  
  
  int c; 
  unsigned short _port = 0;
  int _backlog = 0;
  
  while ((c = getopt(argc, argv, "p:b:")) != -1) {
    switch(c){
      case 'p':
        port = atoi(optarg);
        break;
      case 'b':
        backlog = atoi(optarg);
        break;
    }
  }
  
  cout << "port: " << port << "\t buffer: " << backlog << endl; 

  NetworkRequestChannel server(port, connection_handler, backlog);

  cout << "Server closing..." << endl;
  server.~NetworkRequestChannel();

}