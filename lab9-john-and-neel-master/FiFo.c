// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 4/10/2017 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
// --UUU-- Declare state variables for FiFo
//        size, buffer, put and get indexes
	uint32_t Iput=0;
	void DisableInterrupts(void);
	void EnableInterrupts(void);
	uint32_t Iget=0;
	uint32_t count=0;
	char FiFo[8];
// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init() {
// --UUU-- Complete this
	Iput=0;
	Iget=0;

}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(char data) {
	// --UUU-- Complete this routine
	if(count==8){
		return(0);
	}
	else{
		FiFo[Iput]=data;
		Iput=(Iput+1)%8;
		DisableInterrupts();
		count++;
		EnableInterrupts();
	return(1);
	}
		
		
	}
		
	
// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get(char *datapt)
{
	//--UUU-- Complete this routine
	
if(count==0){
	return 0;
}
else{
*datapt=FiFo[Iget];
Iget=(Iget+1)%8;
DisableInterrupts();
count--;
EnableInterrupts();
return(1);
}
 
}



