#ifndef ATMEGA_SNAKE_TWI_H_
#define ATMEGA_SNAKE_TWI_H_

#include <inttypes.h>
#include <stdbool.h>
#include "mem.h"
#include "real_inline.h"

#define MAX_SCL_FREQUENCY 100000L /* 400kHz I2C clock */

typedef enum {
    BEGIN__START_CONDITION = 0,    /* Started sending start condition */
    END__START_CONDTION,           /* Start condition sent */
    END__REPEATED_START_CONDITION, /* Repeated start condition sent */

    BEGIN__WRITE_TO_SLAVE,            /* Started sending SLA+W ('Write to slave' condition) */
    END__WRITE_TO_SLAVE_SUCCESSFULLY, /* SLA+W sent and ACK (means that slave received the message) received */
    _END_WRITE_TO_SLAVE_UNSUCCESSFULLY,

    BEGIN__SEND_DATA_BYTE,            /* Started sending data byte */
    END__SEND_DATA_BYTE_SUCCESSFULLY, /* Data byte sent and ACK received */
    _END_SEND_DATA_BYTE_UNSUCCESSFULLY,

    _ARBITRATION_LOST,

    // _START_CONDITION_SENT_BIT_INDEX = 0,
    // _REPEATED_START_CONDITION_SENT_BIT_INDEX,
    // _STARTED_WRITING_TO_SLAVE_CONDITION_SENT_BIT_INDEX,
    // _NOT_STARTED_WRITING_TO_SLAVE_BIT_INDEX,
    // _STARTED_SENDING_DATA_BYTE_BIT_INDEX,
    // _DATA_BYTE_SENT_BIT_INDEX,
    // _DATA_BYTE_NOT_SENT_BIT_INDEX,
    // _ARBITRATION_LOST_BIT_INDEX,
} twi_state_flag_t;

extern volatile uint16_t _twi_state;
extern volatile bool _byte_is_set;

#include <util/delay.h>
#include "uart.h"

static REAL_INLINE bool
twi_read_flag_and_clear(twi_state_flag_t twi_state_flag) {
    if (!mem_u16_get_bit(&_twi_state, twi_state_flag)) {
        // uart_send_str("1");
        return false;
    }
    // uart_send_str("2");
    mem_u16_unset_bit(&_twi_state, twi_state_flag);
    return true;
}

// static REAL_INLINE bool
// twi_state_start_condition_sent(void) {
//     return mem_get_bit(&_twi_state, _START_CONDITION_SENT_BIT_INDEX);
// }

// static REAL_INLINE bool
// twi_state_repeated_start_condition_sent(void) {
//     return mem_get_bit(&_twi_state, _REPEATED_START_CONDITION_SENT_BIT_INDEX);
// }

// static REAL_INLINE bool
// twi_state_started_writing_to_slave(void) {

//     return mem_get_bit(&_twi_state, _STARTED_WRITING_TO_SLAVE_CONDITION_SENT_BIT_INDEX);
// }

// static REAL_INLINE bool
// twi_state_not_started_writing_to_slave(void) {
//     return mem_get_bit(&_twi_state, _NOT_STARTED_WRITING_TO_SLAVE_BIT_INDEX);
// }

// static REAL_INLINE bool
// twi_state_started_sending_data_byte(void) {
//     return mem_get_bit(&_twi_state, _STARTED_SENDING_DATA_BYTE_BIT_INDEX);
// }

// static REAL_INLINE bool
// twi_state_data_byte_sent(void) {
//     return mem_get_bit(&_twi_state, _DATA_BYTE_SENT_BIT_INDEX);
// }

// static REAL_INLINE bool
// twi_state_data_byte_not_sent(void) {
//     return mem_get_bit(&_twi_state, _DATA_BYTE_NOT_SENT_BIT_INDEX);
// }

// static REAL_INLINE bool
// twi_state_arbitration_lost(void) {
//     return mem_get_bit(&_twi_state, _ARBITRATION_LOST_BIT_INDEX);
// }

void twi_init(void);
void twi_start_writing_to_slave(uint8_t slave_address);
void twi_send_byte(uint8_t byte_to_send);
void twi_stop(void);

#endif /* ATMEGA_SNAKE_TWI_H_ */
