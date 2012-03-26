/** @file hm.c 
 *  @brief contains code for collecting health monitoring data and sending the OBC Command Byte
 */
#include "hm.h"
#include "twi.h"
#include "peripherals.h"

void get_HM_data(void)
{
  ///Counter for the 7 bytes of Health monitoring data
  uint8_t HMCounter;
  
  for(HMCounter = 0; HMCounter < 7; HMCounter++)
    ///* Receive each of the bytes by sending a write request to the power muc.
    ///* data stored in the hm structure with the same ordering as that in the power muc
    TWI_rec_byte(ADDR, ((uint8_t *) &Current_state.hm) + HMCounter);
}


void send_loads(void){
  /// Send the load byte to Power uC using the i2c interface
  TWI_send_byte(ADDR, Current_state.hm.LoadStatus);
}

