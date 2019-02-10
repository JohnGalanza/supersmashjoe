.include <m328pdef.inc> 
.org 0
	LDI R25, 0x00 ; first 8 bits of multiplicand
	LDI R24, 0x90 ; last 8 bits of multiplicand
	LDI R22, 0x90 ; 8 bit multiplier 

multiply:
	cp R22, R0 ; check point, compares register 22 to register 0 
	BREQ end ; will branch to end if r22 is equal to 0 

	add R18, R24 ; if r22 isnt equal to 0 then it will add r18 and r24
	adc R19, R25 ; if r18 and r24 have a carry it will add it here
	adc R20, R0 ; if r19 and r25 have a carry then it will add it here

	dec R22 ; this will decrement r22 will determine how many times we multiply
	jmp multiply ; restarts loop and check point 

end: jmp end  