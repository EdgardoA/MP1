/*
Network Request Channel by Daniel Frazee & Edgardo Angel
C File
*/

#include <cassert>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sstream>
#include <stdlib.h>

#include "NetworkRequestChannel.H"

using namespace std;

struct sockaddr_in serverInput;

// CLIENT SIDE
int createClientSide(const char * host, const char * port) {
	struct sockaddr_in socketInput;
	memset(&socketInput, 0, sizeof(socketInput));
	socketInput.sin_family = AF_INET;
	
	// Porting
	if (struct servent * pse = getservbyname(port, "tcp")) {
		socketInput.sin_port = pse->s_port;
	} else if ((socketInput.sin_port = htons((unsigned short)atoi(port))) == 0) { 
		cout << "Can't connect to port " << atoi(port);
		exit(-1);
	}
	
	// Host
	if (struct hostent * hn = gethostbyname(host)) {
		memcpy(&socketInput.sin_addr, hn->h_addr, hn->h_length);
	} else if((socketInput.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE) {
		cout << "Can't resolve host <" << host << ">";
		exit(-1);
	}
	
	// Socket
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		cout << "Can't establish socket";
		exit(-1);
	}
	
	// Connection
	if (connect(s, (struct sockaddr *)&socketInput, sizeof(socketInput)) < 0) {
		cout << "Can't connect to " << host << "::" << port;
		exit(-1);
	}
	
	return s;
}

// SERVER SIDE
int createServerSide(const char * svc, int backlog) {

	memset(&serverInput, 0, sizeof(serverInput));
	serverInput.sin_family = AF_INET;
	serverInput.sin_addr.s_addr = INADDR_ANY;
	
	// Mapping
	if (struct servent * pse = getservbyname(svc, "tcp")) {
		serverInput.sin_port = pse->s_port;
	} else if ((serverInput.sin_port = htons((unsigned short)atoi(svc))) == 0) {
		cout << "Can't get service entry";
		exit(-1);
	}

	// Binding
	int snum  = socket(AF_INET, SOCK_STREAM, 0);
	
	if (snum < 0) {
		cout << "Can't create socket";
		exit(-1);
	}
	
	if (bind(snum, (struct sockaddr *)&serverInput, sizeof(serverInput)) < 0) {
		cout << "Can't bind";
		exit(-1);
	}
	
	// Listening
	if (listen(snum, backlog) < 0) {
		cout << "Error trying to begin listening";
		exit(-1);
	}
		
	return snum;
}

// CONSTRUCTOR CLIENT
NetworkRequestChannel::NetworkRequestChannel(const string _server_host_name, const unsigned short _port) {
	stringstream ss;
	ss << _port;
	string port = ss.str();
	
	fd = createClientSide(_server_host_name.c_str(), port.c_str());
	
}

// CONSTRUCTOR SERVER
NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port, void * (*connection_handler) (void *), int backlog) {
	stringstream ss;
	ss << _port;
	string port = ss.str();
	
	int master_sock = createServerSide(port.c_str(), backlog);
	int serverSize = sizeof(serverInput);
	
	while (true) {
		int * tempSocket = new int;
		
		pthread_t thread;
		pthread_attr_t attr; 
		pthread_attr_init(&attr);
			
		*tempSocket = accept(master_sock,(struct sockaddr*)&serverInput, (socklen_t*)&serverSize);
		
		if (tempSocket < 0) {
			delete tempSocket;
			
			if (errno == EINTR) continue;
			else {
				cout << "Accept failure!";
				exit(-1);
			}
		}	
		
		pthread_create(&thread, &attr, connection_handler, (void*)tempSocket);
		
	}
	cout << "Connection established";
}

// DESTRUCTOR
NetworkRequestChannel::~NetworkRequestChannel() {
	close(fd);
}

// READ
string NetworkRequestChannel::cread() {
	char buf[255];
	
	if (read(fd, buf, 255) < 0) {
		perror("Can't read");
		exit(-1);
	}
	
	string s = buf;
	return s;
}

// WRITE
int NetworkRequestChannel::cwrite(string _msg) {
	if (_msg.length() >= 255) {
		cout << "Message exceeded 255";
		return -1;
	}
	
	const char * s = _msg.c_str();

	if (write(fd, s, strlen(s)+1) < 0) {
		perror("Can't write.");
		exit(-1);
	}
}

int NetworkRequestChannel::get_fd() {
	return fd; 
}
