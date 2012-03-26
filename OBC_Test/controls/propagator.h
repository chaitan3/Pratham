/**
 *@file  propagator.h
 *@brief Functions and constants for SGP propagator
*/

#ifndef PROP_H
#define PROP_H
#include "mathutil.h"

#define R_E2 40680631590769.0
#define J2 1.08263e-3
#define GM 3.98658366e+14
#define EPSILON 0.4102
#define SECONDS_IN_YEAR 31536000.0

void copy_gps_reading(void);

void sgp_get_acceleration(vector v_g);

void sgp_orbit_propagator(void);

void sun_vector_estimator(vector v_sun_o);

void magnetic_field_estimator(vector v_B_o);

#endif
