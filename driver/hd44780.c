/*
  hd44780.c - LCD controller driver
*/
#ifdef __AVR__
#include <avr/io.h>
#include <util/delay_basic.h>
#include "driver/hd44780.h"

#define DATA_PORT PORTA
#define DATA_DDR DDRA
#define DATA_MASK 0x0F
#define CTRL_PORT PORTG
#define CTRL_DDR DDRG
#define CTRL_MASK 0x07
#define CTRL_PIN_E 0x01
#define CTRL_PIN_RW 0x02
#define CTRL_PIN_RS 0x04

static inline void input(void) {
	DATA_DDR |= DATA_MASK;
}

static inline void output(void) {
	DATA_DDR &= ~DATA_MASK;
} 

static inline void write (char c) {
	output();  // Set data direction to output
	CTRL_PORT &= ~CTRL_MASK;  // Clear all control pins
	CTRL_PORT |= CTRL_PIN_E;  // Clock high
	DATA_PORT = (DATA_PORT & ~DATA_MASK) | ((c >> 4) & 0xf0); // Assert hi nibble
	CTRL_PORT &= ~CTRL_PIN_E;  // Clock low
	// Wait?
	CTRL_PORT |= CTRL_PIN_E;  // Clock high
	DATA_PORT = (DATA_PORT & ~DATA_MASK) | (c & 0xf0); // Assert lo nibble
	CTRL_PORT &= ~CTRL_PIN_E;  // Clock low
	// Wait?
}

#define DelayMS(n) _delay_loop_2(n * 100)


#define DISP_IN      {PORTA |= 0x0f; DDRA &= ~0x0f;}
#define DISP_OUT     {DDRA |= 0x0f; }
#define DISP_EN_HIGH {PORTG |= 0x01;}
#define DISP_EN_LOW  {PORTG &= ~0x01;}
#define DISP_READ    {PORTG |= 0x02;}
#define DISP_WRITE   {PORTG &= ~0x02;}
#define DISP_IR      {PORTG &= ~0x04;}
#define DISP_DR      {PORTG |= 0x04;}

#define HIGHNYBBLE(data) ((data & 0xf0) >> 4)
#define LOWNYBBLE(data)  ((data & 0x0f))

#define DISP_DATA_OUT(data) {PORTA = (PORTA & ~0x0f) | (data);}
#define DISP_DATA_IN    (PINA & 0x0f)

void Disp_Init() {
	DATA_PORT &= ~DATA_MASK;
	DATA_DDR |= DATA_MASK;
	CTRL_PORT &= ~CTRL_MASK;
	CTRL_DDR |= CTRL_MASK;
}


unsigned char Disp_ReadSR() {
	unsigned char highnib, lownib;

	DISP_IN;
	PORTG &= ~7;
	DISP_IR;
	DISP_READ;

	DISP_EN_HIGH; // read IR en
	highnib = DISP_DATA_IN;
	DISP_EN_LOW;

	DISP_EN_HIGH;
	lownib = DISP_DATA_IN;
	DISP_EN_LOW;

	return (highnib << 4) + lownib;
}


void Disp_WriteIR(unsigned char d) {
	unsigned char highnib, lownib;
	highnib = HIGHNYBBLE(d);
	lownib = LOWNYBBLE(d);
	DISP_OUT;
	PORTG &= ~7;

	DISP_EN_HIGH;
	DISP_DATA_OUT(highnib);
	DISP_EN_LOW;

	DISP_EN_HIGH;
	DISP_DATA_OUT(lownib);
	DISP_EN_LOW;
}


unsigned char Disp_ReadDR() {
	unsigned char highnib, lownib;

	DISP_IN;

	DISP_READ;
	DISP_DR;

	DISP_EN_HIGH;
	highnib = DISP_DATA_IN;
	DISP_EN_LOW;

	DISP_EN_HIGH;
	lownib = DISP_DATA_IN;
	DISP_EN_LOW;

	return (highnib << 4) | lownib;
}


void Disp_WriteDR(unsigned char d) {
	unsigned char highnib, lownib;
	highnib = HIGHNYBBLE(d);
	lownib = LOWNYBBLE(d);
	DISP_OUT;
	PORTG = (PORTG & ~7) | 4; // data reg
	DISP_EN_HIGH; // strobe
	DISP_DATA_OUT(highnib);
	DISP_EN_LOW;

	DISP_EN_HIGH;
	DISP_DATA_OUT(lownib);
	DISP_EN_LOW;
}


void Disp_Reset() {
	DelayMS(15);
	DISP_OUT;

	PORTG = (PORTG & ~7);
	DISP_DATA_OUT(0x03);  // 8 bit interface
	DISP_EN_HIGH;
	DISP_DATA_OUT(0x03);  // 8 bit interface
	DISP_EN_LOW;

	DelayMS(5);

	DISP_EN_HIGH;
	DISP_DATA_OUT(0x03);  // 8 bit interface
	DISP_EN_LOW;

	DelayMS(5);

	DISP_EN_HIGH;
	DISP_DATA_OUT(0x03);   // 8 bit interface
	DISP_EN_LOW;

	DelayMS(5);

	DISP_DATA_OUT(0x02);
	DISP_EN_HIGH;
	DISP_DATA_OUT(0x02);   // set to 4 bit interface
	DISP_EN_LOW;

	DelayMS(20);

	Disp_WriteIR(0x28);    // 4 bit interface, 2 lines, 5x8
	DelayMS(1);

	Disp_WriteIR(0x08);    // display off
	DelayMS(1);

	Disp_WriteIR(0x01);    // display clear
	DelayMS(20);

	Disp_WriteIR(0x0c);
	DelayMS(1);			// display on

	Disp_WriteIR(0x06);    // Entry mode set, increment, no shift
	DelayMS(1);

}



void Disp_Wait() {
//	while (Disp_ReadSR() & 0x01) ;
//delay(1);
DelayMS(75);
}



void Disp_RC(unsigned char row, unsigned char col) {
	unsigned char addr = 0;

	switch (row) {
	case 0: 
		addr = col;
		break;
	case 1:
		addr = 0x40 + col;
		break;
	case 2:
		addr = 20 + col;
		break;
	case 3:
		addr = 0x40 + 20 + col;
		break;
	}

	Disp_WriteIR(0x80 + addr);
	Disp_Wait();
}
		


void Disp_PutChar(char c) {
	Disp_WriteDR(c);
	Disp_Wait();
}



void Disp_PutStr(char *p) {
	while (*p) {
		Disp_PutChar(*p++);
	}
}



void Disp_CursOn() {
	Disp_WriteIR(0x0d);  // display on, blinking cursor
	Disp_Wait();
}



void Disp_CursOff() {
	Disp_WriteIR(0x0c);  // display on, cursor off
	Disp_Wait();
}
#endif //__AVR__
