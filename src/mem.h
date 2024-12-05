#ifndef ATMEGA_SNAKE_MEM_H_
#define ATMEGA_SNAKE_MEM_H_

#include <stdbool.h>
#include "real_inline.h"

static REAL_INLINE void
mem_set_byte(volatile uint8_t* memory_byte, uint8_t byte_value) {
    *memory_byte = byte_value;
}

static REAL_INLINE void
mem_set_bit(volatile uint8_t* memory_byte, uint8_t bit_index) {
    *memory_byte |= (1 << bit_index);
}

static REAL_INLINE void
mem_set_2_bits(volatile uint8_t* memory_byte, uint8_t bit1_index, uint8_t bit2_index) {
    *memory_byte |= (1 << bit1_index) | (1 << bit2_index);
}

static REAL_INLINE void
mem_assign_bit(volatile uint8_t* memory_byte, uint8_t bit_index) {
    *memory_byte = 1 << bit_index;
}

static REAL_INLINE void
mem_assign_2_bits(volatile uint8_t* memory_byte, uint8_t bit1_index, uint8_t bit2_index) {
    *memory_byte = (1 << bit1_index) | (1 << bit2_index);
}

static REAL_INLINE void
mem_assign_3_bits(volatile uint8_t* memory_byte, uint8_t bit1_index, uint8_t bit2_index, uint8_t bit3_index) {
    *memory_byte = (1 << bit1_index) | (1 << bit2_index) | (1 << bit3_index);
}

static REAL_INLINE void
mem_unset_bit(volatile uint8_t* memory_byte, uint8_t bit_index) {
    *memory_byte &= ~(1 << bit_index);
}

static REAL_INLINE void
mem_unset_2_bits(volatile uint8_t* memory_byte, uint8_t bit1_index, uint8_t bit2_index) {
    *memory_byte &= ~((1 << bit1_index) | (1 << bit2_index));
}

static REAL_INLINE void
mem_unset_3_bits(volatile uint8_t* memory_byte, uint8_t bit1_index, uint8_t bit2_index, uint8_t bit3_index) {
    *memory_byte &= ~((1 << bit1_index) | (1 << bit2_index) | (1 << bit3_index));
}

static REAL_INLINE uint8_t
mem_get_bit(volatile uint8_t* memory_byte, uint8_t bit_index) {
    return *memory_byte & (1 << bit_index);
}

static REAL_INLINE void
mem_u16_assign_bit(volatile uint16_t* two_bytes, uint8_t bit_index) {
    *two_bytes = 1 << bit_index;
}

static REAL_INLINE void
mem_u16_assign_2_bits(volatile uint16_t* two_bytes, uint8_t bit1_index, uint8_t bit2_index) {
    *two_bytes = (1 << bit1_index) | (1 << bit2_index);
}

static REAL_INLINE void
mem_u16_unset_bit(volatile uint16_t* two_bytes, uint8_t bit_index) {
    *two_bytes &= ~(1 << bit_index);
}

static REAL_INLINE bool
mem_u16_get_bit(volatile uint16_t* two_bytes, uint8_t bit_index) {
    return *two_bytes & (1 << bit_index);
}

#endif /* ATMEGA_SNAKE_MEM_H_ */
