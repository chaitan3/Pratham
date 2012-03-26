/**
 * @file master.c
 * @brief Main file for master uC.
 * 
 * Contains the scheduler, preflight check routines, high level task blocks like power, control and communication.
 */

#include "common.h"
#include "spi.h"
#include "timer.h"
#include "peripherals.h"
#include "mag.h"
#include "gps.h"
#include "hm.h"
#include "slave_comm.h"
#include "comm.h"
#include "controller.h"

/// @todo Write Error checking codes to ensure the OBC does not go into infinite loops

/**
 * @defgroup global_state Global State parameters
 */
//@{
volatile uint8_t GPS_done = -1;
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
  }
}

/**
 * @brief Main function
 */

int main(void){
    
  _delay_ms(200);
  init_UART_GPS();
  
  ///Configure the magnetometer
  //configure_MM();
  
  ///Configure the Torquer
  configure_torquer();
  
  ///Set Preflight pin as input
  //cbi(DDR_PF, PIN_PF);
  
  send_preflight("Master\n", 7);
  
  Current_state.pwm.x_dir = 0;
  Current_state.pwm.x = 20000;
  Current_state.pwm.y_dir = 0;
  Current_state.pwm.y = 20000;
  Current_state.pwm.z_dir = 1;
  Current_state.pwm.z = 0xffff - 20000;
  set_PWM ();
  
  while(1);
  
  return 0;
}
