/*
  adcutils.h
*/

#define INCLUDE_ADCUTILS \
	V(adc)\
	V(adcdump)\
	V(adcmon)

int adc (int argc, char * argv[], environment * env);
int adcdump (int argc, char * argv[], environment * env);
int adcmon (int argc, char * argv[], environment * env);

static const char adc_desc [] PROGMEM = "Analog to Digital Converter utility"; 
static const char adcdump_desc [] PROGMEM = "Dump all ADC channel values";
static const char adcmon_desc [] PROGMEM = "Dump all ADC channel values continuously";
