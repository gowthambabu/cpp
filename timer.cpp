#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

typedef void (*timer_callback) (void);

struct timer_s {
	uint64_t delay_us;
	timer_callback timer_cb;
	bool active;
	struct timer_s *next;
};

uint16_t qlen;
struct timer_s *timerq = NULL;

void add_timer(timer_callback cb, uint16_t delay) {
	time_t now = time(NULL);
	printf("add_timer %llu\n", now);
	struct timer_s *t1 = (struct timer_s*) malloc (sizeof(struct timer_s));
	t1->active = true;
	t1->delay_us = now + delay;
	t1->next = NULL;
	t1->timer_cb = cb;
	qlen++;
	if (!timerq) {
		timerq = t1;
	} else {
		t1->next = timerq;
		timerq = t1;
	}
}


void poll_for_timer() {
	time_t now = time(NULL);
	struct timer_s *timer_itr = timerq;
	struct timer_s *prev = timerq;
	bool timer_elapsed = false;
	while(timer_itr && qlen) {
		timer_elapsed = false;
		now = time(NULL);
		//printf("poll for timer %llu, %llu\n", now, timer_itr->delay_us);
		if (timer_itr->active && timer_itr->delay_us <= now) {
			//trigger timer
			if (timer_itr->timer_cb) {
				timer_callback cb = timer_itr->timer_cb;
				cb();
			}
			timer_itr->active = false;
			timer_elapsed = true;
		}
		if (timer_elapsed) {
				//free the nodes
				if (prev == timer_itr) {
					timerq = timer_itr->next;
					free(timer_itr);
					timer_itr = prev = timerq;
				} else {
					prev->next = timer_itr->next;
					free(timer_itr);
				}
				qlen--;
		} else {
			prev = timer_itr;
			timer_itr = timer_itr->next;
		}
	}
}

void timer_cb1() {
	printf("Callback 1\n");
}

void timer_cb2() {
	printf("Callback 2\n");
}

void timer_cb3() {
	printf("Callback 3\n");
}

void timer_cb4() {
	printf("Callback 4\n");
}

void timer_cb5() {
	printf("Callback 5\n");
}

void timer_cb6() {
	printf("Callback 6\n");
}
int main() {
	qlen = 0;
	add_timer(timer_cb1, 1);
	add_timer(timer_cb2, 2);
	add_timer(timer_cb3, 3);
	add_timer(timer_cb4, 1);
	add_timer(timer_cb5, 2);
	add_timer(timer_cb6, 5);
	while (1) {
		//wait for all timer to exit
		if (!qlen) break;
		poll_for_timer();
	}
	return 0;
}
