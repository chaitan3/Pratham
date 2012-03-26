	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions

;write the config bits
    __CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _LVP_OFF & _CPD_OFF 
    
cblock 0x20
	Channel
	Count
	InverseCount
	datasend
	endc


	org 0x0000
	goto Start
	


Start:
	banksel ADCON1
	
	;configure ADC
	clrf ADCON1

	;set ADC ports as input	
	movlw 0xFF
	movwf TRISA
	
	bsf TRISC, 7
	bcf TRISC, 6
	movlw 0x19
	movwf SPBRG			 ;9600 baud
	clrf TXSTA
	bsf TXSTA, TXEN		 ;enable transmitter
	
	banksel ADCON0
	clrf RCSTA
	bsf RCSTA, SPEN		 ;enable UART on the pins
	
Stall:
	clrf Count
	movlw 0x05
	movwf InverseCount
Loop:

	movf Count, W
	movwf Channel
	bcf STATUS, C
	
	rlf Channel,f
	rlf Channel,f
	rlf Channel,f
	
	movf Channel, W
	iorlw 0x81
	movwf ADCON0
	
	movf ADCON0, W
	movwf Channel
		
	bsf ADCON0, GO
ADCWait:
	btfsc ADCON0,GO
	goto ADCWait
	
	movf ADRESH, W
	movwf datasend

	call SendData
	
	movf Channel, W
	movwf datasend
	call SendData
	
	incf Count, f
	decfsz InverseCount, f
	goto Loop
	goto Stall
	
SendData:
	nop
TXwait:
	btfss PIR1, TXIF
	goto TXwait
	
	;store the result in the location 0x21+inversecount
	movf datasend, W
	movwf TXREG
	return 


	end
