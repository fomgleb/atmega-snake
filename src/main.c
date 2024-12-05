#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>
#include "oled.h"
#include "square.h"
#include "stdlib.h"
#include "timer.h"
#include "uart.h"

#define OLED_ADDRESS       0x3C /* 00111100 */
#define OLED_COLUMNS_COUNT 128
#define OLED_ROWS_COUNT    8 /* One row is one byte and consists of 8 bits/pixels */

static void initialize_oled(void);
static void clear_oled(void);
square_t* create_squares(uint8_t squares_count);

volatile bool do_something_please = false;

int
main(void) {
    sei();
    uart_init();
    _delay_ms(200);

    initialize_oled();

    // start_twi_and_wait(OLED_ADDRESS);

    clear_oled();

    bytes_matrix_t frame_buffer = bm_create(OLED_COLUMNS_COUNT, OLED_ROWS_COUNT);

    const uint8_t SQUARES_COUNT = 50;
    square_t* squares = create_squares(SQUARES_COUNT);

    // oled_send_command_display_start_line_once(OLED_ADDRESS, 0);

    point_u8_t rendering_byte_position = {0, 0};
    oled_start_rendering_and_wait(OLED_ADDRESS);
    tmr_1_init();

    // uart_transmit(0xFF00 & _twi_state);
    // uart_transmit(0x00FF & _twi_state);

    while (1) {

        // bm_clear(&frame_buffer);

        // for (uint8_t i = 0; i < SQUARES_COUNT; i++) {
        //     sqr_bouncing_move(&squares[i]);
        // }

        // for (uint8_t i = 0; i < SQUARES_COUNT; i++) {
        //     sqr_render_to_frame_buffer(&frame_buffer, &squares[i]);
        // }

        if (do_something_please) {
            do_something_please = false;
            //                                                             uart_transmit('s');
            // uart_transmit('\n');
            // uart_transmit('\r');

            if (bm_get_byte(&frame_buffer, 0, 0) == 0x00) {
                for (uint8_t y = 0; y < frame_buffer.height; y++) {
                    for (uint8_t x = 0; x < frame_buffer.width; x++) {
                        bm_assign_byte(&frame_buffer, x, y, 0xFF);
                    }
                }
            } else {
                for (uint8_t y = 0; y < frame_buffer.height; y++) {
                    for (uint8_t x = 0; x < frame_buffer.width; x++) {
                        bm_assign_byte(&frame_buffer, x, y, 0x00);
                    }
                }
            }
        }

        // uart_send_str("hello");
        if (twi_read_flag_and_clear(BEGIN__SEND_DATA_BYTE)) {
            // uart_send_str("h");
            // uart_send_str("hello");
            // uart_transmit(bm_get_byte(&frame_buffer, rendering_byte_position.x, rendering_byte_position.y));

            // uart_send_str("END__SEND_DATA_BYTE_SUCCESSFULLY, BEGIN__SEND_DATA_BYTE");
            // uint8_t a = 0;
            // while (1) {
            //     a++;
            //     _delay_ms(10);
            // }
            //         uart_transmit('s');
            // uart_transmit('\n');
            // uart_transmit('\r');
            oled_render_byte(bm_get_byte(&frame_buffer, rendering_byte_position.x, rendering_byte_position.y));
            // uart_send_str("h");
            rendering_byte_position.x++;
            if (rendering_byte_position.x >= frame_buffer.width) {
                rendering_byte_position.x = 0;
                rendering_byte_position.y++;
                if (rendering_byte_position.y >= frame_buffer.height) {
                    rendering_byte_position.y = 0;
                }
            }
        }

        // for (uint8_t y = 0; y < frame_buffer.height; y++) {
        //     for (uint8_t x = 0; x < frame_buffer.width; x++) {
        //         oled_send_data(bm_get_byte(&frame_buffer, x, y));
        //         // _delay_ms(10);
        //     }
        // }
    }
    oled_stop_sending_data();

    bm_delete(&frame_buffer);

    return 0;
}

