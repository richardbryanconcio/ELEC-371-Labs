#include "nios2_control.h"

/* place additional #define macros here */
#ifndef _NIOS2_CONTROL_H_
#define _NIOS2_CONTROL_H_


// NIOS2_CONTOL HEADER
#define NIOS2_WRITE_STATUS(value)  (__builtin_wrctl (0, value))

#define NIOS2_READ_IENABLE()	   (__builtin_rdctl (3))

#define NIOS2_WRITE_IENABLE(value) (__builtin_wrctl (3, value))

#define NIOS2_READ_IPENDING()	   (__builtin_rdctl (4))


#endif /* _NIOS2_CONTROL_H_ */

/*-----------------------------------------------------------------*/
// TIMER HEADER
#ifndef _TIMER_H_
#define _TIMER_H_

// TIMER 1
#define TIMER1_STATUS	((volatile unsigned int *) 0x10004020)
#define TIMER1_CONTROL	((volatile unsigned int *) 0x10004024)
#define TIMER1_START_LO	((volatile unsigned int *) 0x10004028)
#define TIMER1_START_HI	((volatile unsigned int *) 0x1000402C)
#define TIMER1_SNAP_LO	((volatile unsigned int *) 0x10004030)
#define TIMER1_SNAP_HI	((volatile unsigned int *) 0x10004034)

// TIMER 3
#define TIMER3_STATUS	((volatile unsigned int *) 0x10004060)
#define TIMER3_CONTROL	((volatile unsigned int *) 0x10004064)
#define TIMER3_START_LO	((volatile unsigned int *) 0x10004068)
#define TIMER3_START_HI	((volatile unsigned int *) 0x1000406C)
#define TIMER3_SNAP_LO	((volatile unsigned int *) 0x10004070)
#define TIMER3_SNAP_HI	((volatile unsigned int *) 0x10004074)

/* define a bit pattern reflecting the position of the timeout (TO) bit
   in the timer status register */
#define TIMER_TO_BIT 0x1

#endif /* _TIMER_H_ */

/*-----------------------------------------------------------------*/
// LEDS HEADER
#ifndef _LEDS_H_
#define _LEDS_H_

/* define pointer macro for accessing the LED interface data register */

#define LEDS ((volatile unsigned int *) 0x10000010)

#endif /* _LEDS_H_ */

/*-----------------------------------------------------------------*/
// SEVEN SEGMENT DISPLAY HEADER

#define SEVEN_SEG ((volatile unsigned int*) 0x10000020)

/*-----------------------------------------------------------------*/
// SWITCHES HEADER

#define SWITCHES ((volatile unsigned int*) 0x10000040)

/* define global program variables here */
// tracker for led pattern
int ledCount = 0;
int timer1_flag = 0;
int timer3_flag = 0;

/* place additional functions here */



/*-----------------------------------------------------------------*/

/* this routine is called from the_exception() in exception_handler.c */

void interrupt_handler(void)
{
	unsigned int ipending;

	/* read current value in ipending register */
	ipending = NIOS2_READ_IPENDING();
	/* do one or more checks for different sources using ipending value */

	// CHECK FOR TIMER3 INTERRUPT (BIT 16)
	if (ipending & 0x10000) {
		// CLEAR TIMER INTERRUPT
		*TIMER3_STATUS = 0;
		
		timer3_flag = 1;
	}
	// CHECK FOR TIMER1 INTERRUPT (BIT 14)
	if (ipending & 0x4000) {
		// CLEAR TIMER INTERRUPT
		*TIMER1_STATUS = 0;
		
		// set new led pattern
        if(ledCount == 0){
            *LEDS = 0b1100000011;
            ledCount++;
        }
        else if(ledCount == 1){
            *LEDS = 0b0110000110;
            ledCount++;
        }
        else if(ledCount == 2){
            *LEDS = 0b0011001100;
            ledCount++;
        }
        else if(ledCount == 3){
            *LEDS = 0b0001111000;
            ledCount++;
        }
        else if(ledCount == 4){
            *LEDS = 0b0000110000;
            ledCount++;
        }
		else if(ledCount == 5){
            *LEDS = 0b0001111000;
            ledCount++;
        }
		else if(ledCount == 6){
            *LEDS = 0b0011001100;
            ledCount++;
        }
		else if(ledCount == 7){
            *LEDS = 0b0110000110;
            ledCount = 0;
        }
		
		timer1_flag = 1;
	}
	
	/* do one or more checks for different sources using ipending value */

	/* remember to clear interrupt sources */
}

