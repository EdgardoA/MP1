/*

Network Request Channel by Daniel Frazee & Edgardo Angel
C File

*/

#include <cassert>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sstream>

#include "NetworkRequestChannel.H"

using namespace std;

struct sockaddr_in serverIn;

const int MAX_MESSAGE = 255;

// CLIENT CONNECTION

int createClientConnection(const char * host, const char * port) {
	struct sockaddr_in sockIn;
	memset(&sockIn, 0, sizeof(sockIn));
	sockIn.sin_family = AF_INET;
	
	// establish port
	if (struct servent * pse = getservbyname(port, "tcp")) {
		sockIn.sin_port = pse->s_port;
	} else if ((sockIn.sin_port = htons((unsigned short)atoi(port))) == 0) { 
		cout << "Cannot connect to port " << atoi(port);
		exit(-1);
	}
	
	// try to reach the host
	if (struct hostent * hn = gethostbyname(host)) {
		memcpy(&sockIn.sin_addr, hn->h_addr, hn->h_length);
	} else if((sockIn.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE) {
		cout << "Cannot resolve host <" << host << ">";
		exit(-1);
	}
	
	// establish a socket
	int s = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM finds a socket for a TCP connection
	if (s < 0) {
		cout << "Cannot establish socket";
		exit(-1);
	}
	
	// make the connection
	if (connect(s, (struct sockaddr *)&sockIn, sizeof(sockIn)) < 0) {
		cout << "Cannot connect to " << host << "::" << port;
		exit(-1);
	}
	
	return s;
}

// SERVER CONNECTION

int createServerConnection(const char * svc, int backlog) {

	memset(&serverIn, 0, sizeof(serverIn));
	serverIn.sin_family = AF_INET;
	serverIn.sin_addr.s_addr = INADDR_ANY;
	
	// map to the provided port
	if (struct servent * pse = getservbyname(svc, "tcp")) {
		serverIn.sin_port = pse->s_port;
	} else if ((serverIn.sin_port = htons((unsigned short)atoi(svc))) == 0) {
		cout << "Cannot get service entry";
		exit(-1);
	}
	
	
	// bind to the port
	int snum  = socket(AF_INET, SOCK_STREAM, 0);
	
	if (snum < 0) {
		cout << "Cannot create socket";
		exit(-1);
	}
	
	if (bind(snum, (struct sockaddr *)&serverIn, sizeof(serverIn)) < 0) {
		cout << "Cannot bind";
		exit(-1);
	}
	
	// begin listening for traffic
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
	
	fd = createClientConnection(_server_host_name.c_str(), port.c_str());
	
}

// CONSTRUCTOR SERVER

NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port, void * (*connection_handler) (void *), int backlog) {
	stringstream ss;
	ss << _port;
	string port = ss.str();
	
	int master_sock = createServerConnection(port.c_str(), backlog);
	int serverSize = sizeof(serverIn);
	

	while (true) {
		int * slave_sock = new int;
		
		pthread_t thread;
		pthread_attr_t attr; 
		pthread_attr_init(&attr);
			
		
		*slave_sock = accept(master_sock,(struct sockaddr*)&serverIn, (socklen_t*)&serverSize);
		
		if (slave_sock < 0) {
			delete slave_sock;
			
			if (errno == EINTR) continue;
			else {
				cout << "Accept failure!";
				exit(-1);
			}
		}	
		
		pthread_create(&thread, &attr, connection_handler, (void*)slave_sock);
		
 
	}
	cout << "Connection established";
}

// DESTRUCTOR
NetworkRequestChannel::~NetworkRequestChannel() {
	close(fd); 
}

// READ

string NetworkRequestChannel::cread() {
	char buf[MAX_MESSAGE];
	
	if (read(fd, buf, MAX_MESSAGE) < 0) {
		perror("Can't read");
		exit(-1);
	}
	
	
	string s = buf;

	return s;
}

// WRITE

int NetworkRequestChannel::cwrite(string _msg) {
	if (_msg.length() >= MAX_MESSAGE) {
		cout << "Message exceeded MAX_MESSAGE";
		return -1;
	}
	
	const char * s = _msg.c_str();

	if (write(fd, s, strlen(s)+1) < 0) {
		perror("Can't write.");
		exit(-1);
	}
	
	
}

/*--------------------------------------------------------------------------*/
/* helper  */
/*--------------------------------------------------------------------------*/

int NetworkRequestChannel::get_fd() { return fd; }


