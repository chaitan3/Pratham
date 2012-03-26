;**********************************************************************
;   This file is a basic code template for object module code         *
;   generation on the PIC16F877A. This file contains the              *
;   basic code building blocks to build upon.  As a project minimum   *
;   the 16F877A.lkr file will also be required for this file to       *
;   correctly build. The .lkr files are located in the MPLAB          *  
;   directory.                                                        *
;                                                                     *
;   If interrupts are not used all code presented between the         *
;   code section "INT_VECTOR and code section "MAIN" can be removed.  *
;   In addition the variable assignments for 'w_temp' and             *
;   'status_temp' can be removed.                                     *                         
;                                                                     *
;   If interrupts are used, as in this template file, the 16F877A.lkr *
;   file will need to be modified as follows: Remove the lines        *
;     CODEPAGE   NAME=vectors  START=0x0      END=0x4      PROTECTED  *
;   and                                                               *
;     SECTION    NAME=STARTUP  ROM=vectors                            *
;   and change the start address of the page0 section from 0x5 to 0x0 *                                                               *
;                                                                     *
;   Refer to the MPASM User's Guide for additional information on     *
;   features of the assembler and linker (Document DS33014).          *
;                                                                     *
;   Refer to the respective PIC data sheet for additional            *
;   information on the instruction set.                               *
;                                                                     *
;**********************************************************************
;                                                                     *
;    Filename:	    xxx.asm                                           *
;    Date:                                                            *
;    File Version:                                                    *
;                                                                     *
;    Author:                                                          *
;    Company:                                                         *
;                                                                     * 
;                                                                     *
;**********************************************************************
;                                                                     *
;    Files required:                                                  *
;                                                                     *
;                                                                     *
;                                                                     *
;**********************************************************************
;                                                                     *
;    Notes:                                                           *
;                                                                     *
;                                                                     *
;                                                                     *
;                                                                     *
;**********************************************************************


	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions
	
	__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _LVP_OFF & _CPD_OFF

; '__CONFIG' directive is used to embed configuration data within .asm file.
; The labels following the directive are located in the respective .inc file.
; See respective data sheet for additional information on configuration word.

;Constant Definitions
;---------------------------------------------------------------------
#define NODE_ADDR 0x22 ; I2C address of this node
; Change this value to address that
; you wish to use.
;---------------------------------------------------------------------


;***** VARIABLE DEFINITIONS (examples)

; example of using Shared Uninitialized Data Section
INT_VAR		UDATA_SHR	   
databyte    RES 1           ; main data variable

; example of using Uninitialized Data Section
TEMP_VAR	UDATA			; explicit address specified is not required
WREGsave res 1
STATUSsave res 1
FSRsave res 1
PCLATHsave res 1
Temp res 1 ;

HEALTH_DATA UDATA 0x20
LoadStatus res 1	;0x20
LoadEnable	res 1	;0x21
V3v3       res 1 	;0x22
V5v     	res 1	;0x23
Vbatt		res 1	;0x24
Idischarge	res 1	;0x25
Icharge		res 1	;0x26
Index  res 1 ;0x27	


;**********************************************************************
RESET_VECTOR	CODE	0x000		; processor reset vector
	nop				; nop required for icd
	movlw	high  start		; load upper byte of 'start' label
	movwf	PCLATH			; initialize PCLATH
	goto	start			; go to beginning of program


INT_VECTOR	CODE	0x004		; interrupt vector location
	goto ISR


;---------------------------------------------------------------------
; Macros
;---------------------------------------------------------------------
memset macro Buf_addr,Value,Length
movlw Length ; This macro loads a range of data memory
movwf Temp ; with a specified value. The starting
movlw Buf_addr ; address and number of bytes are also
movwf FSR ; specified.
SetNext movlw Value
movwf INDF
incf FSR,F
decfsz Temp,F
goto SetNext
endm

LFSR macro Address,Offset ; This macro loads the correct value
movlw Address ; into the FSR given an initial data
movwf FSR ; memory address and offset value.
movf Offset,W
addwf FSR,F
endm




MAIN	CODE
a1 equ 0X28
a2 equ  0X29
bcf STATUS,RP0
bsf STATUS,RP1
clrf TRISC
bcf STATUS,RP0
toggle:
clrf a1
clrf a2
movlw b'00000111'
movwf PORTC
loop:
decfsz a1
goto loop
decfsz a2
goto loop
goto toggle
ISR
movwf WREGsave ; Save WREG
movf STATUS,W ; Get STATUS register
banksel STATUSsave ; Switch banks, if needed.
movwf STATUSsave ; Save the STATUS register
movf PCLATH,W;
movwf PCLATHsave ; Save PCLATH
movf FSR,W ;
movwf FSRsave ; Save FSR
banksel PIR1
btfss PIR1,SSPIF ; Is this a SSP interrupt?
goto $ ; No, just trap here.
bcf PIR1,SSPIF
call SSP_Handler ; Yes, service SSP interrupt.
banksel FSRsave
movf FSRsave,W ;
movwf FSR ; Restore FSR
movf PCLATHsave,W ;
movwf PCLATH ; Restore PCLATH
movf STATUSsave,W ;
movwf STATUS ; Restore STATUS
swapf WREGsave,F ;
swapf WREGsave,W ; Restore WREG
retfie ; Return from interrupt.

