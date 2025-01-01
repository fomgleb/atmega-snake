#ifndef ATMEGA_SNAKE_TWI_H_
#define ATMEGA_SNAKE_TWI_H_

#include <inttypes.h>

#define MAX_SCL_FREQUENCY TWI_SPEED /* 400kHz I2C clock */

void twi_init(void);
void twi_start(uint8_t slave_address);
void twi_transmit(uint8_t data);
void twi_stop(void);

#endif /* ATMEGA_SNAKE_TWI_H_ */
