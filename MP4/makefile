# makefile

all: dataserver client

reqchannel.o: reqchannel.H reqchannel.C
	g++ -c -g reqchannel.C
	
semaphore.o: semaphore.H semaphore.C
	g++ -c -g semaphore.C -o semaphore.o
	
item.o: item.H item.C
	g++ -c -g item.C -o item.o
	
boundedbuffer.o: boundedbuffer.H boundedbuffer.C semaphore.H item.H
	g++ -c -g boundedbuffer.C -o boundedbuffer.o

dataserver: dataserver.C reqchannel.o 
	g++ -g dataserver.C reqchannel.o -o dataserver.o -lpthread
	
client: client.C reqchannel.o semaphore.o boundedbuffer.o item.o
	g++ -g client.C semaphore.o reqchannel.o boundedbuffer.o item.o -o client -lpthread
	
clean:
	rm -f *.o core
