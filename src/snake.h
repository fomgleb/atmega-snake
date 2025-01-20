#ifndef ATMEGA_SNAKE_SNAKE_H_
#define ATMEGA_SNAKE_SNAKE_H_

#include <stdbool.h>
#include "avr-utils/common/matrix.h"
#include "avr-utils/common/point.h"
#include "direction.h"

typedef struct {
    matrix_u8_t flesh_chunks; /* 0 - no flesh chunk; 1 - flesh chunk */
    matrix_u16_t directions;  /* Matrix of direction_t */
    uint8_t length;
    point_u8_t head_position;
    point_u8_t tail_position;
} snake_t;

snake_t snk_create(point_u8_t start_postion, uint8_t length, point_u8_t board_size, uint8_t scale);
void snk_set_position(snake_t* snake, point_u8_t head_position, direction_t direction, uint8_t length);
void snk_delete(snake_t* snake);
void snk_move(snake_t* snake, direction_t direction, point_u8_t food_position, bool* collided, bool* ate_food);
void snk_render(matrix_u8_t* frame_buffer, const snake_t* snake);

#endif /* ATMEGA_SNAKE_SNAKE_H_ */
