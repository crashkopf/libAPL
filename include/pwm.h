/*
  pwm.h -- Pulse width modulation, in software
*/


typedef struct {
	unsigned int period;  // Period in timer ticks
	unsigned int duty;  // high cycle length in timer ticks
} PWM;
