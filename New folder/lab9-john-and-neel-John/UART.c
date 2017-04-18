// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// November 17, 2014
// Modified by EE345L students Charlie Gough && Matt Hawk
// Modified by EE345M students Agustinus Darmawan && Mingjie Qiu

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "FiFo.h"
#include "UART.h"
#include "tm4c123gh6pm.h"
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t RxCounter,RXmail,RXstatus = 0;	// debugging counter- measures number of converted samples (in)

uint32_t DataLost; 
// Initialize UART1
// Baud rate is 115200 bits/sec
// Make sure to turn ON UART1 Receiver Interrupt (Interrupt 6 in NVIC)
// Write UART1_Handler
// Also initializes PC4/PC5

void UART_Init(void){
 
	SYSCTL_RCGCUART_R |= 0x0002;		//activates UART1
	SYSCTL_RCGCGPIO_R |= 0x0004; 		//Activate Port C
	UART1_CTL_R &= ~0x0001;					//Disable UART
	UART1_IBRD_R = 43;							//IBRD = int(80000000)/(16*115200) = 43.40277
	UART1_FBRD_R = 26;							//FBRD = round(0.4028*64)=26
	UART1_LCRH_R = 0x0070;					//8 bit word length-enable FIFO
	UART1_CTL_R = 0x301;						//enable RXE, TXE, and UART
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFFFFFF00)+ 0x00000011; // UART
	GPIO_PORTC_AMSEL_R  &= ~0x30;		//Disable analog function on PC4, PC5
	GPIO_PORTC_AFSEL_R |=0x30;			//Enable alternate function on PC4, PC5
	GPIO_PORTC_DEN_R |= 0x30;				//Enable digital function on PC4, PC5
	
}




//------------UART1_InChar------------
// Input: none
// Output: ASCII character from UART
// spins if RxFifo is empty

char UART_InChar(void){
	while ((UART1_FR_R & 0x0010) !=0);
  return ((char)(UART1_DR_R & 0xFF));
}






//------------UART1_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none

void UART_OutChar(char data){
  while ((UART1_FR_R & 0x0020) != 0);		//waits until TXFF is 0
	UART1_DR_R = data;
}







// hardware RX FIFO goes from 7 to 8 or more items
// UART receiver Interrupt is triggered; This is the ISR

void UART1_Handler(void){
  // --UUU-- complete with your code
	PF2^=0X04;
	for(int i=0;i<8;i++){
	while((UART1_FR_R&0x0010)!=0){}
		RXmail=UART_InChar();
		FiFo_Put(RXmail);
	}

	RxCounter++;
	UART1_ICR_R = 0x10;
	PF2^=0X04;
}
