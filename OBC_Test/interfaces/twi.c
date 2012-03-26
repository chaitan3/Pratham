/**
 * @file twi.c
 * @brief code for a TWI master
 * @created Feb 20, 2010
 * @author Chiraag Juvekar
 */

#include "twi.h"
#include <util/twi.h>

void init_TWI(void) {
  /// <b>Execution</b> <br><br>
  ///Initialize TWI clock: 50 kHz clock, TWPS = 0 => prescaler = 1 
  TWSR = 0;
  TWCR = 0;
  TWBR = 72;
}

int TWI_rec_byte(uint8_t addr, uint8_t *buf){

  ///Transmit Address
  do{						
    ///* send start cond.
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
  }while(TW_STATUS == TW_MT_ARB_LOST);

  if ( (TW_STATUS != TW_REP_START) && (TW_STATUS != TW_START)) return -1;

  TWDR = (addr << 1) + TW_READ;
  TWCR = (1 << TWINT) | (1 << TWEN);

  while (!(TWCR & (1 << TWINT)));
  if (TW_STATUS == TW_MR_SLA_NACK) {TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); return 1;}
  if (TW_STATUS == TW_MR_ARB_LOST) return -2;
  if (TW_STATUS != TW_MR_SLA_ACK) {TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); return -1;}

  _delay_us(50);

  ///Receive Data
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  switch (TW_STATUS) {
  case TW_MR_DATA_NACK: // FALLTHROUGH
  case TW_MR_DATA_ACK:
    *buf = TWDR;
    break;
  default:
    {TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); return -1;}
  }

  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); return 1;
}

int TWI_send_byte(uint8_t addr, uint8_t data){
  ///Transmit Address
  do{						\
    /// Send start cond.
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
  }while(TW_STATUS == TW_MT_ARB_LOST);
  if ( (TW_STATUS != TW_REP_START) && (TW_STATUS != TW_START)) return -1;


  TWDR = (addr << 1) + TW_WRITE;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  if (TW_STATUS == TW_MT_SLA_NACK) {TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); return 1;}
  if (TW_STATUS == TW_MT_ARB_LOST) return -2;
  if (TW_STATUS != TW_MT_SLA_ACK) {TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); return -1;}

  _delay_us(50);

  ///Transmit Data
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  if (TW_STATUS != TW_MT_DATA_ACK) {TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); return -1;}

  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); return 1;
}

