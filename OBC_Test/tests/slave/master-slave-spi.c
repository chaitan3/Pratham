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


void init_UART_GPS(void )
{

  UCSR0A = 0;
  UCSR0B = 0;
  UCSR0C = 0;
  
  ///Double Baud Rate
  UCSR0A |= _BV(U2X0);
  ///Enable Reception
  UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
  ///8-bit Data Byte, 2 Stop bits
  UCSR0C |= _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);
  ///Set Baud Rate to 9600
  UBRR0L = 103;
  UBRR0H = 0;
}

void send_preflight(char *data, uint16_t size)
{
  uint16_t i = 0;
  
  ///Loop through the data
  while(i < size)
  {
    ///* Wait for empty transmit buffer 
    while ( !(UCSR0A & (_BV(UDRE0))) );
    ///* Start transmission
    UDR0 = data[i];
    i++;
  }
}

///Variables required for receiving data thru the SPI interface
volatile char message[256];
volatile uint8_t end_spi = 0, start_spi = 0, t = 0, process = 0, spi;

static uint32_t read_addr = 0, write_addr = 0;
/**
 * @brief Interrupt service routine for SPI
 */
ISR(SPI_STC_vect)
{
  spi = SPDR;
  
  if((!process) && (start_spi >= N_END_SPI))
  {
    message[t] = spi;
    
    if(message[t] == END_SPI)
      end_spi++;
    else if(end_spi)
      end_spi = 0;
      
    if(end_spi == N_END_SPI)
      process = 1;
    t++;
  }
  
  if(spi == START_SPI)
    start_spi++;
  else if(start_spi < N_END_SPI)
    start_spi = 0;
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
  
  init_UART_GPS();
  send_preflight("Slave", 5);
  
   /// Current state of satellite
  uint8_t frame[256];
  uint8_t transmission = 0, command;
  uint16_t crc, recv_crc;
  
  
  ///Disable Watchdog Timer
  wdt_disable();
  ///Initialise SPI as slave
  init_SPI_slave();
  
  ///EEPROM function to initialise I/O
  //ioinit();
  
  ///Initialise CC1020 DIO pins
  //cc1020_init_dio();
  
  ///Enable Global Interrupts
  sei();
  
  ///Main Loop for slave
  while (1)
  {
    if(process)
    {
      crc = calculate_crc_16((uint8_t *)message, t - (end_spi + sizeof(uint16_t)));
      memcpy((void *)&recv_crc, (void *)&(message[t - (end_spi + sizeof(uint16_t))]), sizeof(uint16_t));
      if(crc == recv_crc)
      {
        command = message[0];
        if(command == BEGIN_TX_COMM)
        {
          //transmission = IN;
          send_preflight("Recvcomm", 8);
          send_preflight((char *)message + 2, message[1]);
        }
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
      end_spi = 0;
      start_spi = 0;
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
