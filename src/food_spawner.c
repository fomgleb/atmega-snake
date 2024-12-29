#include "food_spawner.h"

static bool
get_bit_in_u8_matrix(const matrix_u8_t* matrix, point_u8_t position) {
    uint8_t page = mx_get(matrix, position.x, position.y / 8);
    return page & (1 << position.y % 8);
}

REAL_INLINE static bool
get_flesh_chunk(const snake_t* snake, point_u8_t position) {
    return get_bit_in_u8_matrix(&snake->flesh_chunks, position);
}

point_u8_t
fs_get_random_food_position(const snake_t* snake) {
    uint16_t random_number = 0;
    for (uint8_t y = 0; y < snake->directions.height; y++) {
        for (uint8_t x = 0; x < snake->directions.width; x++) {
            random_number += mx_get(&snake->directions, x, y);
        }
    }
    point_u8_t random_position = {.x = random_number % 32,
                                  .y = (random_number + snake->head_position.x + snake->head_position.y
                                        + snake->tail_position.x + snake->tail_position.y)
                                       % 16};
    if (get_flesh_chunk(snake, random_position)) {
        while (true) {
            random_position.x++;
            if (random_position.x >= 32) {
                random_position.x = 0;
                random_position.y++;
                if (random_position.y >= 16) {
                    random_position.y = 0;
                }
            }

            if (!get_flesh_chunk(snake, random_position)) {
                break;
            }
        }
    }

    return random_position;
}

void
fs_render_food(matrix_u8_t* frame_buffer, point_u8_t food_position) {
    uint8_t x = food_position.x;
    uint8_t y = food_position.y;
    mx_set(frame_buffer, x * 4, y / 2, (y % 2 == 0 ? 0x0F : 0xF0) | mx_get(frame_buffer, x * 4, y / 2));
    mx_set(frame_buffer, x * 4 + 1, y / 2, (y % 2 == 0 ? 0x0F : 0xF0) | mx_get(frame_buffer, x * 4 + 1, y / 2));
    mx_set(frame_buffer, x * 4 + 2, y / 2, (y % 2 == 0 ? 0x0F : 0xF0) | mx_get(frame_buffer, x * 4 + 2, y / 2));
    mx_set(frame_buffer, x * 4 + 3, y / 2, (y % 2 == 0 ? 0x0F : 0xF0) | mx_get(frame_buffer, x * 4 + 3, y / 2));
}
