#include <avr/interrupt.h>
#include <util/delay.h>
#include "food_spawner.h"
#include "matrix_keyboard.h"
#include "oled.h"
#include "timer.h"

#define OLED_ADDRESS       0x3C /* 00111100 */
#define OLED_COLUMNS_COUNT 128
#define OLED_ROWS_COUNT    8 /* One row is one byte and consists of 8 bits/pixels */

static void initialize_oled(void);
static void clear_oled(void);
static matrix_keyboard_t create_matrix_keyboard(void);

static bool move_snake_please = false;
TMR1_INTERRUPT { move_snake_please = true; }

int
main(void) {
    sei();
    tmr1_init();

    initialize_oled();
    clear_oled();

    matrix_u8_t frame_buffer = mx_create(uint8_t, OLED_COLUMNS_COUNT, OLED_ROWS_COUNT);
    matrix_keyboard_t matrix_keyboard = create_matrix_keyboard();
    snake_t snake = snk_create((point_u8_t){10, 10}, 5, (point_u8_t){OLED_COLUMNS_COUNT, OLED_ROWS_COUNT}, 4);
    point_u8_t food_position = fs_get_random_food_position(&snake);

    direction_t snake_direction = DIR_LEFT;

    oled_start_sending_data(OLED_ADDRESS);
    // TODO: Somehow when running with SimulIDE, rendering pointer is not at the start of the display
    if (TWI_SPEED == 100000) { /* If SimulIDE is used */
        uint8_t x_ = 49;
        for (uint8_t y = 0; y < frame_buffer.height; y++) {
            for (; x_ < frame_buffer.width; x_++) {
                oled_send_data(0x00);
            }
            x_ = 0;
        }
    }
    while (1) {
        for (uint8_t y = 0; y < frame_buffer.height; y++) {
            for (uint8_t x = 0; x < frame_buffer.width; x++) {
                mx_set(&frame_buffer, x, y, 0);
            }
        }

        if (mk_button_is_pressed(&matrix_keyboard.column2, &matrix_keyboard.row1)) {
            snake_direction = DIR_UP;
        } else if (mk_button_is_pressed(&matrix_keyboard.column1, &matrix_keyboard.row2)) {
            snake_direction = DIR_LEFT;
        } else if (mk_button_is_pressed(&matrix_keyboard.column2, &matrix_keyboard.row2)) {
            snake_direction = DIR_DOWN;
        } else if (mk_button_is_pressed(&matrix_keyboard.column3, &matrix_keyboard.row2)) {
            snake_direction = DIR_RIGHT;
        }

        if (move_snake_please) {
            move_snake_please = false;
            bool snake_collided = false;
            bool snake_ate_food = false;
            snk_move(&snake, snake_direction, food_position, &snake_collided, &snake_ate_food);
            if (snake_ate_food) {
                food_position = fs_get_random_food_position(&snake);
            }

            if (snake_collided) {
                snk_set_position(&snake, (point_u8_t){10, 10}, DIR_LEFT, 5);
                snake_direction = DIR_LEFT;
            }
        }

        snk_render(&frame_buffer, &snake);
        fs_render_food(&frame_buffer, food_position);

        for (uint8_t y = 0; y < frame_buffer.height; y++) {
            for (uint8_t x = 0; x < frame_buffer.width; x++) {
                oled_send_data(mx_get(&frame_buffer, x, y));
            }
        }
    }
    oled_stop_sending_data();

    mx_delete(&frame_buffer);

    return 0;
}

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
    oled_start_sending_data(OLED_ADDRESS);
    for (uint8_t x = 0; x < OLED_COLUMNS_COUNT; x++) {
        for (uint8_t y = 0; y < OLED_ROWS_COUNT; y++) {
            oled_send_data(0);
        }
    }
    oled_stop_sending_data();
}

static matrix_keyboard_t
create_matrix_keyboard(void) {
    matrix_keyboard_t matrix_keyboard = {
        .row1 = leg_create_output_leg(&DDRB, &PORTB, PB0),
        .row2 = leg_create_output_leg(&DDRD, &PORTD, PD7),
        .row3 = leg_create_output_leg(&DDRD, &PORTD, PD6),
        .row4 = leg_create_output_leg(&DDRD, &PORTD, PD5),
        .column1 = leg_create_input_leg(&DDRD, &PORTD, &PIND, PD4),
        .column2 = leg_create_input_leg(&DDRD, &PORTD, &PIND, PD3),
        .column3 = leg_create_input_leg(&DDRD, &PORTD, &PIND, PD2),
    };
    mk_init(&matrix_keyboard);
    return matrix_keyboard;
}
