// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Student names: Neel Kattandum and John Sigmon
// Last modification date: 4/17/17

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

	Iput=0;
	Iget=0;

}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(char data) {

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
uint32_t FiFo_Get(char *datapt){
	
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
