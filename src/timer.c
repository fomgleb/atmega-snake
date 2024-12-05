#include "timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include "mem.h"

void
timer1_init(void) {
    // Set Timer1 in CTC mode
    TCCR1A = 0;            // Normal operation, WGM bits in TCCR1B control the mode
    TCCR1B = (1 << WGM12); // Set WGM12 bit for CTC mode

    // Set the compare match value for 0.1-second interrupts
    OCR1A = 6250 - 1; // Compare match value (zero-indexed)

    // Set the prescaler to 256
    TCCR1B |= (1 << CS12); // CS12 = 1, CS11 = 0, CS10 = 0 (Prescaler = 256)

    // Enable Timer1 Compare Match A interrupt
    TIMSK1 = (1 << OCIE1A);

    // Enable global interrupts
    sei();
}

void
tmr_1_init(void) {
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

    OCR1A = F_CPU / 1024; /* Start timer/counter */
}
