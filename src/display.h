#ifndef ATMEGA_SNAKE_DISPLAY_H_
#define ATMEGA_SNAKE_DISPLAY_H_

#include <stdbool.h>
#include "bytes_matrix.h"

typedef struct {
    bytes_matrix_t _frame_buffer;
    bool _is_rendered;
} display_t;

display_t dpl_create(uint8_t width, uint8_t height);
void dpl_delete(const display_t* display);

static REAL_INLINE bool
dpl_is_rendered(const display_t* display) {
    return display->_is_rendered;
}

#endif /* ATMEGA_SNAKE_DISPLAY_H_ */
