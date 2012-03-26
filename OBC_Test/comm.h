/**
 * @file comm.h
 * @brief Header for Communication Check Routine for Master
 * 
 */
#ifndef COMM_H
#define COMM_H


/** 
 * @defgroup TX_region Transmission Regions
 * @brief Defining centre and radius for communication check
 */
//@{
#define IN_LAT 22.5833
#define IN_LON 82.7666
#define IN_RAD 15
#define FR_LAT 48.861101
#define FR_LON 2.352104
#define FR_RAD 6
#define GS_LAT 19.133167
#define GS_LON 72.915144
#define PHI_MIN_ANGLE 6
#define IN 0xAA
#define GS 0xBB
#define FR 0xCC
//@}

uint8_t check_satellite_position(void);
void comm(void);

#endif
