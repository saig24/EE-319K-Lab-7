; Print.s
; Student names: John Sigmon and Neel Kattumadam
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 
MAX EQU 10000
STAR EQU 0x2A
DOT EQU 0x2E
    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
	PRESERVE8
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec

CHECK
	PUSH{R14}
	MOV R3,#10;
 	UDIV R1,R0,R3;		;divide R0(input) by 10
	MUL R2,R1,R3
	SUBS R2,R0,R2		;calculate remainder of r0/10
	ADD R2,R2,#0X30;	; add x30 to calculate ascii value of integer
	PUSH{R2}
	ADDS R1,#0;			; check if quotient of division is 0, exit if that's the case
	BNE CHECK
	
	
	
	
	
finish 
	POP{R0}		; POP all the registers containing ascii values
	CMP R0,#0
	BEQ DONE
	BL ST7735_OutChar		; branch to output method
	B finish


	


DONE
	POP {R14}
    BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix ;Checks to see if input is greater than max
	PUSH{R14}
	LDR R1,=MAX
	SUBS R2,R0,R1
	BMI LARGE
	MOV R12, #4
EVAL	; calculate the remainder, and use the quotient to start each iteration
	MOV R3,#10;
 	UDIV R1,R0,R3;		;divide R0(input) by 10
	MUL R2,R1,R3
	SUBS R2,R0,R2
	ADD R2,R2,#0X30;	
	PUSH{R2}
	ADD R0,R1,#0
	SUBS R12,#1
	BNE EVAL
	
NORMAL ; output value, .,value,value,value
	POP{R0} 
	BL ST7735_OutChar
	LDR R0,=DOT
	BL ST7735_OutChar
	POP{R0}
	BL ST7735_OutChar
	POP{R0}
	BL ST7735_OutChar
	POP{R0}
	BL ST7735_OutChar
	POP{R14}
	BX LR
	
	
	
LARGE; if the original input was too large output this default string *.***
	LDR R0,=STAR
	BL ST7735_OutChar
	LDR R0,=DOT
	BL ST7735_OutChar
	LDR R0,=STAR
	BL ST7735_OutChar
	LDR R0,=STAR
	BL ST7735_OutChar
	LDR R0,=STAR
	BL ST7735_OutChar
	POP{R14}
     BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
