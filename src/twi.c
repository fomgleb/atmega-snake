#include "twi.h"

#include <avr/io.h>
#include "mem.h"

void
twi_init(void) {
    mem_unset_2_bits(&TWSR, TWPS1, TWPS0);                    /* Set prescaler value to 1 */
    mem_set_byte(&TWBR, F_CPU / (2 * MAX_SCL_FREQUENCY) - 8); /* Set max SCL frequency */
    mem_set_bit(&TWCR, TWEN);                                 /* Enable TWI */
}

static void
wait_for_message_transmission(void) {
    while (mem_get_bit(&TWCR, TWINT) == 0) {}
}

void
twi_start(uint8_t slave_address) {
    mem_set_2_bits(&TWCR, TWINT, TWSTA); /* Send START condition */
    wait_for_message_transmission();
    /* TODO: if TWSR != START then error */
    twi_transmit(slave_address << 1);
    /* TODO: if TWSR != MT_SLA_ACK then error */
}

void
twi_transmit(uint8_t data) {
    mem_set_byte(&TWDR, data);             /* Load data */
    mem_assign_2_bits(&TWCR, TWINT, TWEN); /* Start transmission */
    wait_for_message_transmission();
    /* TODO: if TWSR != MT_DATA_ACK then error */
}

void
twi_stop(void) {
    mem_set_2_bits(&TWCR, TWINT, TWSTO);
}
