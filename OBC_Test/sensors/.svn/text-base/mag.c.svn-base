/**
 * @file mag.c
 * @brief Read the magnetic field vector from the Magnetometer
 */
#include "common.h"
#include "mag.h"
#include "peripherals.h"


void init_UART_MM(void )
{

  UCSR1A = 0;
  UCSR1B = 0;
  UCSR1C = 0;
  
  ///Double Baud Rate
  UCSR1A |= _BV(U2X1);
  ///Enable Transmission and Reception
  UCSR1B |= _BV(TXEN1) | _BV(RXEN1);
  ///8-bit Data Byte
  UCSR1C |= _BV(UCSZ11) | _BV(UCSZ10);
  ///Set Baud Rate to 9600
  UBRR1L = 103;
  UBRR1H = 0;
}

void poll_MM(void)
{
  ///Temporary variables for magnetometer readings
  int16_t x, y, z;
  
  ///Send the poll command
  send_MM_cmd("*00P\r", 0);
  
  ///Receive the values from the magnetometer
  x = ((int16_t)receive_UART()) << 8;
  x &= 0xFF00;
  x |= (int16_t)receive_UART();
  
  y = ((int16_t)receive_UART()) << 8;
  y &= 0xFF00;
  y |= (int16_t)receive_UART();
  
  z = ((int16_t)receive_UART()) << 8;
  z &= 0xFF00;
  z |= (int16_t)receive_UART();
  
  ///Receive \n
  receive_UART();
  
  ///Convert the readings to Gauss
  Current_state.mm.B_x = ((float) x) / 15000;
  Current_state.mm.B_y = ((float) y) / 15000;
  Current_state.mm.B_z = ((float) z) / 15000;
}

uint8_t receive_UART(void)
{
  ///Wait for a Byte to arrive
  while ((UCSR1A & (1 << RXC1)) == 0);
  return UDR1;
}

void send_MM_cmd(char *data, uint8_t lines)
{
  char a, i;
  ///Send the char array till stop
  while((*data) != '\0')
  {
    ///* Wait for empty transmit buffer 
    while ( !(UCSR1A & (_BV(UDRE1))) );
    ///* Start transmission
    UDR1 = *data;
    data++;
  }
  
  ///No. of lines to wait for
  for(i = 0; i < lines; i++)
    while((a = receive_UART()) != '\r');
}

void configure_MM(void)
{
  /// <b>Initialization commands</b>

  ///Baud Rate-> Set baud rate=9600 bps
  send_MM_cmd("*99WE\r", 1);
  send_MM_cmd("*99!BR=S\r", 2);
  ///Device ID-> Set device ID=00
  send_MM_cmd("*99WE\r", 1);
  send_MM_cmd("*99ID=00\r", 1); 
  ///Format-> Turn Binary Mode on mode on
  send_MM_cmd("*00WE\r", 1);
  send_MM_cmd("*00B\r", 1);
  
  ///Output-> Turn polling mode on
  send_MM_cmd("*00P\r", 1);
  
  ///Turn off Zero reading
  send_MM_cmd("*00WE\r", 1);
  send_MM_cmd("*00ZF\r", 1);
  
  ///S/R Mode-> Automatic S/R Pulses
  send_MM_cmd("*00WE\r", 1);
  send_MM_cmd("*00TN\r", 1);
  ///Average Readings-> Turn average off
  send_MM_cmd("*00WE\r", 1);
  send_MM_cmd("*00VF\r", 1);
  ///Store parameters: Paramters reloaded on restoring power.
  send_MM_cmd("*00WE\r", 1);
  send_MM_cmd("*00SP\r", 2);
}

