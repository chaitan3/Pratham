/**
 *@file  frame.h
 *@brief Contains the various constant/macro/struct definitions and functions for frame transformations
*/

#ifndef FRAME_H
#define FRAME_H
#include "mathutil.h"

#define STPERUT 1.00273790935
#define W_EARTH_ROT 7.2722e-5

#define A 6378137.0
#define B 6356752.314245
#define F 0.003352811
#define E2 0.006694381
#define EP2 0.006739498 

uint64_t days_in_months(uint8_t month);

uint64_t get_seconds_since_pivot(uint16_t year, uint8_t month, uint8_t date, uint8_t hours, uint8_t minutes, uint8_t seconds);

void get_seconds_since_equinox(void);

void eci2ecef(vector v_eci, vector v_ecef);

void ecef2eci(vector v_ecef, vector v_eci);

void eci2orbit(vector v_r, vector v_v, vector v_eci, vector v_orbit);

void ecef2lla(vector v_ecef, vector v_lla);

void ned2ecef(vector v_ned, vector v_lla, vector v_ecef);

extern uint64_t seconds_since_equinox, seconds_since_pivot;

#endif
