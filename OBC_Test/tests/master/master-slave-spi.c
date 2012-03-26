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
  /// Initialise Interfaces - UART of Magnetometer and GPS and the SPI bus
  //init_UART_MM();
  init_UART_GPS();
  init_SPI();
  
  ///Configure the magnetometer
  //configure_MM();
  
  ///Configure the Torquer
  //configure_torquer();
  
  ///Set Preflight pin as input
  //cbi(DDR_PF, PIN_PF);
  
  send_preflight("Master", 6);
  
  slave_send(BEGIN_TX_COMM, "Data", 4);
  
  _delay_ms(50);
  
  slave_send(BEGIN_TX_COMM, "Data2", 4);
  
  while(1);
  
  return 0;
}
