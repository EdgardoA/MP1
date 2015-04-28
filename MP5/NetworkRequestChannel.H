/*

Network Request Channel by Daniel Frazee & Edgardo Angel
H File

*/

#ifndef _NETWORKREQUESTCHANNEL_H_                   
#define _NETWORKREQUESTCHANNEL_H_


#include <iostream>
#include <fstream>
#include <string>
#include "semaphore.H"

using namespace std;

class NetworkRequestChannel {

private:

  int fd;

public:

  // From mp5.pdf

  NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no);
  NetworkRequestChannel(const unsigned short _port_no, void * (*connection_handler) (void *), int backlog);
 
  ~NetworkRequestChannel();  /* Destructor of the local copy of the channel. */

  string send_request(string _request); /* Send a string over the channel and wait for a reply. */
  
  string cread(); /* Blocking read of data from the channel. Returns a string of characters read from the channel. Returns NULL if read failed. */
  
  int cwrite(string _msg);  /* Write the data to the channel. The function returns the number of characters written to the channel. */
  
  int get_fd(); // gets the file descriptor
  
};


#endif


