#include "square.h"

#include "oled.h"

#define NUMBER_OF_BITS_IN_PAGE 8

void
sqr_render_to_frame_buffer(matrix_u8_t* frame_buffer, const square_t* square) {
    uint8_t start_page_index = square->position.y / NUMBER_OF_BITS_IN_PAGE;
    uint8_t end_page_index = (square->position.y + square->size) / NUMBER_OF_BITS_IN_PAGE;

    uint8_t start_bit_index_of_start_page = square->position.y - start_page_index * NUMBER_OF_BITS_IN_PAGE;
    uint8_t end_bit_index_of_end_page = (square->position.y + square->size) - end_page_index * NUMBER_OF_BITS_IN_PAGE;

    if (start_page_index == end_page_index) {
        for (uint8_t x = square->position.x; x < square->position.x + square->size; x++) {
            mx_set(frame_buffer, x, start_page_index,
                   mx_get(frame_buffer, x, start_page_index)
                       | ((0xFF << start_bit_index_of_start_page) & ~(0xFF << end_bit_index_of_end_page)));
        }
    } else {
        for (uint8_t x = square->position.x; x < square->position.x + square->size; x++) {
            mx_set(frame_buffer, x, start_page_index,
                   mx_get(frame_buffer, x, start_page_index) | (0xFF << start_bit_index_of_start_page));
            for (uint8_t y_page = start_page_index + 1; y_page < end_page_index; y_page++) {
                mx_set(frame_buffer, x, y_page, mx_get(frame_buffer, x, y_page) | 0xFF);
            }
            mx_set(frame_buffer, x, end_page_index,
                   mx_get(frame_buffer, x, end_page_index) | ~(0xFF << end_bit_index_of_end_page));
        }
    }
}

void
sqr_move(square_t* square) {
    if (square->position.x + (int16_t)square->speed.x >= square->min_position.x
        && square->position.x + (int16_t)square->speed.x <= square->max_position.x) {
        square->position.x = square->position.x + square->speed.x;
    } else {
        square->position.x = square->position.x + (int16_t)square->speed.x >= square->min_position.x
                                 ? square->max_position.x
                                 : square->min_position.x;
    }

    if (square->position.y + (int16_t)square->speed.y >= square->min_position.y
        && square->position.y + (int16_t)square->speed.y <= square->max_position.y) {
        square->position.y += square->speed.y;
    } else {
        square->position.y = square->position.y + (int16_t)square->speed.y >= square->min_position.y
                                 ? square->max_position.y
                                 : square->min_position.y;
    }
}

void
sqr_bouncing_move(square_t* square) {
    if (square->position.x == square->max_position.x || square->position.x == square->min_position.x) {
        square->speed.x *= -1;
    }

    if (square->position.y == square->max_position.y || square->position.y == square->min_position.y) {
        square->speed.y *= -1;
    }

    sqr_move(square);
}
