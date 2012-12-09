	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions

;write the config bits
    __CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _LVP_OFF & _CPD_OFF 

cblock 0x20
	LoadStatus

	;0x21 stores ADC HM data
	BatteryCurrent
	BatteryVoltage
	Panel
	SixVolt
	ThreeVolt
	
    ;Overvoltage and low power of battery
    BatteryStatus
    
    
	HMCounter
	
	;variables required for adc: Don't use in interrupts
	Channel
	Count
	InverseCount

	;variables required for OC
	Timer
	Timer7
	Timer6
	Timer5
	Timer4
	Timer3

	;variables required for OBC Command
	CommandByte
	
	;Variable for i2c byte status
	TWIByte
  
  ;Variable for Ejection State: Snap/Preflight
	State
	endc

w_temp equ 0x70 ; variable used for context saving
status_temp equ 0x71 ; variable used for context saving
fsr_temp equ 0x72

I2C_ADDR equ 0x44

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
	
	btfsc PIR1, SSPIF
	call I2CHandler   ; I2C receive interrupt
	
	banksel ADCON0

	bcf PIR1, SSPIF
	bcf INTCON, INTF

ExitISR: 

	movf    status_temp, W     ; retrieve copy of STATUS register
	movwf	STATUS            ; restore pre-isr STATUS register contents
	movf    fsr_temp, W     ; retrieve copy of FSR register
	movwf	FSR            ; restore pre-isr FSR register contents
	movf w_temp, W	;restore pre-isr W register
	retfie                    ; return from interrupt	
   
I2CHandler:
	movf SSPBUF, W
	;Check Read/Write bit
    banksel ADCON1
   
	;Check if last byte was Data or address
	btfsc SSPSTAT, 5
	;If it was Data call the OBC command response subroutine	
	goto ExitI2C
	
	
SendHMData:
	;Check if Master in Read Mode
	btfss SSPSTAT, 2
	goto ExitI2C

    banksel ADCON0
  
	movlw LoadStatus
	movwf FSR

	;Calculate address of next data byte
	movf HMCounter, W
	addwf FSR, f
	movf INDF, W

WriteI2C:
	;Send Data
	bcf SSPCON, WCOL
  movlw 0xAA
	movwf SSPBUF
	btfsc SSPCON, WCOL
	goto WriteI2C
	
	;Release Clock
	incf TWIByte, f
	bsf SSPCON, CKP

	;Increment Counter, check if equal to 7 make it 0
	incf HMCounter, f
	movlw 7
	subwf HMCounter, W
	btfsc STATUS, Z
    clrf HMCounter
	
	goto ExitI2C
  
ExitI2C:
  nop
  return


Start:
   
  
  banksel ADCON1
  ;confugure I2C
	clrf SSPSTAT
	clrf SSPCON2
	;configure address of I2C slave
    movlw I2C_ADDR
    movwf SSPADD

	;configure I2C ports as Input
	bsf TRISC, 3
	bsf TRISC, 4
  
  ;Enable SSP/I2C interrupt
	bsf PIE1, SSPIE
	
  banksel ADCON0
	bsf INTCON, PEIE
  
	bsf INTCON, INTE
	bsf INTCON, GIE	
	bcf INTCON, INTF
  
  movlw b'11001100'
	movwf LoadStatus
  movwf CommandByte
  
Loop:
  nop
  goto Loop

  end

