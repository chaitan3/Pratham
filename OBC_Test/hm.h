/**
 *@file hm.h
 *@brief This file contains definition of functions that are required for health monitoring data acquisition.
 */

#ifndef HM_H
#define HM_H
#include "common.h"

/** @brief Captures load bytes as obtained from power for health monitoring.
 */
struct HM_data{
  uint8_t LoadStatus;
  uint8_t BatteryCurrent;
  uint8_t BatteryVoltage;
  uint8_t Panel;
  uint8_t SixVolt;
  uint8_t ThreeVolt;
  uint8_t BatteryStatus;
};

/** @brief Get the health monitoring data from the Power Microcontroller
 *  using the I2C interface.
 */
void get_HM_data(void);

/** @brief Send the CommandByte to power uC after suitable modifications
 */
void send_loads(void);

/// I2C slave address of the power MuC 
#define ADDR 0x22

/** @defgroup Load Status Bits
 */ 
//@{
#define BEACONPOWER  7
#define PCONTROL 6
#define PGPS  5
#define PCC 4
#define POBC 3
#define PMAG 2
//@}

#endif
