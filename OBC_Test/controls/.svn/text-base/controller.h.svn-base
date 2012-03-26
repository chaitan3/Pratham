/**
 *@file  controller.h
 *@brief Contains the functions for the Control Law
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define MAG_B 2.5e-5
#define K_DETUMBLING 4e4

#define TOLW_D2N  4e-3
#define TOLW_N2D  10e-3

#define D_TIME 2000
#define N2D_TIME 10000

#define M_MAX 0.95
#define N_TURNS 60
#define AREA 0.0442483613596446
#define PWM_RES 0xFFFF

///Decide
#define I_MAX 1

#include "mathutil.h"

/**
 * @brief Controls codes
 *
 * Performs attitude determination and control.
 * @todo Implement the Control law
 */
void control(void);
void detumbling(vector v_m_D);

#endif
