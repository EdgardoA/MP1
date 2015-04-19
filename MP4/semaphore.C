#include <pthread.h>
#include "semaphore.H"
#include <iostream>

using namespace std;


Semaphore::Semaphore(int _val) {
	value = _val;
	pthread_cond_init(&c, NULL);
	pthread_mutex_init(&m, NULL);
}

Semaphore::~Semaphore() 
{
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&c);
}

int Semaphore::P() {
	Lock();
	while (value <= 0) {
		pthread_cond_wait(&c, &m);
	}
	value --;
	Unlock();
	
	return 0;
}

int Semaphore::V() {
	Lock();
	value ++;
	pthread_cond_broadcast(&c);
	Unlock();
	
	return 0;
}