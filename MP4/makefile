# makefile

all: dataserver client

reqchannel.o: reqchannel.H reqchannel.C
	g++ -c -g reqchannel.C

semaphore.o: semaphore.H semaphore.C
	g++ -c -g semaphore.C -o semaphore.o

boundedbuffer.o: boundedbuffer.H boundedbuffer.C
	g++ -c -g boundedbuffer.C -o boundedbuffer.o

dataserver: dataserver.C reqchannel.o 
	g++ -g -o dataserver dataserver.C reqchannel.o -lpthread

client: client.C reqchannel.o
	g++ -g -o client client.C reqchannel.o

clean:
	rm -f *.o core