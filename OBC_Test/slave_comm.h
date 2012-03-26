/**
 * @file slave_comm.h 
 * @brief Master slave communication
 *
 * Contains all information/functions used to communicate between master and slave muCs.
 */

#ifndef SLAVE_COMM_H
#define SLAVE_COMM_H
#include "common.h"
/**
 * Data packet from master:
 * Command
 * <size of transmission>
 * Data
 * END_SPI
 * END_SPI
 */

/**
 * @defgroup slave_commands
 * @brief Commands from master to slave 
 */
//@{
/// Health Monitoring Data Transmission
#define HM_DATA          0x00
/// Real Time HM Data
#define REAL_TIME        0b11010101
/// Begin transmission to GS
#define BEGIN_TX_GS      0b01010101
/// Begin transmission but not the one for GS
#define BEGIN_TX_COMM    0b01011010
/// End transmission
#define END_TX           0b00110011
//@}

/// Flags
#define END_SPI          0b10101010
#define N_END_SPI 2
#define START_SPI        0b11100111
#define N_START_SPI 2
#define ACK              0b10010010

/**
 * @brief Send data to slave
 * @param command Command to be sent
 * @param buffer String pointer
 * @param size Size of string/packet
 */
void slave_send (uint8_t command, char* buffer, uint8_t size);


#endif
