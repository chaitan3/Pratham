#include "common.h"
#include "timer.h"
/**
 * @file timer.c
 * @brief Implement the 2-second Time frame for the main loop using a timer
 */

///@todo verify the timer code

void timer_reset_two_sec(void){
  ///@todo Confirm the configuration of the Timer
  
  ///* Clear Timer Compare Flag
  sbi(TIFR, OCF1A);
  
  ///* Stop Timer
  TCCR1B = 0x00;
  
  ///* Reset Count
  TCNT1H = 0;
  TCNT1L = 0;
  
  ///* Output compare match value 8 MHz clock
  OCR1A = TIMER_TWO_SEC;
  
  ///* Start Timer
  TCCR1A = 0x00; 
  TCCR1B = 0x05;
}

void watch_dog(int time){
  /*wdt_reset();
  wdt_disable();
  _delay_us(100);
  wdt_enable(time);*/
}

void timer_wait_reset(void){
  _delay_ms(10);
  watch_dog(T_WAIT);
  ///* Return when timer>2 seconds.
  while(!(TIFR & _BV(OCF1A)));
  ///* Restart timer
  timer_reset_two_sec();  
}
