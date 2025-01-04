#ifndef ATMEGA_SNAKE_SQUARE_H_
#define ATMEGA_SNAKE_SQUARE_H_

#include <inttypes.h>
#include "avr-utils/common/matrix.h"
#include "avr-utils/common/point.h"

typedef struct _square_s_ {
    point_u8_t position; /* Left top corner of the square */
    const point_u8_t min_position;
    const point_u8_t max_position;
    point_i8_t speed;
    uint8_t size;
} square_t;

void sqr_render_to_frame_buffer(matrix_u8_t* frame_buffer, const square_t* square);
void sqr_move(square_t* square);
void sqr_bouncing_move(square_t* square);

#endif // ATMEGA_SNAKE_SQUARE_H_
