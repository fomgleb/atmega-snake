#include "twi.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include "mem.h"

volatile uint16_t _twi_state = 0x00;
volatile uint8_t _slave_address = 0x00;
volatile uint8_t _byte_to_send = 0x00;
volatile bool _byte_is_set = false;

typedef enum {
    START_CONDITION_TRANSMITTED = 0x08,
    REPEATED_START_CONDITION_TRANSMITTED = 0x10,
    WRITE_TO_SLAVE_TRANSMITTED_AND_ACK_RECEIVED = 0x18,
    WRITE_TO_SLAVE_TRANSMITTED_AND_NOTACK_RECEIVED = 0x20,
    DATA_BYTE_TRANSMITTED_AND_ACK_RECEIVED = 0x28,
    DATA_BYTE_TRANSMITTED_AND_NOTACK_RECEIVED = 0x30,
    ARBITRATION_LOST = 0x38,
} status_code_t;

static REAL_INLINE void
send_start_condition(void) {
    mem_unset_bit(&TWCR, TWSTO);
    mem_set_2_bits(&TWCR, TWINT, TWSTA); /* Send START condition */
}

static REAL_INLINE void
send_start_writing_to_slave(uint8_t slave_address) {
    mem_set_byte(&TWDR, slave_address << 1); /* Load data: slave_address */
    mem_unset_2_bits(&TWCR, TWSTA, TWSTO);
    mem_set_bit(&TWCR, TWINT); /* Start transmission */
}

static REAL_INLINE void
send_byte(uint8_t byte_to_send) {
    TWDR = byte_to_send; /* Load data */
    // uart_send_str("ura");
    mem_unset_2_bits(&TWCR, TWSTA, TWSTO);
    mem_set_bit(&TWCR, TWINT); /* Start transmission */
}

void
twi_init(void) {
    mem_unset_2_bits(&TWSR, TWPS1, TWPS0);                    /* Set prescaler value to 1 */
    mem_set_byte(&TWBR, F_CPU / (2 * MAX_SCL_FREQUENCY) - 8); /* Set max SCL frequency */
    mem_set_2_bits(&TWCR, TWEN, TWIE);                        /* TWEN - Enable TWI; TWIE - Enable interrupts */
}

void
twi_start_writing_to_slave(uint8_t slave_address) {
    mem_u16_assign_bit(&_twi_state, BEGIN__START_CONDITION);

    _slave_address = slave_address;
    send_start_condition();
}

void
twi_send_byte(uint8_t byte_to_send) {
    _byte_to_send = byte_to_send;
    // _byte_is_set;
}

#include <util/delay.h>
#include "uart.h"

void
twi_stop(void) {
    mem_unset_bit(&TWCR, TWSTA);
    mem_set_2_bits(&TWCR, TWINT, TWSTO);
}

ISR(TWI_vect) {
    volatile status_code_t twi_status = TWSR & ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3));
    switch (twi_status) {
        case START_CONDITION_TRANSMITTED:
            // uart_send_str("END__START_CONDTION, BEGIN__WRITE_TO_SLAVE");
            mem_u16_assign_2_bits(&_twi_state, END__START_CONDTION, BEGIN__WRITE_TO_SLAVE);
            send_start_writing_to_slave(_slave_address);
            return;
        case REPEATED_START_CONDITION_TRANSMITTED:
            // uart_send_str("END__REPEATED_START_CONDITION, BEGIN__WRITE_TO_SLAVE");
            mem_u16_assign_2_bits(&_twi_state, END__REPEATED_START_CONDITION, BEGIN__WRITE_TO_SLAVE);
            send_start_writing_to_slave(_slave_address);
            return;
        case WRITE_TO_SLAVE_TRANSMITTED_AND_ACK_RECEIVED:
            // uart_send_str("END__WRITE_TO_SLAVE_SUCCESSFULLY, BEGIN__SEND_DATA_BYTE");
            mem_u16_assign_2_bits(&_twi_state, END__WRITE_TO_SLAVE_SUCCESSFULLY, BEGIN__SEND_DATA_BYTE);
            send_byte(_byte_to_send);
            return;
        case WRITE_TO_SLAVE_TRANSMITTED_AND_NOTACK_RECEIVED:
            // mem_set_bit(&_twi_state, _NOT_STARTED_WRITING_TO_SLAVE_BIT_INDEX);
            return;
        case DATA_BYTE_TRANSMITTED_AND_ACK_RECEIVED:
            // uart_send_str("END__SEND_DATA_BYTE_SUCCESSFULLY, BEGIN__SEND_DATA_BYTE");
            // uart_send_str("da");
            mem_u16_assign_2_bits(&_twi_state, END__SEND_DATA_BYTE_SUCCESSFULLY, BEGIN__SEND_DATA_BYTE);
            send_byte(_byte_to_send);
            return;
            // case DATA_BYTE_TRANSMITTED_AND_NOTACK_RECEIVED: break;
            // case ARBITRATION_LOST: mem_set_bit(&_twi_state, _ARBITRATION_LOST_BIT_INDEX); break;

        default: uart_send_str("wtf"); break;
    }
    // if (twi_status == 0) {
    //     _delay_ms(10);
    //     uart_transmit('s');
    //     uart_transmit('\n');
    //     uart_transmit('\r');
    // }
    // mem_unset_bit(&TWCR, TWIE);
    // mem_set_bit(&TWCR, TWINT); /* Interrupt is completed */
    // TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
    // sei();
    // cli();
}
