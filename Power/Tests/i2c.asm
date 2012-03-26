	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions

;write the config bits
    __CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _LVP_OFF & _CPD_OFF   

w_temp equ 0x70 ; variable used for context saving
status_temp equ 0x71 ; variable used for context saving
fsr_temp equ 0x72


  org 0x0000
	
	;start power code
	goto Start
	
	org     0x004             ; interrupt vector location

	movwf   w_temp            ; save off current W register contents
	movf	STATUS, W          ; move status register into W register
	movwf	status_temp       ; save off contents of STATUS register
	movf	FSR, W
	movwf	fsr_temp		;save FSR register
	
	banksel ADCON0
  bsf PORTB, 4
  
	btfsc PIR1, SSPIF
	call I2CHandler   ; I2C receive interrupt
	banksel ADCON0
	bcf PIR1, SSPIF

ExitISR: 

	movf    status_temp, W     ; retrieve copy of STATUS register
	movwf	STATUS            ; restore pre-isr STATUS register contents
	movf    fsr_temp, W     ; retrieve copy of FSR register
	movwf	FSR            ; restore pre-isr FSR register contents
	movf w_temp, W	;restore pre-isr W register
	retfie

I2CHandler:
	;Check Read/Write bit
	movf SSPBUF, W
  banksel ADCON1
	btfsc SSPSTAT, 2
    ;Send HM data if Slave in Write mode
	goto SendHMData

	;Check if last byte was Data or address
	btfsc SSPSTAT, 5
	;If it was Data call the OBC command response subroutine	
	goto Dubbo
  
  goto ExitTWI

Dubbo:
	banksel ADCON0
	movwf 0x21
  bsf PORTB, 5
  goto ExitTWI
  
SendHMData:
		btfsc SSPSTAT, 5
		goto ExitTWI

  banksel ADCON0

  movf 0x21, W
  
WriteI2C:
	;Send Data
	bcf SSPCON, WCOL
	movwf SSPBUF
	btfsc SSPCON, WCOL
	goto WriteI2C
	
	bsf SSPCON, CKP

	;Increment Counter, check if equal to 7 make it 
ExitTWI:

	return 


  
Start:
  banksel ADCON1
  ;confugure I2C
	clrf SSPSTAT
	clrf SSPCON2
	;movlw 0x01
	;movwf SSPCON2
	;configure address of I2C slave
  movlw 0x44
  movwf SSPADD
  bsf TRISC, 3
	bsf TRISC, 4
  
  bcf TRISB, 4
  bcf TRISB, 5
  
  bsf PIE1, SSPIE
  
  banksel ADCON0
  
  clrf PIR1
  
  bcf PORTB, 4
	bcf PORTB, 5

  
  movlw 0x36
	movwf SSPCON
	

  
  bsf INTCON, PEIE

  
	bsf INTCON, GIE	
Loop:
  nop
  
  goto Loop
  end
