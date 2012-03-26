#include "common.h"
#include "ax25.h"

void make_ax25_frame_from_data(uint8_t *frame, uint8_t *data)
{
  uint8_t i, buf[10], size = 0;
  uint16_t crc;
  
  frame[size++] = AX_FLAG;
  
  memcpy(buf, "CQ    ", AX_ADDR_SIZE);
  for(i = 0; i < AX_ADDR_SIZE; i++)
    frame[i + size] = buf[i] << 1;
  size += AX_ADDR_SIZE;
  frame[size++] = 0b01100000;
  
  memcpy(buf, "VU2DMQ", AX_ADDR_SIZE);
  for(i = 0; i < AX_ADDR_SIZE; i++)
    frame[i + size] = buf[i] << 1;
  size += AX_ADDR_SIZE;
  frame[size++] = 0b01101000;
  
  memcpy(buf, "RELAY ", AX_ADDR_SIZE);
  for(i = 0; i < AX_ADDR_SIZE; i++)
    frame[i + size] = buf[i] << 1;
  size += AX_ADDR_SIZE;
  frame[size++] = 0b01100001;
  
  frame[size++] = AX_CONTROL;
  frame[size++] = AX_PID;
  
  memcpy(frame + size, data, HM_SIZE + 4);
  size += HM_SIZE + 4;
  
  crc = calculate_crc_16(frame + 1, FRAME_SIZE - 4);
  
  memcpy(frame + size, &crc, 2);
  size += 2;
  
  frame[size] = AX_FLAG;
  
}

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


