/**
 * @file slave_comm.c
 * @brief Send commands/data to the Slave
 */

#include "slave_comm.h"
#include "spi.h"
#include "ax25.h"

void slave_send (uint8_t command, char* data, uint8_t size){

  static uint32_t seq = 0;
  uint16_t crc = 0xFFFF;
  uint8_t i, c, tsize = size;
  
  ///Initialize SPI
  init_SPI_trans (SLAVE);
  
  ///* Start communication
  c = START_SPI;
  for (i = 0; i < N_START_SPI; i++)
    SPI_send ((char *)&c, sizeof (uint8_t));
  
  ///* Notify the command
  SPI_send ((char *)&command,sizeof (uint8_t));
  crc = update_crc_16(command, crc);
  
  ///* Send size of data
  if((command == HM_DATA) ||(command == REAL_TIME))
  {
    tsize += 4;
    SPI_send ((char *)&tsize,sizeof (uint8_t));
    crc = update_crc_16(tsize, crc);
    ///* Send Sequence Number
    SPI_send ((char *)&seq,sizeof (uint32_t));
    for(i = 0; i < sizeof(uint32_t); i++)
      crc = update_crc_16(((uint8_t *)&seq)[i], crc);
    seq++;
  }
  else
  {
    SPI_send ((char *)&tsize,sizeof (uint8_t));
    crc = update_crc_16(tsize, crc);
  }
    
  ///* Send data corresponding to command
  SPI_send ((char *)data,sizeof(char) * size);
  for(i = 0; i < size; i++)
    crc = update_crc_16(data[i], crc);
    
  ///* Send CRC
  crc = ~crc;
  SPI_send ((char *)&crc, sizeof (uint16_t));
    
  ///* End communication
  c = END_SPI;
  for (i = 0; i < N_END_SPI; i++)
    SPI_send ((char *)&c, sizeof (uint8_t));
      
}


