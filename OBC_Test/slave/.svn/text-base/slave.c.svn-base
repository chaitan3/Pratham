/**
 * @file slave.c
 * @brief Slave main file
 */

#include "common.h"
#include "comm.h"
#include "spi.h"
#include "eeprom.h"
#include "slave_comm.h"
#include "peripherals.h"
#include "ax25.h"
#include "transmit.h"

///Variables required for receiving data thru the SPI interface
volatile char message[256];
volatile uint8_t end, t = 0, process = 0;

static uint32_t read_addr = 0, write_addr = 0;
/**
 * @brief Interrupt service routine for SPI
 */
ISR(SPI_STC_vect)
{
  if(!process)
  {
    message[t] = SPDR;
    if(message[t] == END_SPI)
      end++;
    else if(end)
      end = 0;
    if(end == N_END_SPI)
      process = 1;
    t++;
  }
}

void write_frame_to_eeprom(uint8_t *frame)
{
  //correct the eeprom function for supporting 32-bit addresses
  static uint8_t start = 0;
  
  if((write_addr == read_addr) && start)
    read_addr += FRAME_SIZE;
    
  eeprom_write_bytes(write_addr, FRAME_SIZE,frame);
  write_addr += FRAME_SIZE;
  
  if(write_addr + FRAME_SIZE > MAX_ADDR)
    write_addr = 0;
  
  start = 1;
}

void read_frame_from_eeprom(uint8_t *frame)
{
  eeprom_read_bytes(read_addr, FRAME_SIZE, frame);
  read_addr += FRAME_SIZE;
  
  if(read_addr + FRAME_SIZE > MAX_ADDR)
    read_addr = 0;
}
/** @brief Main function
 *  @todo Complete slave coding
 */

int main(void) {
  
  /// Current state of satellite
  uint8_t frame[256];
  uint8_t transmission = 0, command;
  uint16_t crc, recv_crc;
  
  
  ///Disable Watchdog Timer
  wdt_disable();
  ///Initialise SPI as slave
  init_SPI_slave();
  
  ///EEPROM function to initialise I/O
  ioinit();
  
  ///Initialise CC1020 DIO pins
  cc1020_init_dio();
  
  ///Enable Global Interrupts
  sei();
  
  ///Main Loop for slave
  while (1)
  {
    if(process)
    {
      crc = calculate_crc_16((uint8_t *)message, t - (end + 1));
      memcpy((void *)&recv_crc, (void *)&(message[t - (end + 1)]), sizeof(uint16_t));
      
      if(crc == recv_crc)
      {
        command = message[0];
        if(command == BEGIN_TX_COMM)
          transmission = IN;
        else if(command == BEGIN_TX_GS)
          transmission = GS;
        else if(command == END_TX)
        {
          SPI_transfer(ACK);
          transmission = 0;
        }
        else if(command == HM_DATA)
        {
          make_ax25_frame_from_data(frame, (uint8_t *)message + 2);
          write_frame_to_eeprom(frame);
        }
        else if(command == REAL_TIME)
          make_ax25_frame_from_data(frame, (uint8_t *)message + 2);
      }
      end = 0;
      t = 0;
      process = 0;
    }
    
    if(transmission == GS)
    {
      read_frame_from_eeprom(frame);
      if(read_addr == write_addr)
        read_addr -= FRAME_SIZE;
        
      cc1020_transmit_packet(frame, FRAME_SIZE);
    }
    else if(transmission == IN)
      cc1020_transmit_packet(frame, FRAME_SIZE);
  }
  return 0;
}
