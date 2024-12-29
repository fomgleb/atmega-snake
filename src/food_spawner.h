#ifndef ATMEGA_SNAKE_FOOD_SPAWNER_H_
#define ATMEGA_SNAKE_FOOD_SPAWNER_H_

#include "snake.h"

point_u8_t fs_get_random_food_position(const snake_t* snake);
void fs_render_food(matrix_u8_t* frame_buffer, point_u8_t food_position);

#endif /* ATMEGA_SNAKE_FOOD_SPAWNER_H_ */
