# makefile

all: dataserver client

reqchannel.o: reqchannel.H reqchannel.C
	g++ -c -g reqchannel.C
	
semaphore.o: semaphore.H semaphore.C
	g++ -c -g semaphore.C -o semaphore.o
	
item.o: item.H Item.C
	g++ -c -g Item.C -o item.o
	
BoundedBuffer.o: BoundedBuffer.H BoundedBuffer.C semaphore.H item.H
	g++ -c -g BoundedBuffer.C -o BoundedBuffer.o

dataserver: dataserver.C reqchannel.o 
	g++ -g dataserver.C reqchannel.o -o dataserver.o -lpthread
	
client: client.C reqchannel.o semaphore.o BoundedBuffer.o item.o
	g++ -g client.C semaphore.o reqchannel.o BoundedBuffer.o item.o -o client -lpthread
	
clean:
	rm -f *.o core
