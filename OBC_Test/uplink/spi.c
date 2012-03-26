/**
 * @file spi.c
 * @brief Interfacing with the SPI bus
 */
#include "common.h"
#include "spi.h"

void init_SPI_slave(void)
{
  ///Set the Direction register for the SPI pins
  DDRB &= 0xF0;
  sbi(DDRB, PB3);
  
  ///Enable SPI, Interrupt, Slave 
  ///@todo Check frequency 
  SPCR = (1<<SPIE) |(1<<SPE);
}

void init_SPI(void)
{
  ///Set MOSI and SCK output, all others input
  DDRB = (1<<SPICLK) | (1<<SPIDO) | (1<<SS);
  sbi(PORTB, SS);
  
  ///Enable SPI, Master, set clock rate
  ///@todo Check frequency 
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);
}

void init_SPI_trans(uint8_t CS_pin){
  
  ///Pull the CS_pin on the corresponding port low
  PORT_CS |=_BV(SLAVE)|_BV(CC1020)|_BV(ADC_S);
  cbi (PORT_CS, CS_pin);

}

void SPI_send(char* str, uint16_t size) 
{
  uint16_t i = 0;
  ///In while loop:
  while(i<size)
  {
	 ///* Send one char
    SPDR = str[i];
	 ///* Wait for SPIF flag to be set (TX complete)
    while(!(SPSR & (1<<SPIF)));
    i++;
  }
  return;
}


uint8_t SPI_transfer(uint8_t transmit_byte)
{
  SPDR = transmit_byte;
  ///Wait for Transmission to complete
  while(!(SPSR & (1<<SPIF)));
  ///return received Byte
  return SPDR;
}

