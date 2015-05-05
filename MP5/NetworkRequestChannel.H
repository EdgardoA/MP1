/*
Network Request Channel by Daniel Frazee & Edgardo Angel
H File
*/

#ifndef _NETWORKREQUESTCHANNEL_H_                   
#define _NETWORKREQUESTCHANNEL_H_

#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include "semaphore.H"

using namespace std;

class NetworkRequestChannel {

private:

  int fd;

public:

  NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no);
  NetworkRequestChannel(const unsigned short _port_no, void * (*connection_handler) (void *), int backlog);
 
  ~NetworkRequestChannel(); 
  
  string cread();
  
  int cwrite(string _msg);
  
  int get_fd();
  
};

#endif