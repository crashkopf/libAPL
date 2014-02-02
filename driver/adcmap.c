/* 
adcmap.c - ADC channel maps, for your sampling pleasure 
Requires adc.h

*/
#include <stdlib.h>
#ifdef __AVR__
#include <avr/pgmspace.h>
#include "driver/adc.h"
#include "driver/adcmap.h"

// These macros make the bitmapping easier
#define CHAN(c) ((c << ADC_CHAN_ROT) & ADC_CHAN_MASK)
#define MUX(m) ((m << ADC_MUX_ROT) & ADC_MUX_MASK)
#define BANK(b) ((b << ADC_BANK_ROT) & ADC_BANK_MASK)
#define AVAIL(a) ((a << ADC_AVAIL_ROT) & ADC_AVAIL_MASK)
// This macro makes a channel bitmap for externally multiplexed channels
#define ADCFLAGS(c, b, a) (CHAN(c) | MUX(1) | BANK(b) | AVAIL(a))

#define NO_SAMPLE 0
#define NO_SENS 1	// Only make dollas
#define NO_GAIN 1	// No pain!
#define NO_OFF 0

#define P_HISENS 286
#define P_LOSENS 1000
#define P_GAIN NO_GAIN
#define P_OFF -512

#define T_SENS 41
#define T_GAIN 100
#define T_OFF 25

#define EZ_CHAN(c) {NO_SAMPLE, NO_SENS, NO_OFF, NO_GAIN, c}
#define T_DEFAULTS(c, b, a) {NO_SAMPLE, T_SENS, T_OFF, T_GAIN, ADCFLAGS(c, b, a)}
#define P_DEFAULTS_LO(c, b, a) {NO_SAMPLE, P_LOSENS, P_OFF, P_GAIN, ADCFLAGS(c, b, a)}
#define P_DEFAULTS_HI(c, b, a) {NO_SAMPLE, P_HISENS, P_OFF, P_GAIN, ADCFLAGS(c, b, a)}


//Sample, sens, offset, gain, flags
const ADCchannel_t ADCcmap_GCU_v303_full[] PROGMEM = {
	// Low channels are easy
	EZ_CHAN(0),
	EZ_CHAN(1),
	EZ_CHAN(2),
	EZ_CHAN(3),
	EZ_CHAN(4),
	EZ_CHAN(5),
	EZ_CHAN(6),
	EZ_CHAN(7),
	/* High channels require the ADCMUX and bank bits set */
	/* These channels are for thermocouples */
	T_DEFAULTS(0, 0, 1),
	T_DEFAULTS(0, 1, 1),
	T_DEFAULTS(0, 2, 1),
	T_DEFAULTS(0, 3, 1),
	T_DEFAULTS(1, 0, 1),
	T_DEFAULTS(1, 1, 1),
	T_DEFAULTS(1, 2, 1),
	T_DEFAULTS(1, 3, 1),
	T_DEFAULTS(2, 0, 1),
	T_DEFAULTS(2, 1, 1),
	T_DEFAULTS(2, 2, 1),
	T_DEFAULTS(2, 3, 1),
	T_DEFAULTS(3, 0, 1),
	T_DEFAULTS(3, 1, 1),
	T_DEFAULTS(3, 2, 1),
	T_DEFAULTS(3, 3, 1),
	/* These channels are for pressure sensors */
	P_DEFAULTS_HI(4, 0, 1),
	P_DEFAULTS_HI(4, 1, 1),
	P_DEFAULTS_HI(4, 2, 1),
	P_DEFAULTS_HI(4, 3, 1),
	P_DEFAULTS_LO(5, 0, 1),
	P_DEFAULTS_LO(5, 1, 1)
};
void load_v303 () {
	ADC_channels = malloc(sizeof(ADCcmap_GCU_v303_full));
	memcpy_P(ADC_channels, &ADCcmap_GCU_v303_full, sizeof(ADCcmap_GCU_v303_full));
}
#endif //__AVR__

