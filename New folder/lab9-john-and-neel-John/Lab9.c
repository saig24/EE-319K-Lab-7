// Lab9.c
// Runs on LM4F120 or TM4C123
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 4/10/2017 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// UART1 on PC4-5
// * Start with where you left off in Lab8. 
// * Get Lab8 code working in this project.
// * Understand what parts of your main have to move into the UART1_Handler ISR
// * Rewrite the SysTickHandler
// * Implement the s/w FiFo on the receiver end 
//    (we suggest implementing and testing this first)


//neeltest
#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "FiFo.h"
extern char FiFo[8];
//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t Data;      // 12-bit ADC
uint32_t Position;  // 32-bit fixed-point 0.001 cm
uint32_t TxCounter = 0;	// debugging counter- measures the number of ADC samples (out)


//------Function Declarations-------
void SysTick_Init(void);
void SysTick_Handler(void);
void PortF_Init(void);
uint32_t Convert(uint32_t input);

//--------Main to test Transmitter-----
int main(void){
	char user;
	char *data=&user;
	char output[8];
	DisableInterrupts();
	TExaS_Init();
	ADC_Init();
	PortF_Init();
	UART_Init();
	EnableInterrupts();
	
	while (1){
		while(FiFo_Get(data)==1){
			if(*data==0x02){
				for(int i=0;i<8;i++){
					FiFo_Get(data);
					output[i]=*data;
				}
				ST7735_SetCursor(0,0);							
		for(int i = 1; i<6; i++){						
			ST7735_OutChar(output[i]);
		}
				
			}
		}};
	
}




// final main program for bidirectional communication
// Sender sends using SysTick Interrupt
// Receiver receives using RX
int main1(void){ 
  
  TExaS_Init();       // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB);
  ADC_Init();    // initialize to sample ADC
  PortF_Init();
  UART_Init();       // initialize UART
	ST7735_SetCursor(0,0);
  LCD_OutFix(0);
  ST7735_OutString(" cm");
	SysTick_Init();
  EnableInterrupts();
  
	while(1){
		//--UUU--Complete this  - see lab manual
	}
}



//SysTick Functions

void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;											//disables systick during setup
	NVIC_ST_RELOAD_R = (2000000);				//reload register = samplerate*f(bus)
	NVIC_ST_CURRENT_R = 0;									//clear current
	NVIC_SYS_PRI3_R &= 0x00FFFFFF;			
	NVIC_SYS_PRI3_R |= 0x30000000;					//set priority to 3
	NVIC_ST_CTRL_R = 0x07;									//set bits 0, 1, 2
}

void SysTick_Handler(void){ 
	int sample, position, i, n = 3;
	char Start = 0x02, Period = 0x2E, r = 0x0D, End = 0x03;
	char Digits[n+1];
	
	//--This takes the ADC sample and converts it----
	GPIO_PORTF_DATA_R |= 0x01;
	sample = ADC_In();
	GPIO_PORTF_DATA_R &= 0x00;
	position = Convert(sample);
	
	//--This strips the position to be sent in reverse order--
	for (i = 0; i < n; i ++){
		Digits[i] = (position % 10);
		Digits[i] = (Digits[i] + 0x30);
		position /= 10;
	}
	
	//---This sends the position to the UART with starts and stops
	UART_OutChar(Start);
	UART_OutChar(Digits[n]);
	UART_OutChar(Period);
	UART_OutChar(Digits[n-1]);
	UART_OutChar(Digits[n-2]);
	UART_OutChar(Digits[n-3]);
	UART_OutChar(r);
	UART_OutChar(End);
	
	//--Increments TxCounter for debugging
	TxCounter++;
	
	GPIO_PORTF_DATA_R |= 0x01;
	
}





// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020;
	delay = SYSCTL_RCGCGPIO_R ;
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   //  unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
	GPIO_PORTF_DIR_R |= 0x07;					//Set PF0-2 to be outputs
	GPIO_PORTF_AFSEL_R = 0x00;				// Turn off Alt Func.
	GPIO_PORTF_DEN_R |= 0x07;					//Set PF2 to Digital
	
}





//Convert ADC Data into a position
uint32_t Convert(uint32_t input){
	
	Position = (36225*input-146360000)/1000;
	
	return (Position);		 
}



uint32_t Status[20];             // entries 0,7,12,19 should be false, others true
char GetData[10];  // entries 1 2 3 4 5 6 7 8 should be 1 2 3 4 5 6 7 8
int mainfifo(void){ // Make this main to test FiFo
  FiFo_Init(); // Assuming a buffer of size 6
  for(;;){
    Status[0]  = FiFo_Get(&GetData[0]);  // should fail,    empty
    Status[1]  = FiFo_Put(1);            // should succeed, 1 
    Status[2]  = FiFo_Put(2);            // should succeed, 1 2
    Status[3]  = FiFo_Put(3);            // should succeed, 1 2 3
    Status[4]  = FiFo_Put(4);            // should succeed, 1 2 3 4
    Status[5]  = FiFo_Put(5);            // should succeed, 1 2 3 4 5
    Status[6]  = FiFo_Put(6);            // should succeed, 1 2 3 4 5 6
    Status[7]  = FiFo_Put(7);            // should fail,    1 2 3 4 5 6 
    Status[8]  = FiFo_Get(&GetData[1]);  // should succeed, 2 3 4 5 6
    Status[9]  = FiFo_Get(&GetData[2]);  // should succeed, 3 4 5 6
    Status[10] = FiFo_Put(7);            // should succeed, 3 4 5 6 7
    Status[11] = FiFo_Put(8);            // should succeed, 3 4 5 6 7 8
    Status[12] = FiFo_Put(9);            // should fail,    3 4 5 6 7 8 
    Status[13] = FiFo_Get(&GetData[3]);  // should succeed, 4 5 6 7 8
    Status[14] = FiFo_Get(&GetData[4]);  // should succeed, 5 6 7 8
    Status[15] = FiFo_Get(&GetData[5]);  // should succeed, 6 7 8
    Status[16] = FiFo_Get(&GetData[6]);  // should succeed, 7 8
    Status[17] = FiFo_Get(&GetData[7]);  // should succeed, 8
    Status[18] = FiFo_Get(&GetData[8]);  // should succeed, empty
    Status[19] = FiFo_Get(&GetData[9]);  // should fail,    empty
  }
}
