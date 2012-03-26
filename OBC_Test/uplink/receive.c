#include "common.h"
#include "receive.h"
#include "ax25.h"

//conformation to standard : CRC and transmission sequence: least/most significant bit
//bit stuffing

void cc1020_init_dio(void)
{
  cbi(DDR_RX, DCLK);
  cbi(DDR_RX, DIO);
}

uint8_t cc1020_receive_bit(void)
{
  uint8_t bit;
  
  while(!(PIN_RX & (1 << DCLK)));
  bit = PIN_RX & (1 << DIO);
  while(PIN_RX & (1 << DCLK));
  
  return bit;
}

uint8_t cc1020_receive_packet(uint8_t *data)
{
  static uint8_t byte = 0;
  uint8_t i, bit, size = 0, flag = 0;
  uint8_t ax_counter = 0;
  
  uint16_t crc_in_packet, crc;
  
  while(byte != AX_FLAG)
  {
    bit = cc1020_receive_bit();
    byte = byte << 1;
    if(bit == 0)
      byte &= ~(0x01);
    else
      byte |= 0x01;
  }
  
  do
  {
    for(i = 0; i < 8; i++)
    {
      bit = cc1020_receive_bit();
      byte = byte << 1;
      if(bit == 0)
        byte &= ~(0x01);
      else
        byte |= 0x01;
      
      if(bit)
      {
        ax_counter++;
        if(ax_counter == 5)
        {
          ax_counter = 0;
          bit = cc1020_receive_bit();
          if(bit)
          {
            byte = byte << 1;
            byte |= 0x01;
            
            flag = 1;
            
            if(i == 5)
              i++;
            else
              return 0;
          }
        }
      }
      else
        ax_counter = 0;
    }
    data[size++] = byte;
    
  }while(!flag);
  
  if(size < 4)
    return 0;
  
  memcpy(&crc_in_packet, data + size - 3, sizeof(uint16_t));
  crc = calculate_crc_16(data, size - 3);
  if (crc_in_packet == crc)
    return size;
  
  //send_UART(data, size);
  //send_UART("CRC fail\r", 9);
  return 0;
}
