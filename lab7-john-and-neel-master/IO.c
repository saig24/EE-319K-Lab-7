// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on LM4F120 or TM4C123
// Program written by: John Sigmon and Neel Kattumadam
// Date Created: 
// Last Modified:  
// Lab number: 6


#include "tm4c123gh6pm.h"
#include <stdint.h>

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) {
 volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020;
	delay = SYSCTL_RCGCGPIO_R ;
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   //  unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
	GPIO_PORTF_DIR_R |= 0x00000004;		//Set PF2 to be output
	GPIO_PORTF_DIR_R &= 0xFFFFFF0F;		//Clear PF4 to be input
	GPIO_PORTF_AFSEL_R = 0x00;				// Turn off Alt Func.
	GPIO_PORTF_DEN_R |= 0x14;					//Set PF2 and PF4 to Digital
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
// PF2 is used for a heartbeat

void IO_HeartBeat(void) {
	GPIO_PORTF_DATA_R ^= 0x04;
	}

	void Wait10ms(void){
		uint32_t delay=31400;
		while(delay>0){
		delay--;	
		}
		
	}



//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
 // --UUU-- wait for release; delay for 20ms; and then wait for press
	while((GPIO_PORTF_DATA_R|=0x10)!=0) {	
	
	}
	
Wait10ms();
	
		while((GPIO_PORTF_DATA_R|=0x10)==0) {
	
	}
	
	
}  

