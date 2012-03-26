/**
 * @file sun.c
 * @brief Read the ADC values for the Sun sensors
 */

#include "common.h"
#include "sun.h"
#include "spi.h"
#include "peripherals.h"

void configure_SS(void)
{
  ///Choose the ADC IC on the SPI bus
  init_SPI_trans(ADC_S);
  
  ///Set PD and range pins as input
  cbi(DDRB, PB6);
  cbi(DDRB, PB7);
  
  ///Set PD high: power up the ADC IC
  sbi(PORTB, PB6);
  
  ///Configure GPIO pins - GPIO3 as PD and GPIO2 as range input
  SPI_transfer(0b01000001);
  
  SPI_transfer(0b10000000);
  
  ///Configure Auto-2 Mode
  
  ///Program the Program register of Auto-2 Mode:
  
  SPI_transfer(0b10010001);
  
  SPI_transfer(0b01000000);
  
  ///Set Mode to Auto-2
  
  SPI_transfer(0b00111100);
  
  SPI_transfer(0b00000000);
}

void poll_SS(void)
{
  uint8_t channel = 0;
  
  ///Loop for reading the 6 sun sensor values
  while(channel <= 5)
  {
    ///* Put the ADC reading in the appropriate variable
    Current_state.ss.reading[channel] = SPI_transfer(0x00);
    Current_state.ss.reading[channel] = Current_state.ss.reading[channel] << 8;
    Current_state.ss.reading[channel] &= 0x0F00;
    Current_state.ss.reading[channel] |= SPI_transfer(0x00);
    channel++;
  }
  ///Power Down PD low
  cbi(PORTB, PB6);
  
}
