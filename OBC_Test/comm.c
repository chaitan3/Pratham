/**
 * @file comm.c
 * @brief Communication Check Routine for Master
 * 
 */
#include "common.h"
#include "comm.h"
#include "mathutil.h"
#include "timer.h"
#include "slave_comm.h"
#include "spi.h"
#include "peripherals.h"

extern vector v_sat;

/** @brief Check whether the Satellite is over India/France/GroundStation
 *  @return Position of Satellite
 */
uint8_t check_satellite_position(void){
  
  ///Calculate latitude and longitude values of the satellite
  float latitude = v_sat[0] * (180 / M_PI);
  float longitude = v_sat[1] * (180 / M_PI);
  float altitude = v_sat[2] / 1000;
  
  float dx, dy, dis_sq, thres;
  
  ///In Preflight Mode return position as GS
  if(Mode == PREFLIGHT)
    return GS;
  
  ///Calculate PHI
  thres = 60 - (asin(5542.6 / (6400 + altitude)) * (180 / M_PI));
  if(thres < PHI_MIN_ANGLE)
    thres = PHI_MIN_ANGLE;
  
  /// Check whether over GS and do the same as above
  dx = latitude - GS_LAT;
  dy = longitude - GS_LON;
  ///Calculate distance and check whether it is below a certain threshold
  dis_sq = pow(dx,2) + pow (dy,2);
  
  if(dis_sq < thres)
    return GS;
  
  /// Check whether over India
  dx = latitude - IN_LAT;
  dy = longitude - IN_LON;
  dis_sq = pow(dx,2) + pow (dy,2);
  
  if(dis_sq < 15 + thres)
    return IN;
  
  ///Check whether over France and do the same as above
  dx = latitude - FR_LAT;
  dy = longitude - FR_LON;
  dis_sq = pow(dx,2) + pow (dy,2);
  if(dis_sq < pow(FR_RAD,2))
    return FR;
  
  ///Return that not over any required GS
  return 0;
}

/**
 * @brief Communcation Tasks
 *
 * Performs the various tasks to be performed for communcation
 * to ground station including configuring CC1020 through SPI interface
 * and ordering slave microcontroller to start transfer of appropriate data
 * when over GS, India, or France as per the lat/long values. It uses the
 * @ref check_satellite_position function.
 */
void comm(void){
  
  static uint8_t Transmission = 0, downlink_time = 0;
  uint8_t sat_pos;
  
  ///Set watchdog timer for the task.
  watch_dog(T_COMM);
  
  if((GPS_done < 0) && (Mode != PREFLIGHT)) 
  
    return;
  
  send_preflight("Entered Comm\r", 13);
  
  ///Send data to slave every 1.5 minutes
  if(Time % 90 == 0)
  {
    slave_send (HM_DATA, (char *)&Current_state, sizeof (struct state));
    ///* Delay for slave SPI processing
    _delay_ms(10);
  }
  else if((Time % 10 == 0) && ((Transmission == FR) || (Transmission == IN)))
  {
    slave_send (REAL_TIME, (char *)&Current_state, sizeof (struct state));
    ///* Delay for slave SPI processing
    _delay_ms(10);
  }
  
  ///16 second slot for uplink
  if(Transmission)
  {
    downlink_time += FRAME_TIME;
    if(downlink_time == 6)
    {
      ///* Stop transmission
      slave_send (END_TX, NULL, 0);
      
      ///* Wait for Acknoledgement
      while(SPI_transfer(0xFF) != ACK);
      
      ///* Power off the CC1020
      power_down_peripheral(PCC);
    }
    else if(downlink_time == 8)
    {
      downlink_time = 0;
      if(Transmission == GS)
      {
        ///* Start transmission
        slave_send (BEGIN_TX_GS, NULL, 0);
        
        ///* Power on the CC1020
        power_up_peripheral(PCC);
        _delay_ms(10);
      
        ///* Initialise CC1020
        init_CC1020();
      }
      else
      {
        ///* Start transmission
        slave_send (BEGIN_TX_COMM, NULL, 0);
        
        ///* Power on the CC1020
        power_up_peripheral(PCC);
        _delay_ms(10);
      
        ///* Initialise CC1020
        init_CC1020();
      }
    } 
  }
    
  sat_pos = check_satellite_position();
  if((sat_pos != Transmission) && (Mode == NOMINAL))
  {
    downlink_time = 0;
    
    if(sat_pos == 0)
    {
      ///* Stop transmission
      slave_send (END_TX, NULL, 0);
      
      ///* Wait for Acknoledgement
      while(SPI_transfer(0xFF) != ACK);
      
      ///* Power off the CC1020
      power_down_peripheral(PCC);
    }
    else if(sat_pos == GS)
    {
      ///* Start transmission
      slave_send (BEGIN_TX_GS, NULL, 0);
      
      ///* Power on the CC1020
      power_up_peripheral(PCC);
      _delay_ms(10);
      
      ///* Initialise CC1020
      init_CC1020();
    }
    else
    {
      ///* Start transmission
      slave_send (BEGIN_TX_COMM, NULL, 0);
      
      ///* Power on the CC1020 and wait for response
      power_up_peripheral(PCC);
      _delay_ms(10);
      
      ///* Initialise CC1020
      init_CC1020();
    }
  }
  
  Transmission = sat_pos;
}