/*-----------------------------------------------------------------*/

void Init (void)
{
	/* initialize software variables */

	/* set up each hardware interface */
	// Set the TIMER3 to generate interrupts every 0.25 seconds
	*TIMER3_START_HI = 0x00BE;
	*TIMER3_START_LO = 0xBC20;
	*TIMER3_CONTROL = 0x7;
	// Set the TIMER1 to generate interrupts every 0.125 seconds
	*TIMER1_START_HI = 0x005F;
	*TIMER1_START_LO = 0x5E10;
	*TIMER1_CONTROL = 0x7;
	/* set up ienable */
	NIOS2_WRITE_IENABLE(0x14000);  // enable interrupts for timer3 at bit 16 and timer1 at bit 14
	/* enable global recognition of interrupts in procr. status reg. */
	NIOS2_WRITE_STATUS(0x1);
	
	// ADC initialization
    InitADC(2, 2);
}

/*-----------------------------------------------------------------*/

int main (void)
{
	Init ();	/* perform software/hardware initialization */
	
	unsigned int ADC = 0;
	unsigned int ADChigh = 0;
	unsigned int ADClow = 0;
	
	// used to determine if leftmost is true
    int ch = GetChar();
	//int ch = 0;
    
	int show_dash = 0;
	
	if(ch == '-'){
        show_dash = 1;
    }

    // welcome msg
    PrintString("\nELEC 371 Lab 4 by Conrad, Richard, Mo\n\nHexadecimal result from A/D conversion: 0x??");

	
	while (1)
	{
		// check timer 3 flag
        if(timer3_flag == 1){
            
            // reset flag
            timer3_flag = 0;
        

            ADC = ADConvert();
            ADChigh = ADC >> 4;
			ADClow = ADC & 0b1111;
            
            // delete
            PrintChar('\b');
			PrintChar('\b');
            
			PrintHexDigit(ADChigh);
			PrintHexDigit(ADClow);
			
        } else if (timer1_flag == 1) {
			// reset flag
            timer1_flag = 0;
			*SEVEN_SEG = 0;
			if (show_dash == 1) {
				if (*SWITCHES & 0b0001) {
					*SEVEN_SEG = *SEVEN_SEG + 0x40;
				}
				if (*SWITCHES & 0b0010) {
					*SEVEN_SEG = *SEVEN_SEG + 0x4000;
				}
				if (*SWITCHES & 0b0100) {
					*SEVEN_SEG = *SEVEN_SEG + 0x400000;
				}
				if (*SWITCHES & 0b1000) {
					*SEVEN_SEG = *SEVEN_SEG + 0x40000000;
				}
			}
			else {
				if (*SWITCHES & 0b0001) {
					*SEVEN_SEG = *SEVEN_SEG + 0x3F;
				}
				if (*SWITCHES & 0b0010) {
					*SEVEN_SEG = *SEVEN_SEG + 0x3F00;
				}
				if (*SWITCHES & 0b0100) {
					*SEVEN_SEG = *SEVEN_SEG + 0x3F0000;
				}
				if (*SWITCHES & 0b1000) {
					*SEVEN_SEG = *SEVEN_SEG + 0x3F000000;
				}
			}
		}
	}

	return 0;	/* never reached, but main() must return a value */
}
