typedef struct {
	unsigned long wait;  // Time to fire the event (in ticks)
	//unsigned long period;  // For re-occurring events
	//unsigned short flags;
	void (*callback)( void *); // Callback to execute when the event fires
	void * context; // Context pointer to use as argument to the callback
} event;



typedef struct {
	int len;
	event * e;
} event_queue;

// Add an event to the queue
event * eq_add(unsigned long, void (*callback)(void*), void *, event_queue *);
// Remove an event from the queue
void eq_remove(event *);
// eq_update - update the counters for all queued events
void eq_update(event_queue *, unsigned long);
// eq_dispatch - Checks for events ready to fire, invokes their callback function, and removes them from the queue
void eq_dispatch (event_queue *);
//Returns a pointer to the next queued event that will fire
event * eq_get_next_event (event_queue *);
// Find out how long we have to wait for the next event.
unsigned int eq_get_wait_time (event_queue *);
