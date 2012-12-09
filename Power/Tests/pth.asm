	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions

;write the config bits
    __CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _LVP_OFF & _CPD_OFF 

  org 0x0000
	goto Start
	


Start:
;enable output on the PON lines
  banksel ADCON1
  bcf TRISD, 1
  bcf TRISD, 4
  bcf TRISC, 5
  banksel ADCON0
  bcf PORTD, 1
  bcf PORTD, 4
  bcf PORTC, 5 
  
  call DelayS
  
  bsf PORTD, 1
  bsf PORTD, 4
  bsf PORTC, 5 
Loop:
  nop
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


  end
