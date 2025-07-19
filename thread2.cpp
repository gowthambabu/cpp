#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include "circular_buffer.h"

#define MAX_THREADS 2

using namespace std;

void* (*p[2]) (void *args);

char *buff = "Hi hello how are you doing.";
pthread_cond_t cond_var_prod, cond_var_cons;
pthread_mutex_t lock, lock1, lock2;
uint8_t counter;

//Use same lock, producer will produce till buffer full, then consumer will process it.
//Use individual lock1 -> prod, lock2 -> cons (for locking the critical section, then it will switch between the threads

//producer
void * producer(void *args) {
	cout<<"tid "<<*((int*)args)<<endl;
	while (1) {
		pthread_mutex_lock(&lock1);
		if (isFull()) {
			pthread_cond_wait(&cond_var_cons, &lock);
		}
		cout<<"producing..."<<unsigned(counter)<<endl;
		insert(counter);
		counter++;
		sleep(0.5);
		cout<<"Signal the consumer thread"<<endl;
		pthread_cond_signal(&cond_var_prod);
		pthread_mutex_unlock(&lock1);
	}
	return NULL;
}

//consumer
void * consumer(void *args) {
	cout<<"tid "<<*((int*)args)<<endl;
	while (1) {
		pthread_mutex_lock(&lock2);
		if (isEmpty()) {
			cout<<"buffer empty wait..."<<endl;
			pthread_cond_wait(&cond_var_prod, &lock);
			cout<<"signal from producer thread"<<endl;
		}
		cout<<"consuming..."<<unsigned(peek())<<endl;
		remove();
		sleep(0.1);
		pthread_cond_signal(&cond_var_cons);
		pthread_mutex_unlock(&lock2);
	}
	return NULL;
}

int main() {
	pthread_t t[2];
	int cnt[2] = {1, 2}, i;
	p[0] = &producer;
	p[1] = &consumer;
	//cond_var_prod = cond_var_cons = PTHREAD_COND_INITIALIZER;
	//lock = lock1 = lock2 = PTHREAD_MUTEX_INITIALIZER;
	
	
   pthread_mutex_init(&lock, NULL);
   pthread_mutex_init(&lock1, NULL);
   pthread_mutex_init(&lock2, NULL);
   pthread_cond_init(&cond_var_cons, NULL);
   pthread_cond_init(&cond_var_prod, NULL);
   
	init_queue(50);
	counter = 0;
	for (i=0;i<MAX_THREADS;i++) {
		pthread_create(&t[i], NULL, p[i], &cnt[i]);
	}
	
	for (i=0;i<MAX_THREADS;i++) {
		pthread_join(t[i], NULL);
	}
	
	pthread_mutex_destroy(&lock);
	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);
	pthread_cond_destroy(&cond_var_cons);
	pthread_cond_destroy(&cond_var_prod);
	return 0;
}
