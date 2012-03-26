/**
 *@file mag.h
 *@brief Includes various functions for magnetometer.
 */
/**
 * @file mag.h
 * @brief Functions to read the magnetometer
 */

#ifndef MAG_H
#define MAG_H
/**
 * @brief Initializes UART at 9600 bps
 * Uses double baud rate but same bps for slower clocks.
 */
void init_UART_MM(void);

/**
 * @brief Sends magnetometer command
 * @param pointer to data Bytes to be sent
 */
void send_MM_cmd(char *data);

/** @brief Poll the Magnetometer for B readings
*/
void poll_MM(void);

/** @brief Receive byte through UART
 *  @return Received Byte
 */
uint8_t receive_UART(void);

#endif
