#include "common.h"
#include "ax25.h"

uint16_t update_crc_16(uint8_t byte, uint16_t crc)
{
  uint8_t bit, crcbit, j;
  
  for (j = 0; j < 8; j++)
  {
    bit = byte & 0x01;      
    crcbit = crc & 0x01;
    crc = crc >> 1;      
    if (bit != crcbit)
      crc ^= CRC_CCIT_REV;
    byte = byte >> 1;
  }
  return crc;
}

uint16_t calculate_crc_16(uint8_t *data, uint8_t size)
{
  uint16_t crc = 0xFFFF;
  uint8_t i;
  
  for(i = 0; i < size; i++)
    crc = update_crc_16(data[i], crc);
    
  return ~crc;
}


