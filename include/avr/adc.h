#ifdef __AVR__
#include <avr/io.h>
#endif

#include <stdint.h>

// 10 bits
#define ADC_MAXCOUNT (1024l)

// in microvolts
#define ADC_VREF (5000000l)

#define ADC_MUXA PL7
#define ADC_MUXB PL6
#define ADC_MUX_PORT PORTL
#define ADC_MUX_DDR DDRL

// Shift level of bit flags in channel map
#define ADC_CHAN_ROT 0
#define ADC_MUX_ROT 3
#define ADC_BANK_ROT 6 // Use the same bits as the port register for less work
#define ADC_AVAIL_ROT 5

// Use shift level defined above so we can change easily
#define ADC_CHAN_MASK (7 << ADC_CHAN_ROT)	// 00000111
#define ADC_MUX_MASK (1 << ADC_MUX_ROT)		// 00001000
#define ADC_BANK_MASK (3 << ADC_BANK_ROT)	// 11000000
#define ADC_AVAIL_MASK (1 << ADC_AVAIL_ROT)	// 00010000

#define ADC_NULL_CHANNEL (~0)

#define ADC_NULL_SAMPLE (~0)

typedef unsigned int ADCcnum_t;
typedef uint16_t ADCsample_t;
typedef unsigned int ADCsens_t;
typedef int ADCoffset_t;
typedef int ADCgain_t;
typedef uint16_t ADCcflags_t;

typedef struct {
	ADCsample_t sample;
	ADCsens_t sens;
	ADCoffset_t offset;
	ADCgain_t gain;
	ADCcflags_t flags;
	PGM_P name;
} ADCchannel_t;

// ADC channel as opaque structure
typedef struct adc_s adc_s;

extern adc_s ADC0;
extern adc_s ADC1;
extern adc_s ADC2;
extern adc_s ADC3;
extern adc_s ADC4;
extern adc_s ADC5;
extern adc_s ADC6;
extern adc_s ADC7;

/* 
Since the ADC channel configurations on the GCU can vary and have fancy multiplexing we have to make a map 
The mapping structure may seem a little tedious, but it allows for a lot of flexibility.  It also keeps
the settings in one place.
*/
/*
typedef struct {
	unsigned int len;
	ADCdata_t channel[];
} ADCcmap_t;
*/

// Prototype as C for weenies that want to use C++
#ifdef __cplusplus
extern "C" {
#endif

// This points to the current ADC channel map and is more or less global
// NULL-terminated channel array is the new hotness
//extern ADCcmap_t * ADC_cmap;
extern ADCchannel_t * ADC_channels;

void ADC_Init();
void ADC_Reset();
//void ADC_loadmap(void *, const ADCchannel_t *);
ADCsample_t ADC_Read(ADCcnum_t chan);
unsigned int ADC_Channels();

// Future ADC API
uint16_t adc_read(adc_s *);

#ifdef __cplusplus
}
#endif
