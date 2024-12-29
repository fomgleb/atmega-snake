#include "snake.h"

#define BITS_COUNT_IN_BYTE 8

static void
set_direction(matrix_u16_t* directions_matrix, point_u8_t position, direction_t direction) {
    uint16_t old_value = mx_get(directions_matrix, position.x, position.y / 8);
    uint16_t value_with_new_direction = (((direction & 0x01) == 1) ? old_value | (1 << ((position.y % 8) * 2))
                                                                   : old_value & ~(1 << ((position.y % 8) * 2)));
    value_with_new_direction = ((((direction >> 1) & 0x01) == 1)
                                    ? value_with_new_direction | (1 << ((position.y % 8) * 2 + 1))
                                    : value_with_new_direction & ~(1 << ((position.y % 8) * 2 + 1)));
    mx_set(directions_matrix, position.x, position.y / 8, value_with_new_direction);
}

static direction_t
get_direction(matrix_u16_t* directions_matrix, point_u8_t position) {
    uint16_t direction_page = mx_get(directions_matrix, position.x, position.y / 8);
    direction_t direction = (direction_page >> ((position.y % 8) * 2)) & 0x03;
    return direction;
}

static void
set_bit_in_u8_matrix(matrix_u8_t* matrix, point_u8_t position) {
    uint8_t old_byte = mx_get(matrix, position.x, position.y / 8);
    uint8_t new_byte = old_byte | (1 << (position.y % 8));
    mx_set(matrix, position.x, position.y / 8, new_byte);
}

REAL_INLINE static void
unset_bit_in_u8_matrix(matrix_u8_t* matrix, point_u8_t position) {
    uint8_t old_byte = mx_get(matrix, position.x, position.y / 8);
    uint8_t new_byte = old_byte & ~(1 << (position.y % 8));
    mx_set(matrix, position.x, position.y / 8, new_byte);
}

static bool
get_bit_in_u8_matrix(matrix_u8_t* matrix, point_u8_t position) {
    uint8_t page = mx_get(matrix, position.x, position.y / 8);
    return page & (1 << position.y % 8);
}

/**
 * \param[in]       position: x value: from 0 to 31; y value: from 0 to 15
 */
static void
add_flesh_chunk(snake_t* snake, point_u8_t position, direction_t direction) {
    set_direction(&snake->directions, position, direction);
    set_bit_in_u8_matrix(&snake->flesh_chunks, position);
}

REAL_INLINE static void
remove_flesh_chunk(snake_t* snake, point_u8_t position) {
    unset_bit_in_u8_matrix(&snake->flesh_chunks, position);
}

REAL_INLINE static bool
get_flesh_chunk(snake_t* snake, point_u8_t position) {
    return get_bit_in_u8_matrix(&snake->flesh_chunks, position);
}

snake_t
snk_create(point_u8_t start_postion, uint8_t length, point_u8_t board_size, uint8_t scale) {
    snake_t snake = {
        .flesh_chunks = mx_create(uint8_t, board_size.x / scale, board_size.y / scale),
        .directions = mx_create(uint16_t, board_size.x / scale, board_size.y / scale),
        .length = length,
        .head_position = start_postion,
        .tail_position = (point_u8_t){start_postion.x + 4, start_postion.y},
    };

    add_flesh_chunk(&snake, start_postion, LEFT);
    add_flesh_chunk(&snake, (point_u8_t){start_postion.x + 1, start_postion.y}, LEFT);
    add_flesh_chunk(&snake, (point_u8_t){start_postion.x + 2, start_postion.y}, LEFT);
    add_flesh_chunk(&snake, (point_u8_t){start_postion.x + 3, start_postion.y}, LEFT);
    add_flesh_chunk(&snake, (point_u8_t){start_postion.x + 4, start_postion.y}, LEFT);

    return snake;
}

void
snk_delete(snake_t* snake) {
    mx_delete(&snake->flesh_chunks);
    mx_delete(&snake->directions);
}

#define SUM_POINTS_U8(point1, point2)                                                                                  \
    (point_u8_t) { point1.x + point2.x, point1.y + point2.y }

#define SUM_POINTS_I8(point1, point2)                                                                                  \
    (point_i8_t) { point1.x + point2.x, point1.y + point2.y }

