/* 
MP5 - Daniel Frazee & Edgardo Angel
*/

/* INCLUDES -------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <queue>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>


#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>

#include <errno.h>

#include "NetworkRequestChannel.H"
#include "semaphore.H"
#include "BoundedBuffer.H"
#include "item.H"

using namespace std;

/* CONSTANTS  -------------------------------------------------------------*/

const string users[3] = { "Joe Smith", "Jane Smith", "John Doe" };

/* GLOBALS  ---------------------------------------------------------------*/

int n_requests;
int b_size;
int w_threads;

BoundedBuffer reqBuffer;
BoundedBuffer resBuffer;

pthread_t workerThread;

pthread_t joeThread;
pthread_t janeThread;
pthread_t johnThread;

void* joe = new int(0);
void* jane = new int(1);
void* john = new int(2);

pthread_t joeStat;
pthread_t janeStat;
pthread_t johnStat;

char readBuffer[200];

int * reader =	new int[50];
int * writer =	new int[50];
int * ids =	new int[155];

long stats[3][100] = {0};

NetworkRequestChannel* nChanControl;
string HOST;
int PORT;
vector<NetworkRequestChannel*> channels;

/* FUNCTIONS --------------------------------------------------------------*/

void printStats() {
	
	for (int i = 0; i < 3; i ++) {
		int histogram = 0;
		
		cout << "Statistics for " << users[i] << "..." << endl;
		
		for (int j = 0; j <= 100; j ++) {
			if (j % 10 == 0 && j != 0) {
				cout << j - 10 << " - " << j - 1 << "\t\t" << histogram << '\n';
				
				histogram = 0;
			} else {
				if (j != 100)
					histogram += stats[i][j];
			}
		}
		cout << "Total requests: " << n_requests << '\n' << endl;
	}			
}

void startWorkerChannels() {
	for (int i = 0; i < w_threads; i ++) {
		Item * item;
		item = reqBuffer.RetrieveItem();
		
		string data = item->data;
		ids[i] = item->id;
		
		channels[i]->cwrite(data.c_str());
	}
}

/* THREAD FUNCTIONS --------------------------------------------------------*/

void* reqThreadRoutine(void* _person) {
	int person = *(int*)_person;
	
	for (int i = 0; i < n_requests; i ++) {
		Item* item = new Item;
		item->id = person;
		item->data = "data " + users[person];
		reqBuffer.depositItem(item);
	}
	
	delete (int*)_person;
	pthread_exit(NULL);
}

void* workerThreadRoutine(void* _nothing) {
	startWorkerChannels();
	
	bool done = false;
	int maxfd = 0;
	
	while(!done) {
		fd_set read_set;
		
		//FD_ZERO(&read_set);
		
		for (int i = 0; i < w_threads; i++) {
			FD_SET(channels[i]->get_fd(), &read_set);
		}

		for(int i = 0; i < w_threads; i++)
		{
			if (channels[i]->get_fd() > maxfd) maxfd = channels[i]->get_fd();
		}
		
		int ready = select(maxfd + 1, &read_set, NULL, NULL, NULL);

		if (ready == -1) {
			cout << "Error calling select.";
			exit(1);
		}
		
		else {
		
			for (int i = 0; i < w_threads; i ++) {
				if(FD_ISSET(channels[i]->get_fd(), &read_set)) {
					Item * item_r;

					string reply = channels[i]->cread();
					
					item_r = new Item(ids[i], reply, 0);
					
					resBuffer.depositItem(item_r);

					if (!reqBuffer.isEmpty()) {
						Item * item_w;
						
						item_w = reqBuffer.RetrieveItem();
						ids[i] = item_w->id;
						string data = item_w->data;
						
						channels[i]->cwrite(data.c_str());

					} else {
						usleep(10000);
						done = true;
					}
				}
			}
		}

	}

	for (int i = 0; i < w_threads; i ++) {
		channels[i]->cwrite("quit");
		usleep(1000);
		
	}

	pthread_exit(NULL);
	
}

void* statRoutine(void* nothing) {
	while (!reqBuffer.isEmpty()) {
		while (!resBuffer.isEmpty()) {
			Item* sta_item = resBuffer.RetrieveItem();
			
			int id = sta_item->id;
			int value = atoi(sta_item->data.c_str());
			if (value < 100 - 1) {
				stats[id][value] += 1;
			} else {
				
			}
		
			delete sta_item;
		}
	}
	
	pthread_exit(NULL);
}

/* MAIN FUNCTION  ---------------------------------------------------------*/

int main(int argc, char * argv[]) {
	
	int c, n, b, w;
	string hostStr;

	//for debugging
	HOST = "localhost";
	PORT = 12357;
   
   	while((c = getopt(argc, argv, "n:b:w:h:p:")) != -1) {
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
			case 'h':
				hostStr = optarg;
				HOST = hostStr;
				break;
			case 'p':
				PORT = atoi(optarg);
				break;
			case '?':
				break;
			default:
				exit(0);
		}
    }   

	cout << "\nCLIENT STARTED...\n" << endl;
	cout << "---Number of data requests per person: " << n_requests << '\n' << endl ;
	cout <<	"---Size of bounded buffer in requests: " << b_size <<  '\n' << endl;
	cout << "---Number of worker threads: " << w_threads <<  '\n' << endl;
	cout << "---Server: " << HOST <<  '\n' << endl;
	cout << "---Port: " << PORT <<  '\n' << endl;

	cout << "Creating Data Server..." << endl;
	pid_t child_pid = fork();
	
	// child
	if (child_pid == 0) {
		execl("dataserver.o", "", 0);
	}
	// parent
	else {
		usleep(100000);
		
		cout << "Creating Request Buffer...\n" << endl;
		reqBuffer = BoundedBuffer(b_size);
		cout << "Creating Response Buffer...\n" << endl;
		resBuffer = BoundedBuffer(b_size);
		 
		cout << "Creating Request Threads...\n" << endl;
		
		pthread_create(&joeThread, NULL, reqThreadRoutine, joe);
		pthread_create(&janeThread, NULL, reqThreadRoutine, jane);
		pthread_create(&johnThread, NULL, reqThreadRoutine, john);
		
		cout << "Creating Request Channels...\n" << endl;
		for (int i = 0; i < w_threads; i ++) {
			NetworkRequestChannel * channel = new NetworkRequestChannel(HOST,PORT);
			channels.push_back(channel);
		}
				
		cout << "Creating Worker Threads...\n" << endl;
		pthread_create(&workerThread, NULL, workerThreadRoutine, NULL);
		
		cout << "Creating Statistics Thread...\n" << endl;
		pthread_create(&joeStat, NULL, statRoutine, NULL);
		pthread_create(&janeStat, NULL, statRoutine, NULL);
		pthread_create(&johnStat, NULL, statRoutine, NULL);
		
		cout << "Creating Histogram...\n" << endl;

        pthread_join(workerThread, NULL);
		
		printStats();
		
		cout<<"Closing Everything..." << endl;
		
		
		/*
		cout << "Closing...\n" << endl;
		REPLY = chan.send_request("quit");
		cout << "***Server Response: " << REPLY << "***\n" << endl;
		*/

		usleep(1000000);
	}
	return 0;
}