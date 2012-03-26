#include "common.h"
#include "spi.h"
#include "receive.h"
#include "functions.h"
#include "uart.h"
int main(void)
{
  uint8_t frame[100], x;
  
  init_UART();
  send_UART((uint8_t *)"Hi\r\n", 3);
  
  init_SPI();
  cc1020_init_dio();
  
  x = init_CC1020();
  sprintf((char *)frame, "%d Done1\r\n", x);
  send_UART(frame, strlen((char *)frame));
  
  
  while(1) 
  {
    x = cc1020_receive_packet(frame);
    if(x)
    {
      send_UART("Received Packet\r", 16);
      send_UART(frame, x - 1);
    }
  }
  return 0;
}