REAL_INLINE static point_i8_t
get_direction_point(direction_t direction) {
    point_i8_t direction_point = {.x = 0, .y = 0};
    switch (direction) {
        case UP: direction_point.y--; break;
        case RIGHT: direction_point.x++; break;
        case DOWN: direction_point.y++; break;
        case LEFT: direction_point.x--; break;
        default: break;
    }
    return direction_point;
}

void
snk_move(snake_t* snake, direction_t direction, point_u8_t food_position, bool* collided, bool* ate_food) {
    *collided = false;
    *ate_food = false;

    direction_t head_direction = get_direction(&snake->directions, snake->head_position);
    // TODO: fast solution, not convenient
    if (head_direction == UP && direction == DOWN) {
        direction = UP;
    } else if (head_direction == DOWN && direction == UP) {
        direction = DOWN;
    } else if (head_direction == LEFT && direction == RIGHT) {
        direction = LEFT;
    } else if (head_direction == RIGHT && direction == LEFT) {
        direction = RIGHT;
    }

    point_i8_t new_head_position = SUM_POINTS_I8(snake->head_position, get_direction_point(direction));
    if (new_head_position.x < 0 || new_head_position.x >= 32 || new_head_position.y < 0 || new_head_position.y >= 16
        || get_flesh_chunk(snake, (point_u8_t){.x = new_head_position.x, .y = new_head_position.y})) {
        *collided = true;
        return;
    }

    set_direction(&snake->directions, snake->head_position, direction);

    add_flesh_chunk(snake, (point_u8_t){.x = new_head_position.x, .y = new_head_position.y}, direction);
    snake->head_position = (point_u8_t){.x = new_head_position.x, .y = new_head_position.y};

    if (new_head_position.x != food_position.x || new_head_position.y != food_position.y) {
        point_i8_t tail_direction = get_direction_point(get_direction(&snake->directions, snake->tail_position));
        remove_flesh_chunk(snake, snake->tail_position);
        snake->tail_position = SUM_POINTS_U8(snake->tail_position, tail_direction);
    } else {
        *ate_food = true;
    }
}

void
snk_render(matrix_u8_t* frame_buffer, const snake_t* snake) {
    const uint8_t SCALE = 4; // TODO: Use scale from `snake`

    for (uint8_t y_page = 0; y_page < snake->flesh_chunks.height; y_page++) {
        for (uint8_t x = 0; x < snake->flesh_chunks.width; x++) {
            uint8_t page_value = mx_get(&snake->flesh_chunks, x, y_page);
            if (page_value == 0) {
                continue;
            }
            for (uint8_t bit_i = 0; bit_i < BITS_COUNT_IN_BYTE; bit_i++) {
                if (((page_value >> bit_i) & 0x01) == 0) {
                    continue;
                }

                mx_set(frame_buffer, x * SCALE, y_page * SCALE + (bit_i * SCALE) / BITS_COUNT_IN_BYTE,
                       (bit_i % 2 == 0 ? 0x0F : 0xF0)
                           | mx_get(frame_buffer, x * SCALE, y_page * SCALE + (bit_i * SCALE) / BITS_COUNT_IN_BYTE));
                mx_set(frame_buffer, x * SCALE + 1, y_page * SCALE + (bit_i * SCALE) / BITS_COUNT_IN_BYTE,
                       (bit_i % 2 == 0 ? 0x0F : 0xF0)
                           | mx_get(frame_buffer, x * SCALE, y_page * SCALE + (bit_i * SCALE) / BITS_COUNT_IN_BYTE));
                mx_set(frame_buffer, x * SCALE + 2, y_page * SCALE + (bit_i * SCALE) / BITS_COUNT_IN_BYTE,
                       (bit_i % 2 == 0 ? 0x0F : 0xF0)
                           | mx_get(frame_buffer, x * SCALE, y_page * SCALE + (bit_i * SCALE) / BITS_COUNT_IN_BYTE));
                mx_set(frame_buffer, x * SCALE + 3, y_page * SCALE + (bit_i * SCALE) / BITS_COUNT_IN_BYTE,
                       (bit_i % 2 == 0 ? 0x0F : 0xF0)
                           | mx_get(frame_buffer, x * SCALE, y_page * SCALE + (bit_i * SCALE) / BITS_COUNT_IN_BYTE));
            }
        }
    }
}
