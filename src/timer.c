#include "timer.h"

#include <avr/io.h>
#include "avr-utils/common/mem.h"

void
tmr1_init(void) {
    /* Set timer/counter mode of operation to CTC (clear timer on compare match) */
    mem_unset_bit(&TCCR1B, WGM13);
    mem_set_bit(&TCCR1B, WGM12);
    mem_unset_2_bits(&TCCR1A, WGM11, WGM10);

    mem_unset_2_bits(&TCCR1A, COM1A1, COM1A0); /* Set normal port operation, OC1A disconnected */
    mem_unset_2_bits(&TCCR1A, COM1B1, COM1B0); /* Set normal port operation, OC1B disconnected */

    /* Set prescaler to 1024 */
    mem_unset_bit(&TCCR1B, CS11);
    mem_set_2_bits(&TCCR1B, CS12, CS10);

    mem_set_bit(&TIMSK1, OCIE1A); /* Enable interrupts */

    OCR1A = F_CPU / 4096; /* Start timer/counter */
}