TMR_1_INTERRUPT { do_something_please = true; }

static void
initialize_oled(void) {

    oled_init();

    _delay_ms(100); // Wait for OLED power-up

    oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_DISABLE_DISPLAY);

    oled_send_command_brightness_once(OLED_ADDRESS, 127);
    oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_RESUME_RAM_CONTENT_DISPALY);
    oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_NOT_INVERSE_MODE);
    oled_send_command_addressing_mode_once(OLED_ADDRESS, OLED_ADDRESSING_MODE_HORIZONTAL);
    oled_send_command_column_address_once(OLED_ADDRESS, 0, 127);
    oled_send_command_page_address_once(OLED_ADDRESS, 0, 7);
    /* something else */
    oled_send_command_display_start_line_once(OLED_ADDRESS, 0);
    oled_send_command_segment_remap_once(OLED_ADDRESS, OLED_SEGMENT_REMAP_MODE_LEFT_TO_RIGHT);
    oled_set_multiplex_ratio(OLED_ADDRESS, 64);
    oled_set_scan_direction(OLED_ADDRESS, OLED_SCAN_DIRECTION_TOP_TO_BOTTOM);
    oled_set_display_offset(OLED_ADDRESS, 0);

    oled_send_command_once(OLED_ADDRESS, 0xD5); // Set display clock divide ratio
    oled_send_command_once(OLED_ADDRESS, 0x80); // Suggested ratio
    oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_SET_CHARGE_PUMP);
    oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_ENABLE_CHARGE_PUMP);
    oled_send_command_once(OLED_ADDRESS, 0xDA); // Set COM pins hardware configuration
    oled_send_command_once(OLED_ADDRESS, 0x12); // Alternative COM pin config
    oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_ENABLE_DISPLAY);
}

static void
clear_oled(void) {
    oled_start_rendering_and_wait(OLED_ADDRESS);
    uint8_t counter = 0;
    for (uint8_t x = 0; x < OLED_COLUMNS_COUNT; x++) {
        for (uint8_t y = 0; y < OLED_ROWS_COUNT; y++) {
            while (!twi_read_flag_and_clear(BEGIN__SEND_DATA_BYTE) && !twi_read_flag_and_clear(BEGIN__WRITE_TO_SLAVE)) {
                uart_transmit('a' + counter);
            }
            counter++;

            oled_render_byte(0);
        }
    }
    oled_stop_sending_data();
}

uint8_t random_numbers[50] = {123, 147, 3,   83,  56,  188, 180, 131, 173, 42,  97,  125, 148, 102, 157, 221, 105,
                              243, 226, 202, 51,  30,  58,  24,  231, 196, 145, 123, 19,  175, 12,  171, 137, 32,
                              179, 225, 63,  173, 254, 9,   234, 75,  48,  97,  100, 230, 87,  10,  49,  229};

square_t*
create_squares(uint8_t squares_count) {
    square_t* squares = malloc(squares_count * sizeof(square_t));
    for (uint8_t i = 0; i < squares_count; i++) {
        square_t temp = {
            .position = {random_numbers[i] % 128, random_numbers[i] % 64},
            .min_position = {0, 0},
            .max_position = {127 - 2 + 1, 63 - 2 + 1},
            .speed = {random_numbers[i] % 2 == 0 ? (random_numbers[i % 50] % 5) : -(random_numbers[i] % 5),
                      random_numbers[i] % 2 == 0 ? -(random_numbers[i] % 5) : (random_numbers[(i * 2) % 50] % 5)},
            .size = 2,
        };
        temp.speed.x = temp.speed.x == 0 ? 1 : temp.speed.x;
        temp.speed.y = temp.speed.y == 0 ? 1 : temp.speed.y;
        memcpy(&squares[i], &temp, sizeof(square_t));
    }

    return squares;
}
