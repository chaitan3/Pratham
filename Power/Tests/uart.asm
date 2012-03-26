	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions

;write the config bits
    __CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _LVP_OFF & _CPD_OFF 
    
	org 0x0000
	goto Start
	
Start:
	banksel ADCON1
	bsf TRISC, 7
	bcf TRISC, 6
	movlw 0x19
	movwf SPBRG			 ;9600 baud
	clrf TXSTA
	bsf TXSTA, TXEN		 ;enable transmitter
	
	banksel ADCON0
	clrf RCSTA
	bsf RCSTA, SPEN		 ;enable UART on the pins
	
	movlw 0x22
	movwf 0x70
	
Stall:
	
TXwait:
	btfss PIR1, TXIF
	goto TXwait
	incf 0x70, f
	movf 0x70, W
	movwf TXREG
	goto Stall
	end
