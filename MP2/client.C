/* 
    File: client.C

    Author: Daniel Frazee & Edgardo Angel

    Date  : 02/20/15

    Run & Compile:
    make
    ./client
*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <errno.h>
#include <unistd.h>

#include "reqchannel.H"

using namespace std;

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  struct timeval req_str_single, req_end_single; // To measure a single request through the server
  struct timeval rep_str_single, rep_end_single; // To measure a single request through a function that return a reply

  struct timeval req_str_mult, req_end_mult; // To measure a multiple request through the server
  struct timeval rep_str_mult, rep_end_mult; // To measure a multiple request through a function that return a reply

  struct timeval str_time, end_time; // To measure timing for the entire program

  long timedif_ReqS, timedif_RepS, timedif_ReqM, timedif_RepM, timedif_total; // To measure difference between readings

  gettimeofday(&str_time,NULL); // Start timing the entire program

  cout << "CLIENT STARTED:" << endl;

  // Create the dataserver by using fork()

  pid_t child_pid = fork();
  
  if (child_pid == 0) {

    execl("dataserver", 0);

  }

  cout << "Establishing control channel... " << flush;
  RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
  cout << "done." << endl;

  //Single Request through Server

  gettimeofday(&req_str_single,NULL);
  string reply1 = chan.send_request("hello");
  cout << "Reply to request 'hello' is '" << reply1 << "'" << endl; 
  gettimeofday(&req_end_single,NULL);

  //Single Request through Function

  gettimeofday(&rep_str_single,NULL);
  string request_string("data TestPerson" + int2string(42));
  string reply_string = chan.send_request(request_string);
  gettimeofday(&rep_end_single,NULL);

  //Multiple Request through Server

  gettimeofday(&req_str_mult,NULL);
  for(int i = 0; i < 100; i++) {
    string reply_string = chan.send_request("hello");
    cout << "reply to request " << i << ":" << reply_string << endl;;
  }
  gettimeofday(&req_end_mult,NULL);

  //Mutliple Request through Function

  gettimeofday(&rep_str_mult,NULL);
  for(int i = 0; i < 100; i++) {
    string request_string("data TestPerson" + int2string(i));
    string reply_string = chan.send_request(request_string);
    cout << "reply to request " << i << ":" << reply_string << endl;;
  }
  gettimeofday(&rep_end_mult,NULL);

  string reply4 = chan.send_request("quit");
  cout << "Reply to request 'quit' is '" << reply4 << endl;

  gettimeofday(&end_time,NULL); // End timing the entire program

  //Calculates time for SINGLE request through SERVER
  timedif_ReqS = (req_end_single.tv_usec - req_str_single.tv_usec);
  
  //Calculates time for SINGLE request through FUNCTION
  timedif_RepS = (rep_end_single.tv_usec - rep_str_single.tv_usec);
  
  //Calculates time for MULTIPLE request through SERVER (100 iterations)
  timedif_ReqM = (req_end_mult.tv_usec - req_str_mult.tv_usec);
  
  //Calculates time for MULTIPLE request through FUNCTION (100 iterations)
  timedif_RepM = (rep_end_mult.tv_usec - rep_str_mult.tv_usec);

  //Calculates time for total time of program
  timedif_total = (end_time.tv_usec - str_time.tv_usec);

  printf("Timings...\n");
  printf("Total time for SINGLE request through LOCALLY: %ld microseconds.\n",timedif_ReqS);
  printf("Total time for SINGLE request through FUNCTION: %ld microseconds.\n",timedif_RepS);
  printf("Total time for MULTIPLE request through LOCALLY: %ld microseconds.\n",timedif_ReqM);
  printf("Total time for MULTIPLE request through FUNCTION: %ld microseconds.\n",timedif_RepM);
  printf("Total time of program: %ld musec. \n",timedif_total);

  usleep(1000000);
}
