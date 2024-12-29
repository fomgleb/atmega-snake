#ifndef ATMEGA_SNAKE_LEG_H_
#define ATMEGA_SNAKE_LEG_H_

#include <inttypes.h>
#include <stdbool.h>
#include "real_inline.h"

typedef struct {
    const volatile uint8_t* const ddr;
    volatile uint8_t* const port;
    const volatile uint8_t leg_index;
} output_leg_t;

typedef struct {
    const volatile uint8_t* const ddr;
    const volatile uint8_t* const port;
    const volatile uint8_t* const pin;
    const volatile uint8_t leg_index;
} input_leg_t;

output_leg_t leg_create_output_leg(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t leg_index);
input_leg_t leg_create_input_leg(volatile uint8_t* ddr, volatile uint8_t* port, const volatile uint8_t* pin,
                                 volatile uint8_t leg_index);

static REAL_INLINE void
leg_set(output_leg_t* leg) {
    *leg->port |= (1 << leg->leg_index);
}

static REAL_INLINE void
leg_drop(output_leg_t* leg) {
    *leg->port &= ~(1 << leg->leg_index);
}

static REAL_INLINE void
leg_drop_set(output_leg_t* leg) {
    leg_drop(leg);
    leg_set(leg);
}

static REAL_INLINE void
leg_set_drop(output_leg_t* leg) {
    leg_set(leg);
    leg_drop(leg);
}

static REAL_INLINE void
leg_drop_set_drop(output_leg_t* leg) {
    leg_drop(leg);
    leg_set(leg);
    leg_drop(leg);
}

static REAL_INLINE bool
leg_get(const input_leg_t* leg) {
    return *leg->pin & (1 << leg->leg_index);
}

#endif /* ATMEGA_SNAKE_LEG_H_ */
