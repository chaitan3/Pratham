	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions

;write the config bits
    __CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _LVP_OFF & _CPD_OFF 

  org 0x0000
	goto Start
	


Start:
;enable output on the PON lines
  banksel ADCON1
  ;uplink pins input
	bsf TRISB, 5
	bsf TRISB, 4
  ;uplink on off
  bcf TRISB, 7
  
  
  banksel ADCON0
  bcf PORTB, 7
  
  call DelayS
  
  bsf PORTB, 7
  
Loop:
  
  ;Uplink check
	btfss PORTB, 5
	goto SkipUplink
	
	;2 second delay to confirm that the interrupt is not a fake
	call DelayS
	
	;Test Interrupt pin
	btfss PORTB, 5
	goto SkipUplink
	
	;Check data line, if high Kill satellite
	btfsc PORTB, 4
	goto Kill
	
	;Low: Reset
	call Shutdown
	call DelayS
	
	goto Start

SkipUplink:
  
  
  goto Loop

DelayS:
  movlw 4
	movwf 0x62
Delay20:
	clrf 0x61
Delay19:
	clrf 0x60
Delay18:
	incfsz 0x60, f
	goto Delay18
	
	incfsz 0x61, f
	goto Delay19

	decfsz 0x62, f
	goto Delay20
  
  return

Shutdown:
  ;switch off uplink
  bcf PORTB, 7

	return


Kill:

  end

