# makefile

all: dataserver client

NetworkRequestChannel.o: NetworkRequestChannel.H NetworkRequestChannel.C
	g++ -c -g NetworkRequestChannel.C
	
semaphore.o: semaphore.H semaphore.C
	g++ -c -g semaphore.C -o semaphore.o
	
item.o: item.H Item.C
	g++ -c -g Item.C -o item.o
	
BoundedBuffer.o: BoundedBuffer.H BoundedBuffer.C semaphore.H item.H
	g++ -c -g BoundedBuffer.C -o BoundedBuffer.o

dataserver: dataserver.C NetworkRequestChannel.o 
	g++ -g dataserver.C NetworkRequestChannel.o -o dataserver -lsocket -lnsl
	
client: client.C NetworkRequestChannel.o semaphore.o BoundedBuffer.o item.o
	g++ -g client.C semaphore.o NetworkRequestChannel.o BoundedBuffer.o item.o -o client -lsocket -lnsl
	
clean:
	rm -f *.o core