;---------------------------------------------------------------------
Setup
;
; Initializes program variables and peripheral registers.
;---------------------------------------------------------------------

banksel LoadStatus
movlw 0x42 
movwf LoadStatus
movlw 0x43
movwf LoadEnable
movlw 0x44 
movwf V3v3
movlw 0x45 
movwf V5v
movlw 0x46 
movwf Vbatt
movlw 0x47 
movwf Idischarge
movlw 0x48 
movwf Icharge

banksel PORTB ; Clear various program variables
clrf PORTB
clrf PIR1
banksel TRISC
movlw 0x18
movwf TRISC ; Set RC4 and RC3 as inputs as required by I2C
movlw 0x36 ; Setup SSP module for 7-bit
banksel SSPCON
movwf SSPCON ; address, slave mode
movlw NODE_ADDR
banksel SSPADD
movwf SSPADD
clrf SSPSTAT
banksel PIE1 ; Enable interrupts
bsf PIE1,SSPIE
bsf INTCON,PEIE ; Enable all peripheral interrupts
bsf INTCON,GIE ; Enable global interrupts
bcf STATUS,RP0
return



;---------------------------------------------------------------------
SSP_Handler
;---------------------------------------------------------------------
; The I2C code below checks for 5 states:
;---------------------------------------------------------------------
; State 1: I2C write operation, last byte was an address byte.
; SSPSTAT bits: S = 1, D_A = 0, R_W = 0, BF = 1
;;
;State2: I2C write operation, last byte was a data byte.
; SSPSTAT bits: S = 1, D_A = 1, R_W = 0, BF = 1
;;
;State3: I2C read operation, last byte was an address byte.
; SSPSTAT bits: S = 1, D_A = 0, R_W = 1 (see Appendix C for more information)
;;
;State4: I2C read operation, last byte was a data byte.
; SSPSTAT bits: S = 1, D_A = 1, R_W = 1, BF = 0
;;
;State5: Slave I2C logic reset by NACK from master.
; SSPSTAT bits: S = 1, D_A = 1, BF = 0, CKP = 1 (see Appendix C for more information)
;
; For convenience, WriteI2C and ReadI2C functions have been used.
;----------------------------------------------------------------------
banksel SSPSTAT
movf SSPSTAT,W ; Get the value of SSPSTAT
andlw b'00101101' ; Mask out unimportant bits in SSPSTAT.
banksel Temp ; Put masked value in Temp
movwf Temp ; for comparision checking.

State1: ; Write operation, last byte was an
movlw b'00001001' ; address, buffer is full.
xorwf Temp,W ;
btfss STATUS,Z ; Are we in State1?
goto State2 ; No, check for next state.....
banksel SSPBUF ; Do a dummy read of the SSPBUF.
movf SSPBUF,W
return

State2: ; Write operation, last byte was data,
movlw b'00101001' ; buffer is full.
xorwf Temp,W
btfss STATUS,Z ; Are we in State2?
goto State3 ; No, check for next state.....
banksel SSPBUF ; Get the byte from the SSP.
movf SSPBUF,W
movwf databyte ; store in databyte
return

State3: ; Read operation, last byte was an address,
movf Temp,W ;
andlw b'00101100' ; Mask BF bit in SSPSTAT
xorlw b'00001100'
btfss STATUS,Z ; Are we in State3?
goto State4 ; No, check for next state.....
banksel LoadStatus
movf LoadStatus, W
btfsc databyte, 0
goto StartSendingData
call WriteI2C ; Write the byte to SSPBUF
return

StartSendingData
movlw 0x20
movwf Index
movwf FSR
movf INDF ,W
call WriteI2C ; Write the byte to SSPBUF
return

State4: ; Read operation, last byte was data,
banksel SSPCON ; buffer is empty.
btfsc SSPCON, CKP
goto State5
movlw b'00101100'
xorwf Temp,W
btfss STATUS,Z ; Are we in State4?
goto State5 ; No, check for next state....
banksel LoadStatus
incf Index
movf Index , W
movwf FSR
movf INDF, W
call WriteI2C ; Write the byte to SSPBUF
return

State5:
movf Temp,W ; NACK received when sending data to the master
andlw b'00101000' ; Mask RW bit in SSPSTAT
xorlw b'00101000' 
btfss STATUS,Z ;
goto I2CErr ;
return ; If we aren’t in State5, then something is
; wrong.
I2CErr 
nop
banksel PORTC ; Something went wrong! Set LED
bsf PORTB,2 ; and loop forever
goto $ 
return


;---------------------------------------------------------------------
; WriteI2C
;---------------------------------------------------------------------
WriteI2C
banksel SSPSTAT
btfsc SSPSTAT,BF ; Is the buffer full?
goto WriteI2C ; Yes, keep waiting.
banksel SSPCON ; No, continue.
DoI2CWrite
bcf SSPCON,WCOL ; Clear the WCOL flag.
movwf SSPBUF ; Write the byte in WREG
btfsc SSPCON,WCOL ; Was there a write collision?
goto DoI2CWrite
bsf SSPCON,CKP ; Release the clock.
bsf SSPCON,CKP ; Release the clock.
return




start
call Setup
banksel WREGsave
Main 
goto Main ; Loop forever.
	
END                       ; directive 'end of program'

