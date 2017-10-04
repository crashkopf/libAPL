#ifndef TIMER_H
#define TIMER_H

#define xstringify(x) #x
#define stringify(x) xstringify(x)
#define pathinclude(path, file) stringify(path/file)

#ifdef _TARGET
#define inc stringify(_TARGET/timer.h)
#include inc
#undef inc
#endif
#ifdef _CPU
#define inc stringify(_CPU/timer.h)
#include inc
#undef inc
#endif
#ifdef _ARCH
#define inc stringify(_ARCH/timer.h)
#include inc
#undef inc
#endif

//#include <util/atomic.h>

typedef struct timer_s {
	unsigned long count;
	unsigned direction: 1;	// 1 for up, 0 for down
	unsigned overflow: 1;	// 1 for okay to over/under flow the counter
	struct timer_s * next;
} timer_s;

#ifdef __cplusplus
extern "C" {
#endif

void timer_init();
void timer_start(timer_s *);
void timer_stop(timer_s *);
void timer_set(timer_s *, unsigned long);
unsigned long timer_read(timer_s *);

#ifdef __cplusplus
}
#endif

#endif	// TIMER_H