#include "common.h"
#include "transmit.h"

void cc1020_init_dio(void)
{
  cbi(DDR_TX, DCLK);
  sbi(DDR_TX, DIO);
  sbi(PORT_TX, DIO);
}

void cc1020_transmit_bit(uint8_t bit)
{
  while(PIN_TX & (1 << DCLK));
  if(bit)
    sbi(PORT_TX, DIO);
  else
    cbi(PORT_TX, DIO);
  while(!(PIN_TX & (1 << DCLK)));
}

//implement bit stuffing
/*void cc1020_transmit_packet(uint8_t *data, uint8_t size)
{
  uint8_t i, j, byte, bit;
  for(i = 0; i < size; i++)
  {
    byte = data[i];
    for(j = 0; j < 8; j++)
    {
      bit = byte & 0x01;
      
      while(PIN_TX & (1 << DCLK));
      if(bit)
        sbi(PORT_TX, DIO);
      else
        cbi(PORT_TX, DIO);
      while(!(PIN_TX & (1 << DCLK)));
      
      byte = byte >> 1;
    }
  }
  
  sbi(PORT_TX, DIO);
}*/

void cc1020_transmit_packet(uint8_t *data, uint8_t size)
{
  uint8_t i, j, byte, bit;
  uint8_t ax_counter = 0;
  
  cc1020_transmit_bit(1);
  
  for(i = 0; i < size; i++)
  {
    byte = data[i];
    for(j = 0; j < 8; j++)
    {
      bit = byte & 0x80;
      cc1020_transmit_bit(bit);
      
      if((i != 0) && (i != size - 1))
      {
        if(bit)
          ax_counter++;
        else
          ax_counter = 0;
        if(ax_counter == 5)
        {
          ax_counter = 0;
          cc1020_transmit_bit(0);
        }
      }  
      
      byte = byte << 1;
    }
  }
  
  sbi(PORT_TX, DIO);
}

