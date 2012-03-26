/**
 * @file peripherals.c
 * @brief Read data from the various peripherals and configure the Torquers
 */
#include "peripherals.h"
#include "spi.h"
#include "mag.h" 
#include "sun.h"
#include "gps.h"
#include "hm.h"

uint8_t PWM_init = 0;

void power_up_peripheral(uint8_t device)
{
  ///Switch on the device
  Current_state.hm.LoadStatus |= _BV(device);
  ///Send the load byte
  send_loads();
}
void power_down_peripheral(uint8_t device)
{
  ///Switch off the device
  Current_state.hm.LoadStatus &= ~(_BV(device));
  ///send the load byte
  send_loads();
}

void read_GPS(void){
  ///* Switch on the GPS interrupt
  UCSR0B |= _BV(RXCIE0);
  ///* Switch on the GPS
  // power_up_peripheral(PGPS);
  
}
void read_SS(void){
  
  ///Configure the Sunsensor ADC IC
  configure_SS();
  
  ///Poll the ADC for readings
  poll_SS();
  
}

void read_MM(void){
  
  //power_up_peripheral(PMAG);
  ///Poll the magnetometer for readings
  poll_MM();
  
  //power_down_peripheral(PMAG);
  
}

void configure_torquer(void)
{
  ///Configure the current direction pins as output
  DDRC |= _BV(DDC3) | _BV(DDC4) | _BV(DDC5);
  
  ///Configure the PWM pins as output
  DDRE |= _BV(DDE3) | _BV(DDE4) | _BV(DDE5);
  
  ///Configure PWM: Phase Correct mode Prescalar = 1 Frequency = 122Hz
  TCCR3A = 0;
  TCCR3B = 0;
  TCCR3C = 0;
  TCCR3A |= _BV(COM3A1) | _BV(COM3B1) | _BV(COM3C1) | _BV(WGM31);
  TCCR3B |= _BV(WGM33) | _BV(CS30);
  
  ///Set top as MAX 16-bit
  ICR3 = 0xFFFF;
  OCR3B = 0x0000;
  OCR3B = 0x0000;
  OCR3B = 0x0000;
  PORTC &= ~(_BV(PC3) | _BV(PC4) | _BV(PC5));
}

void reset_PWM(void)
{
  Current_state.pwm.x = 0;
  Current_state.pwm.y = 0;
  Current_state.pwm.z = 0;
  Current_state.pwm.x_dir = 0;
  Current_state.pwm.y_dir = 0;
  Current_state.pwm.z_dir = 0;
  set_PWM();
}

void set_PWM(void)
{
  
  ///If current direction of axis X/Y/Z opposite of default set pin
  if(Current_state.pwm.x_dir)
    PORTC |= _BV(PC5);
  else
    PORTC &= ~(_BV(PC5));
    
  if(Current_state.pwm.y_dir)
    PORTC |= _BV(PC4);
  else
    PORTC &= ~(_BV(PC4));
    
  if(Current_state.pwm.z_dir)
    PORTC |= _BV(PC3);
  else
    PORTC &= ~(_BV(PC3));
    
  ///Set PWM values
  OCR3A = Current_state.pwm.x;
  OCR3B = Current_state.pwm.y;
  OCR3C = Current_state.pwm.z;
}

