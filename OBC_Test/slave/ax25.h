#ifndef AX25_H
#define AX25_H
#include "peripherals.h"

void make_ax25_frame_from_data(uint8_t *frame, uint8_t *data);
uint16_t calculate_crc_16(uint8_t *data, uint8_t size);
uint16_t update_crc_16(uint8_t byte, uint16_t crc);

#define AX_FLAG 0x7E
#define AX_ADDR_SIZE 6
#define AX_CONTROL 0x03
#define AX_PID 0xF0

#define CRC_CCIT_REV 0x8408


#define HM_SIZE 5
#define FRAME_SIZE (HM_SIZE + 4 + 27)
#endif
