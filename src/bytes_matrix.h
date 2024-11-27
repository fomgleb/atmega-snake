#ifndef ATMEGA_SNAKE_BYTES_MATRIX_H_
#define ATMEGA_SNAKE_BYTES_MATRIX_H_

#include <inttypes.h>
#include "real_inline.h"

#define _BYTE_IN_MATRIX(bytes_matrix, x, y) bytes_matrix->_arr[y * bytes_matrix->width + x]

typedef struct {
    uint8_t* const _arr;
    const uint8_t width;
    const uint8_t height;
} bytes_matrix_t;

bytes_matrix_t bm_create(uint8_t width, uint8_t height);
void bm_delete(const bytes_matrix_t* bytes_matrix);

static REAL_INLINE void
bm_assign_byte(const bytes_matrix_t* bytes_matrix, uint8_t x, uint8_t y, uint8_t new_byte) {
    _BYTE_IN_MATRIX(bytes_matrix, x, y) = new_byte;
}

static REAL_INLINE void
bm_operation_or(const bytes_matrix_t* bytes_matrix, uint8_t x, uint8_t y, uint8_t value) {
    _BYTE_IN_MATRIX(bytes_matrix, x, y) |= value;
}

static REAL_INLINE void
bm_clear(const bytes_matrix_t* bytes_matrix) {
    for (uint8_t x = 0; x < bytes_matrix->width; x++) {
        for (uint8_t y = 0; y < bytes_matrix->height; y++) {
            bm_assign_byte(bytes_matrix, x, y, 0);
        }
    }
}

static REAL_INLINE uint8_t
bm_get_byte(const bytes_matrix_t* bytes_matrix, uint8_t x, uint8_t y) {
    return _BYTE_IN_MATRIX(bytes_matrix, x, y);
}

#endif /* ATMEGA_SNAKE_BYTES_MATRIX_H_ */
