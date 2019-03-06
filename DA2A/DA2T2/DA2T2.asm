;
; DA2T2.asm
;

	CBI DDRC,2		;makes PC2 an input
	LDI R19, 0x04
	OUT DDRB, R19	;makes PB2 an output port
L1:	SBIC PINC,2		;Skip if bit PB2 is LOW
	RJMP L1			;checks if bit is 0
	SBI PORTB,2		;turns on port b 
	CALL Delay		;delays for 1.250s
	CBI PORTB,2		;clears port b
	RJMP HERE

	Delay:
	LDI R16,102
	LDI R17,118
	LDI R18,194
L2: DEC R18
	BRNE L2
	DEC R17
	BRNE L2
	DEC R16
	BRNE L2

HERE:RJMP HERE