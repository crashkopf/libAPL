/*
  event.c - Used to queue events for triggering or to measure the duration of an event.
*/
#include <stdlib.h>
#include "event.h"

/*
	eq_add - add an event to the queue.
	wait - number of timer ticks to wait before triggering the event
	callback - pointer to a callback funtion
	context - pointer passed to the callback funciton
	queue - the event queue to operate on
*/
event * eq_add(unsigned long wait, void (*callback)(void*), void * context, event_queue * eq) {
	event * e = NULL;
	int j;
	//Iterate through the event queue looking for the first NULL callback pointer
	for (j=eq->len - 1; j>=0; j--) {
		if (eq->e[j].callback == NULL) {
			e = &eq->e[j];
			break;
		}
	}
	//If we found one then we go ahead and add the event information to the queue
	if (e != NULL) {
		e->wait = wait;
		e->callback = callback;
		e->context = context;
	}
	// Return a pointer to the event.  If no free event was found this will be NULL.
	return e;
}

// eq_remove - remove an event from the queue.  Sets callback pointer to NULL
void eq_remove(event * e) {
	e->callback = NULL;
	e->context = NULL;
}

// eq_update - update the counters for all queued events
void eq_update(event_queue * eq, unsigned long t) {
	int j;
	for (j=eq->len - 1; j>=0; j--) {
		if (eq->e[j].callback != NULL) {
			if (eq->e[j].wait > 0) {
				if (eq->e[j].wait >= t) {
					eq->e[j].wait -= t;  //Decrement wait time by the number of ticks that have passed
				}
				else {
					eq->e[j].wait = 0;  //We've overshot the trigger time, so wait becomes 0
				}
			}
		}
	}
}

// eq_dispatch - Checks for events ready to fire, invokes their callback function, and removes them from the queue
void eq_dispatch (event_queue * eq) {
	int j;
	for (j=eq->len - 1; j>=0; j--) {
		if (eq->e[j].callback != NULL) {
			if (eq->e[j].wait == 0) {
				(eq->e[j].callback)(eq->e[j].context); //call the callback
				eq_remove(&eq->e[j]); // remove the event
			}
		}
	}
}

//Returns a pointer to the next queued event that will fire
event * eq_get_next_event (event_queue * eq) {
	event * e;
	int j;
	e = eq->e;
	for (j=eq->len - 1; j>=0; j--) {
		if (eq->e[j].callback != NULL) {	// Make sure this is a real event
			if ((e->wait > eq->e[j].wait) && (eq->e[j].wait > 0)) {		//Ignore events that will trigger on the next dispatch
				e = &eq->e[j];
			}
		}
	}
	return e;
}

// Find out how long we have to wait for the next event.
unsigned int eq_get_wait_time (event_queue * eq) {
	event * ne;
	ne = eq_get_next_event(eq);
	if (ne->callback != NULL) {
		return ne->wait;
	}
	return 0;
}
