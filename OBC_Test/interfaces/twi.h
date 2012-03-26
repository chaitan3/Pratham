/**
 * @file twi.h
 * @brief TWI/I2C routines
 */
#include "common.h"

/**
 * @brief Initiates TWI/I2C interface
 */
void init_TWI(void);

/**
 * @brief Recieves a byte on TWI/I2C interface.
 * @param addr Address of peripheral on I2C
 * @param *buf Address to store data at
 */
int TWI_rec_byte(uint8_t addr, uint8_t *buf);

/**
 * @brief Sends a byte on TWI/I2C interface.
 * @param addr Address of peripheral on I2C
 * @param data Data to send
 */
int TWI_send_byte(uint8_t addr, uint8_t data);
