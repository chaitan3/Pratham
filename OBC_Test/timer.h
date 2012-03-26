#ifndef TIMER_H
#define TIMER_H
/**
 * @file timer.h
 * @brief Timer functions
 * All functions related to frame timer and watchdog.
 */

/** 
 * @defgroup watchdog
 * Timer values for watchdog for each module/task. 
 * @todo Decide timing for watchdog for each task 
 */
//@{
#define T_CONTROL WDTO_1S
#define T_POWER WDTO_500MS
#define T_COMM WDTO_1S
#define T_WAIT WDTO_2S
//@}

///Compare value for two seconds, prescalar 1024
#define TIMER_TWO_SEC 15624

/**
 * @brief  Resets the timer for 2 seconds.
 */
void timer_reset_two_sec(void);

/**
 *  @brief  Initiates watchdog with timer value corresponding to time parameter.
 *  @param  time value to set watchdog timer to.  
 */
void watch_dog(int time);

/**
 *  @brief  Waits for the 2 second timer to finish and then resets it.
 */
void timer_wait_reset(void);
#endif
