/**
 *@file  common.h
 *@brief Contains the various constant/macro/struct definitions, clock frequency and general functions.
*/

#ifndef COMMON_H
#define COMMON_H

///CPU frequency
#define F_CPU 8000000
#define NULL 0

#include <math.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>


#define sbi(x,y)	(x |= (1<<y))
#define cbi(x,y)	(x &= ~(1<<y))
#define tbi(x,y)	(x ^= (1<<y))

#define TRUE 1
#define FALSE 0

/** 
 * @defgroup Modes
 * @brief Modes of operation of satellite
 * @todo Safe and emergency mode details to be obtained and coded
 */
//@{
/// During preflight checks
#define PREFLIGHT 0
/// Normal Mode
#define NOMINAL 1
/// Low power Mode
#define SAFE 2
/// Failure Mode
#define EMERGENCY 3
/// Detumbling Mode
#define DETUMBLING 4
//@}

/**
 *@defgroup Preflight_check
 *@todo Check port and pin for preflight checks 
 */
//@{
///Port for preflight checks
#define DDR_PF DDRD
#define PORT_PF PIND
/// Pin to check for preflight checks mode
#define PIN_PF PD6
//@}


///Frame Time for the main loop
#define FRAME_TIME 2
#endif
