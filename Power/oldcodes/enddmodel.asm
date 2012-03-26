; PIC code for the engineering model
; Code written on October 30, 2008

; Read data from sensors and store the most recent one in RAM
; Read Load status and store in RAM
; Take action if any load is faulty
; On UART receive interrupt, execute the received command and transmit all the data
; Connections are as follows:
; Sensors: Icharge-> PA5/AN4, Idischarge-> PA3/AN3, Vbatt-> PA2/AN2, V5v-> PA1/AN1, V3v3-> PA0/AN0
; Load Enables: RD7, RD6, RD5, RD4, RD3
; Load Status : RB5, RB4, RB2, RB1, RB0   

; The format for the load enable/disable command is as follows: from MSB to LSB
; | Load4 | Load3 | Load2 | Load1 | Load0 |   X   |   X   |  EN   |
; Loadx : Select bit for Loadx, 1=Load selected , 0=Load not selected 
; X     : unused bits
; EN    : Enable/Disable bit , 1= Enable the selected loads, 0=Disable the selected loads 
; Multiple loads can be enabled(or disabled) at the same time

; When a command is received, a 8-byte data packet is sent. 
; Format of the packet is : LoadStatus(PORTB), LoadEnable(PORTD), 3.3V bus voltage, 5V bus voltage, battery bus voltage,
; battery discharging current, battery charging current, end of transmission byte(0x03)
#include <p16f877a.inc>

__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _LVP_OFF & _CPD_OFF 

cblock 0x20         ;data variables
	count
	channel
	TXdata
	A2Dresult
	temp
	LoadStatus		;0x25
	LoadEnable		;0x26
	V3v3        	;0x27
	V5v     		;0x28
	Vbatt			;0x29
	Idischarge		;0x2A
	Icharge			;0x2B
	temp1
	temp2
endc

w_temp        EQU     0x70        ; variable used for context saving 
status_temp   EQU     0x71        ; variable used for context saving
     
   	ORG     0x000             ; processor reset vector
	clrf    PCLATH            ; ensure page bits are cleared
 	goto    main              ; go to beginning of program


	ORG     0x004             ; interrupt vector location
	movwf   w_temp            ; save off current W register contents
	movf	STATUS,w          ; move status register into W register
	movwf	status_temp       ; save off contents of STATUS register

	btfsc PIR1, RCIF
	goto ServiceUARTReceive   ; UART receive interrupt
 
ExitISR: 
	movf    status_temp,w     ; retrieve copy of STATUS register
	movwf	STATUS            ; restore pre-isr STATUS register contents
	swapf   w_temp,f
	swapf   w_temp,w          ; restore pre-isr W register contents
	retfie                    ; return from interrupt
a1 EQU 0X30
a2 EQU  0X31
main:
bcf STATUS,RP1
bsf STATUS,RP0
movlw 0X00
movwf TRISC
bcf STATUS,RP0
toggle:
clrf a1
clrf a2
movlw b'11000111'
movwf PORTC
loop:
decfsz a1
goto loop
decfsz a2
goto loop
clrf PORTC
goto toggle
     
    

bsf STATUS, RP0      ;move to Bank 1
	movlw  0XFF
    movwf  TRISA		 ;PortA as inputs
	movlw 0x07           
	movwf TRISD			 ;PortD as outputs
	movlw 0xff
	movwf TRISB			 ;PortB as inputs
	clrf  ADCON1         ;Left justified result, all 8 channels as analog inputs
	movlw 0x80
	movwf TRISC			 ;set RC7 as input
	movlw 0x19
	movwf SPBRG			 ;9600 baud
	clrf TXSTA
	bsf TXSTA, TXEN		 ;enable transmitter	
	bsf PIE1, RCIE       ;enable receive interrupt

	bcf STATUS, RP0      ;Bank 0
	clrf RCSTA
	bsf RCSTA, SPEN		 ;enable UART on the pins
	bsf RCSTA, CREN      ;enable receiver
	movlw  0X81
    movwf  ADCON0        ;Fosc/32, channel 0, ADC ON    
	movlw 0x05	
    movwf channel        ;initialize the ADC channel
	movlw 0xf8
	movwf PORTD          ;enable all loads
	movwf LoadEnable 
	bsf INTCON, GIE
	bsf INTCON, PEIE     ;enable interrupts
 

LOOP:                    ;the main loop

	movf PORTB, W
    movwf LoadStatus     ;read status of the loads
	
	btfss LoadStatus, 5 ;take action if any load fails
	bcf LoadEnable, 7
	btfss LoadStatus, 4
	bcf LoadEnable, 6
	btfss LoadStatus,2
	bcf LoadEnable, 5	
	btfss LoadStatus,1
	bcf LoadEnable, 4	
	btfss LoadStatus,0
	bcf LoadEnable, 3
	movf LoadEnable, W
	movwf PORTD         ; action taken
	
	movf channel, W     ; setup the ADC
	addlw 0xff
	movwf temp
	addlw 0
	rlf temp, f
	rlf temp, f
	rlf temp, f
	movlw 0x81
	addwf temp, W
	movwf ADCON0
	call A2Dconvert     ; call the ADC conversion subroutine
	
	movf channel, W    
	addlw 0x26
	movwf FSR           ; pointer for health data memory
	movf A2Dresult, W
	movwf INDF          ; move data from W to memory pointed by FSR
	
	decfsz channel, f   ; change channel for conversion in next loop
    goto LOOP
	movlw 0x05
	movwf channel
	goto LOOP

TX: bcf STATUS, RP0     ; subroutine for data transmit, takes data to be transmitted from "TXdata"
	bcf STATUS, RP1
TXwait:
	btfss PIR1, TXIF
	goto TXwait
	movf TXdata, W      
	movwf TXREG
	return

A2Dconvert:	              ;subroutine for ADC
	bcf STATUS, RP0
	bcf STATUS, RP1
	movlw 0x80	
	movwf count
A2Dcapt:	
	decfsz count, F
	goto A2Dcapt
	bsf  ADCON0, GO		  ;begin conversion
A2Dwait:				  ;wait for conversion to finish
	btfsc ADCON0,GO
	goto  A2Dwait
	movf ADRESH,W
	movwf A2Dresult       ;store the result in "A2Dresult"
	return
	
ServiceUARTReceive:       ;ISR for the UART receive
	movf RCREG, w         ;read received data
	movwf temp2
	btfss temp2, 0        ; check if command is for enabling or disabling the loads
	goto DisableLoad
EnableLoad
    andlw 0xf8
	iorwf LoadEnable, f
	movf LoadEnable, w
	movwf PORTD           ; enable the load according to the received data
InitPointer:               	 
	movlw 0x25
	movwf FSR             ; initialize pointer for transmitting data
	movlw 0x07
	movwf temp1
SendData:                 ; send the 7 bytes of data
	movf INDF, w
	movwf TXdata
	call TX
	incf FSR
	decfsz temp1, f
	goto SendData

	movlw 0x03             ; 0x03 denotes end of transmission
	movwf TXdata
	call TX	
	goto ExitISR
DisableLoad:               ; disable the loads according the the command received
	xorlw 0xff
    iorlw 0x07
	andwf LoadEnable, f
	movf LoadEnable, w
	movwf PORTD
	goto InitPointer

	end

