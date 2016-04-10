/*
  adc.c - libAPL ADC system.  ADC data is gathered on a non-blocking interrupt cycle.  A map of channels is used
  to hold sample data along with channel address, availability, and other settings.
  
  TODO:
  Figure out how to compensate...
  Test it!
*/
#include <stdlib.h>
#ifdef __AVR__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "adc.h"

/*
These macros shouldn't be used in the outside world, so we only define them here.
Replacing macros with inline functions.
*/
// Sets MUXA / MUXB pins to correct bank
#define ADC_SETBANK(n) (ADC_MUX_PORT = (ADC_MUX_PORT & ~ADC_BANK_MASK) | ((n) & ADC_BANK_MASK))
static inline void ADC_setbank (ADCcflags_t f) {
	ADC_MUX_PORT = (ADC_MUX_PORT & ~ADC_BANK_MASK) | ((f) & ADC_BANK_MASK);
}

// Set MUX3:0 to the new channel using lower 3 bits
#define ADC_SETCHANNEL(n) (ADMUX = (ADMUX & ~ADC_CHAN_MASK) | ((n) & ADC_CHAN_MASK))
static inline void ADC_setchannel (ADCcflags_t f) {
	ADMUX = (ADMUX & ~ADC_CHAN_MASK) | ((f) & ADC_CHAN_MASK);
}

// Set and clear the MUX5 bit in ADCSRB
#define ADC_SETMUX() (ADCSRB |= _BV(MUX5))
static inline void ADC_setmux (void) {ADCSRB |= _BV(MUX5);}
#define ADC_CLRMUX() (ADCSRB &= ~_BV(MUX5))
static inline void ADC_clrmux (void) {ADCSRB &= ~_BV(MUX5);}

// Start a conversion
#define ADC_START() (ADCSRA |= _BV(ADSC))
static inline void ADC_start (void) {ADCSRA |= _BV(ADSC);}

// Assign a configured channel to be read
static inline void ADC_assign (ADCchannel_t * c) {
	// Now we see if we are using low (0..7) or high (8..15) channels
	if (c->flags & ADC_MUX_MASK) {
		ADC_setmux(); // Set MUX5 bit for high channels
		ADC_setbank(c->flags);  // Set external mux bits from map
	}
	else {
		ADC_clrmux(); // Clear MUX5 for low channels
		ADC_setbank(0); // External muxing doesn't matter for these channels, but just to be safe...
	}
	ADC_setchannel(c->flags);
}

/*
  Special Notes:
  The Kitchen Sink GCU uses a multiplexing system on the upper 8 ADC channels.  Two bits, MUXA and MUXB,
  select the bank we are currently reading from.  This changes all 8 inputs at the same time.  There are
  16 temperature and 6 pressure inputs, plus the other 8 non-muxed inputs, for a total of 30 inputs.
  
  There's also an internal multiplexing bit (MUX5) which separates channels 0..7 from 8..15
*/

ADCchannel_t * ADC_channels = NULL; // This is the current channel array

volatile int cx;  // This is the currently sampled channel

/*
  This interrupt is triggered when a conversion completes.  We store the result, advance to the next channel,
  and begin the next conversion.  Because of the AVR's internal multiplexing and the KS GCU's external multiplexing,
  this gets a little gnarly and we have to use a map to switch the ADC to the proper channel.

  This ISR is set non-blocking, since it can't accidentally recurse.
*/
//ISR (ADC_vect, ISR_NOBLOCK) {
ISR (ADC_vect) {
	if (ADC_channels != NULL) {
		ADC_channels[cx].sample = ADCL;  // Save low byte
		ADC_channels[cx].sample |= ADCH << 8; // Save high byte
		// The two lines below should be re-written to check channel availability
		if (ADC_channels[++cx].flags == ADC_NULL_CHANNEL)  // Advance to next channel
			cx = 0; // Or start over
		ADC_assign(&ADC_channels[cx]);
	}
	ADC_start();
}

void ADC_Init() {
	DDRL |= ADC_BANK_MASK;    // MUXA and MUXB are outputs
	//PORTL = (PORTL & ~0xc0) | (0 << 6);  // bank number for external MUX 
	
	ADMUX |= _BV(REFS0);  // AVCC is AREF   
	ADCSRA |= _BV(ADEN);  // enable ADC
	ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);  // prescale by 128

	DIDR0 = 0x0f;    // disable digital inputs, ADC0 - ADC3 (ADC4..7 used for JTAG)
	DIDR2 = 0xff;    // disable digital inputs, ADC8 - ADC15
	
	// Reset input channel selection
	ADC_setchannel(0);
	ADC_clrmux();
	ADC_setbank(0);
	
	ADCSRA |= _BV(ADIE); // Turn on the ADC interrupt
	ADC_start(); // Start the first conversion
}

void ADC_Reset() {
}

/*
  This function loads an ADC channel map from program memory to the block provided by the user (we don't like calling
  malloc() aroud here).
*/
/*
void ADC_loadmap(ADCchannel_t * dest, const ADCchannel_t * src) {
	// Load channel mapping data from program memory to SRAM
	memcpy_P(dmap, smap, n);
	// Switch the map pointer to our new map
	ADC_cmap = dmap;
}
*/
/*
ADCsample_t ADC_Compensate(ADCdata_t * d) {
	return 0;
}
*/
//#define COMPENSATE(x,i) (( (((signed)(x)) + (OFFSET)) * (ADC_REF_V / ADC_MAXCOUNT) / sensitivity[i]) + offset[i])
//#define COMPENSATE(x) ((((x) * (ADC_REF_V / (GAIN * SENSITIVITY))) / ADC_MAXCOUNT) + COLDTEMP)

/*
Return the number of configured ADC channels
*/
unsigned int ADC_Channels(ADCchannel_t channels[]) {
	unsigned int c = 0;
	if (ADC_channels != NULL) {
		while (ADC_channels[c].flags != ADC_NULL_CHANNEL) c++;
	}
	return c;
}

/*
  Return the last sampled data for a channel
*/
ADCsample_t ADC_Read(ADCcnum_t chan) {
	if (chan < ADC_Channels(ADC_channels)) {
		return ADC_channels[chan].sample;
	}
	return ADC_NULL_SAMPLE;
}
#endif // __AVR__

