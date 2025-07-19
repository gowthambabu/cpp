#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include "circular_buffer.h"

#define MAXBUFFSIZE 65535-1

using namespace std;

uint8_t *buffer = NULL;
uint16_t front, rear, cap;

void init_queue(uint16_t size) {
	buffer = (uint8_t*)malloc(size*sizeof(uint8_t));
	front = 0;
	rear = 0;
	cap = size+1;	
}

bool isEmpty() {
	if (front == rear) {
		cout<<"IsEMpty()"<<endl;
		return true;
	}
	return false;
}

bool isFull() {
	//if (rear == cap) return true; -> linear Q
	//cout<<front<<" "<<rear<<endl;
	if ((rear+1)%cap == front) {
		cout<<"isFUll"<<endl;
		return true;
	}
	return false;
}

void insert(uint8_t data) {
	if (!isFull()) {
		buffer[rear] = data;
		rear = (rear+1)%cap;
	}
}

void remove() {
	if (!isEmpty()) {
		front = (front+1)%cap;
	}
}

uint16_t peek() {
	if (!isEmpty()) return buffer[front];
	return 65535;
}

void print_queue() {
	int i;
	cout<<"printing q..."<<endl;
	//cout<<"front "<<front<<" rear "<<rear<<" cap"<<cap<<endl;
	for (i=front;i!=rear;i = (i+1)%cap) {
		cout<<unsigned(buffer[i])<< " ";
	}
	cout<<endl;
}
void delete_queue() {
	free(buffer);
	front = rear = -1;
	cap = 0;
}

/*int main()
{
    std::cout<<"Hello World"<<endl;
	init_queue(5);
	insert(10);
	insert(20);
	insert(30);
	insert(40);
	insert(50);
	insert(60);
	print_queue();
	remove();
	print_queue();
	insert(60);
	print_queue();
	remove();
	print_queue();
	remove();
	print_queue();
	remove();
	print_queue();
	remove();
	print_queue();
	remove();
	print_queue();
	insert(100);
	print_queue();
	cout<<"Top "<<peek()<<endl;;
	delete_queue();
    return 0;
}*/
