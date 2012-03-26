/**
 *@file  igrf.h
 *@brief Contains the various constant/macro/struct definitions and functions for Magnetic Field Estimation
*/

#ifndef IGRF_H
#define IGRF_H

#define RE 6371.2
#define A2 40680631.59
#define B2 40408299.98
#define IGRF_YEAR 2010

void igrf(vector v_lla, float years, uint8_t order, vector v_B_ned);

#endif
