#ifndef RECEIVE_H
#define RECEIVE_H

uint8_t cc1020_receive_packet(uint8_t *data);
void cc1020_init_dio(void);
uint8_t cc1020_receive_bit(void);

#define DIO		PD6	// Connected to DIO pin of CC1020
#define DCLK	PD3	// Connected to DCLK pin of CC1020

#define DDR_RX DDRD
#define PIN_RX PIND

#define UPINT 1
#define UPGPIO 0

#endif
