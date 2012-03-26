#ifndef RECEIVE_H
#define RECEIVE_H

uint8_t cc1020_receive_packet(uint8_t *data);
void cc1020_init_dio(void);
uint8_t cc1020_receive_bit(void);

#define DIO		PB1	// Connected to DIO pin of CC1020
#define DCLK	PB2	// Connected to DCLK pin of CC1020

#define DDR_RX DDRB
#define PIN_RX PINB

#endif
