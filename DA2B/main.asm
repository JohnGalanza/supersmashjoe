; DA2B_A.asm
.include<m328pdef.inc>

	.ORG 0
	JMP MAIN
	.ORG 0x02
	JMP EX0_ISR

MAIN:
	SBI DDRB,2				;Port B2 an output
	SBI PORTB,2				;Turn led off
	SBI PORTD,2				;activate pull up 
	LDI R20, 0x2			;Int0 falling edge
	STS EICRA,R20

	LDI R20, HIGH(RAMEND)	;setup stack
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	OUT SPL, R20

	LDI R20, 1<<INT0		
	OUT EIMSK, R20
	SEI						;checks INT0 if changed
	RJMP MAIN				;Repeats 

EX0_ISR:
	CBI PORTB, 2			;turn led on
    LDI R16,102				;Start of delay for 1.25s		
    LDI R17,118	
    LDI R18,194
L2: DEC R18
    BRNE L2
    DEC R17
    BRNE L2
    DEC R16
    BRNE L2
	RETI 
