#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

using namespace std;

#define MAX_THREADS 5

pthread_mutex_t lock;

int counter;
char *buff = "Hi hello How are you doing? I am good How is the work going on. Is it good time to talk? Hi hello How are you doing? I am good How is the work going on. Is it good time to talk? Back to the future";

void* func(void *args) {
	while (1) {
		int *val = (int*)args;
		pthread_mutex_lock(&lock);
		cout<<endl;
		cout<<"tid "<<*val<<endl;
		if (buff[counter] == ' ') counter++;
		while (buff[counter] != '\0' && buff[counter] != ' ') {
			cout<<buff[counter++];
		}
		//for (volatile int d = 0; d < 100000000; d++); // Delay loop
		pthread_mutex_unlock(&lock);
		if (buff[counter] == '\0') break;
	}
	return NULL;
}

int main() {
	pthread_t t[MAX_THREADS];
	int cnt[MAX_THREADS] = {1, 2, 3, 4, 5};
	int i;
	pthread_mutex_init(&lock, NULL);
	counter = 0;
	for (i=0;i<MAX_THREADS;i++) {
		pthread_create(&t[i], NULL, &func, &cnt[i]);
	}
	for (i=0;i<MAX_THREADS;i++) {
		pthread_join(t[i], NULL);
	}
	pthread_mutex_destroy(&lock);
	return 0;	
}
