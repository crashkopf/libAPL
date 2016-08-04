

// Pulse width modulator, in software
typedef struct {
	unsigned int period;  // Period in timer ticks
	unsigned int duty;  // high cycle length in timer ticks
} PWM;

#ifdef __cplusplus
extern "C" {
#endif

void Servo_Init() ;
void Servo_Write();

#ifdef __cplusplus
}
#endif
