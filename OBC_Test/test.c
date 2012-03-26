/**
 * @file master.c
 * @brief Main file for master uC.
 * 
 * Contains the scheduler, preflight check routines, high level task blocks like power, control and communication.
 */

#include "common.h"
#include "spi.h"
#include "propagator.h"
#include "twi.h"
#include "timer.h"
#include "peripherals.h"
#include "mag.h"
#include "gps.h"
#include "hm.h"
#include "slave_comm.h"
#include "comm.h"
#include "controller.h"

//Stack Checking

#define STACK_CANARY 0xc5

extern uint8_t _end;
extern uint8_t __stack; 

void StackPaint(void) __attribute__ ((naked)) __attribute__ ((section (".init1")));

void StackPaint(void)
{
#if 0
    uint8_t *p = &_end;

    while(p <= &__stack)
    {
        *p = STACK_CANARY;
        p++;
    }
#else
    __asm volatile ("    ldi r30,lo8(_end)\n"
                    "    ldi r31,hi8(_end)\n"
                    "    ldi r24,lo8(0xc5)\n" /* STACK_CANARY = 0xc5 */
                    "    ldi r25,hi8(__stack)\n"
                    "    rjmp .cmp\n"
                    ".loop:\n"
                    "    st Z+,r24\n"
                    ".cmp:\n"
                    "    cpi r30,lo8(__stack)\n"
                    "    cpc r31,r25\n"
                    "    brlo .loop\n"
                    "    breq .loop"::);
#endif
} 

uint16_t StackCount(void)
{
    const uint8_t *p = &_end;
    uint16_t       c = 0;

    while(*p == STACK_CANARY && p <= &__stack)
    {
        p++;
        c++;
    }

    return c;
} 

//Method 2
extern void __bss_end;
extern void *__brkval;

    int get_free_memory()
    {
      int free_memory;

      if((int)__brkval == 0)
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
      else
        free_memory = ((int)&free_memory) - ((int)__brkval);

      return free_memory;
    }


//Stack checking end

/// @todo Write Error checking codes to ensure the OBC does not go into infinite loops

/**
 * @defgroup global_state Global State parameters
 */
//@{
volatile int8_t GPS_done = -1;
uint8_t Mode;
uint64_t Time;
volatile struct state Current_state;
//@}

/**
 * @brief Communication with Power
 *
 * Obtains the health monitoring data by communcicating with Power microcontroller.
 */
void power(void){
  /// Start watchdog for power tasks
  watch_dog(T_POWER);
  
  ///Every 1.5 minutes get health monitoring data from the power uC
  if(Time % 90 == 0)
  {
    get_HM_data();
    send_preflight("Power\r", 6);
  }
  
}

/**
 * @brief Main function
 */

int main(void){
  //while(1);
  wdt_disable();
  
  ///Configure the Torquer
  configure_torquer();
  char buf[100];
  uint16_t v;
    
  int ao;
  for(ao=0;ao<1;ao++) 
  _delay_ms(1000);  
    
  /// Initialise Interfaces - UART of Magnetometer and GPS and the SPI bus
  init_UART_MM();
  init_UART_GPS();
  init_SPI();
  init_TWI();
  
  send_preflight("Master\r", 7);
  
  ///Set Preflight pin as input
  cbi(DDR_PF, PIN_PF);
  
  ///* Switch on Global interrupts
  sei();
  
  ///Check if Preflight Pin is high
  while(1){
    
    ///* * Reset timer for 2 seconds
    timer_reset_two_sec();
    //get_HM_data();
    //send_preflight("Power\r", 6);
    
    
    ///* Preflight Checks
    if(0){
      ///* * Set the mode as preflight
      Mode = PREFLIGHT;
      /*slave_send(HM_DATA, "Hello", 5);
      _delay_ms(10);
      
      slave_send (BEGIN_TX_GS, NULL, 0);
      
      //power_up_peripheral(PCC);
      _delay_ms(10);
      
      ao = init_CC1020();
      
      if(ao) 
        send_preflight("CC Init\r", 8);
      else
        send_preflight("No Init\r", 8);
      while(1);*/
      /*power_up_peripheral(PGPS);
      while(1) {
        read_GPS();
      while(UCSR0B & _BV(RXCIE0));
      copy_gps_reading();
      sprintf(buf,"%ld %ld %ld\r", Current_state.gps.x/1000, Current_state.gps.y/1000, Current_state.gps.z/1000);
      
      send_preflight(buf, strlen(buf));
      sprintf(buf,"%ld %ld %ld\r", Current_state.gps.v_x/1000, Current_state.gps.v_y/1000, Current_state.gps.v_z/1000);
      
      send_preflight(buf, strlen(buf));
      _delay_ms(1000);
      }*/
      ///* * Magnetometer and Torquer test

	  ///* * Reading with one torquer on at once
      /*Current_state.pwm.x_dir = 0;
      Current_state.pwm.x = 10000;
      Current_state.pwm.y_dir = 0;
      Current_state.pwm.y = 30000;
      Current_state.pwm.z_dir = 1;
      Current_state.pwm.z = 20000;
      send_preflight("Read\r", 5);
      set_PWM();*/
      while(1) {
        read_SS();
        //send_preflight((char *)&Current_state.ss, sizeof(struct SS_reading));
        for(v = 0; v < 6; v++)
        {
          sprintf(buf,"%u\r", (uint16_t)(((float)Current_state.ss.reading[v])*1.6*100/4096));
          send_preflight(buf, strlen(buf));
          sprintf(buf,"%x\r", Current_state.ss.reading[v]);
          send_preflight(buf, strlen(buf));
          
      }
      _delay_ms(1000);
    }
	  ///* * Set Torquer values to zero
      reset_PWM();
      while(1)
        read_MM();

      read_GPS();
      send_preflight((char *)&Current_state.gps, sizeof(struct GPS_reading));
      
      ///* * Sunsensor test
      read_SS();
      send_preflight((char *)&Current_state.ss, sizeof(struct SS_reading));

      ///* Health Montoring
      get_HM_data();
      send_preflight((char *)&Current_state.hm, sizeof(struct HM_data));
	  
      ///Communication Task
      comm();
	  
	  ///* * Wait for 2 seconds to get over
      timer_wait_reset();
    }

  ///Normal Mode
  
    else{
    
      ///* Set default mode of Satellite
      Mode = DETUMBLING;
      
      ///* initialise Timer
      Time = 0;
      
      ///Loop begins
      while(1){
        //Fuses Pain, Nominal Mode checking
        send_preflight("Loop\r", 5);
        control();
        send_preflight("Control\r", 8);
        //power();
        comm();
        send_preflight("Comm\r", 5);
        v = StackCount();
        sprintf(buf, "Stack = %d\r", v);
        send_preflight(buf, strlen(buf));
        v = get_free_memory();
        sprintf(buf, "Stack(Method 2) = %d\r", v);
        send_preflight(buf, strlen(buf));
        Time += FRAME_TIME;
        timer_wait_reset();
      }
    }
  }
  return 0;
}
