#include "common.h"
#include "spi.h"
#include "receive.h"
#include "functions.h"
#include "uart.h"

#define KILL_CODE "Kill"
#define SHUT_CODE "Shut"

int main(void)
{
  uint8_t frame[100], x;
  uint8_t lk, ls;
  
  ls = strlen(SHUT_CODE);
  lk = strlen(KILL_CODE);
  
  init_SPI();
  cc1020_init_dio();
  
  _delay_ms(200);
  
  sbi(DDRD, UPINT);
  sbi(DDRD, UPGPIO);
  cbi(PORTD, UPINT);
  cbi(PORTD, UPGPIO);
  
  cbi(DDRB, 4);
  
  //POWer down CC
  SetupCC1020PD();
  
  do {
    while(!(PINB & _BV(4)));
    _delay_ms(2000);
  }while(!(PINB & _BV(4)));
  
  x = init_CC1020();
  if(x)
    sbi(PORTD, UPGPIO);
  while(1) 
  {
    x = cc1020_receive_packet(frame);
    if(x)
    {
      if(strncmp((char *)(frame + 27), SHUT_CODE, ls) == 0)
      {
        sbi(PORTD, UPINT);
        sbi(PORTD, UPGPIO);
      }
      else if(strncmp((char *)(frame + 27), KILL_CODE, lk) == 0) 
      {
        sbi(PORTD, UPINT);
        cbi(PORTD, UPGPIO);
      }
    }
  }
  return 0;
}
