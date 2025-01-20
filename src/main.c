#include <avr/interrupt.h>
#include <util/delay.h>
#include "food_spawner.h"
#include "joystick.h"
#include "oled.h"
#include "timer.h"

#define OLED_ADDRESS       0x3C /* 00111100 */
#define OLED_COLUMNS_COUNT 128
#define OLED_ROWS_COUNT    8 /* One row is one byte and consists of 8 bits/pixels */
#define JOYSTICK_DIRECTION DIR_UP_MACRO

#if JOYSTICK_DIRECTION == DIR_UP_MACRO
static const point_i16_t RIGTH_BOTTOM_JOYSTICK_CORNER = {.x = MAX_AXIS_VALUE, .y = MIN_AXIS_VALUE};
static const point_i16_t LEFT_TOP_JOYSTICK_CORNER = {.x = MIN_AXIS_VALUE, .y = MAX_AXIS_VALUE};
static const point_i16_t LEFT_BOTTOM_JOYSTICK_CORNER = {.x = MIN_AXIS_VALUE, .y = MIN_AXIS_VALUE};
static const point_i16_t RIGHT_TOP_JOYSTICK_CORNER = {.x = MAX_AXIS_VALUE, .y = MAX_AXIS_VALUE};
#elif JOYSTICK_DIRECTION == DIR_RIGHT_MACRO
static const point_i16_t RIGTH_BOTTOM_JOYSTICK_CORNER = {.x = MIN_AXIS_VALUE, .y = MIN_AXIS_VALUE};
static const point_i16_t LEFT_TOP_JOYSTICK_CORNER = {.x = MAX_AXIS_VALUE, .y = MAX_AXIS_VALUE};
static const point_i16_t LEFT_BOTTOM_JOYSTICK_CORNER = {.x = MIN_AXIS_VALUE, .y = MAX_AXIS_VALUE};
static const point_i16_t RIGHT_TOP_JOYSTICK_CORNER = {.x = MAX_AXIS_VALUE, .y = MIN_AXIS_VALUE};
#elif JOYSTICK_DIRECTION == DIR_DOWN_MACRO
static const point_i16_t RIGTH_BOTTOM_JOYSTICK_CORNER = {.x = MIN_AXIS_VALUE, .y = MAX_AXIS_VALUE};
static const point_i16_t LEFT_TOP_JOYSTICK_CORNER = {.x = MAX_AXIS_VALUE, .y = MIN_AXIS_VALUE};
static const point_i16_t LEFT_BOTTOM_JOYSTICK_CORNER = {.x = MAX_AXIS_VALUE, .y = MAX_AXIS_VALUE};
static const point_i16_t RIGHT_TOP_JOYSTICK_CORNER = {.x = MIN_AXIS_VALUE, .y = MIN_AXIS_VALUE};
#elif JOYSTICK_DIRECTION == DIR_LEFT_MACRO
static const point_i16_t RIGTH_BOTTOM_JOYSTICK_CORNER = {.x = MAX_AXIS_VALUE, .y = MAX_AXIS_VALUE};
static const point_i16_t LEFT_TOP_JOYSTICK_CORNER = {.x = MIN_AXIS_VALUE, .y = MIN_AXIS_VALUE};
static const point_i16_t LEFT_BOTTOM_JOYSTICK_CORNER = {.x = MAX_AXIS_VALUE, .y = MIN_AXIS_VALUE};
static const point_i16_t RIGHT_TOP_JOYSTICK_CORNER = {.x = MIN_AXIS_VALUE, .y = MAX_AXIS_VALUE};
#endif

static REAL_INLINE direction_t get_stick_direction(const joystick_t* joystick);
static void initialize_oled(void);
static void clear_oled(void);

static bool move_snake_please = false;
TMR1_INTERRUPT { move_snake_please = true; }

int
main(void) {
    sei();
    tmr1_init();

    initialize_oled();
    clear_oled();

    matrix_u8_t frame_buffer = mx_create(uint8_t, OLED_COLUMNS_COUNT, OLED_ROWS_COUNT);
    joystick_t joystick = jk_create(ADC2, ADC1, btn_create(&DDRC, &PORTC, &PINC, PC0));
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

        direction_t new_direction = get_stick_direction(&joystick);
        if (new_direction != DIR_NONE) {
            snake_direction = new_direction;
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

static REAL_INLINE direction_t
get_stick_direction(const joystick_t* joystick) {
    if (joystick == NULL) {
        return DIR_NONE;
    }

    static const int16_t DEAD_ZONE = 50;

    point_i16_t stick_pos = POINT_I16(jk_get_stick_position(joystick));

    if ((stick_pos.x <= IDLE_STICK_POS_XY + DEAD_ZONE) && (stick_pos.x >= IDLE_STICK_POS_XY - DEAD_ZONE)
        && (stick_pos.y <= IDLE_STICK_POS_XY + DEAD_ZONE) && (stick_pos.y >= IDLE_STICK_POS_XY - DEAD_ZONE)) {
        return DIR_NONE;
    }

    /* Equation of straight line */
    bool is_up_or_right = ((stick_pos.y * 10 - RIGTH_BOTTOM_JOYSTICK_CORNER.y * 10)
                               / (LEFT_TOP_JOYSTICK_CORNER.y - RIGTH_BOTTOM_JOYSTICK_CORNER.y)
                           - (stick_pos.x * 10 - RIGTH_BOTTOM_JOYSTICK_CORNER.x * 10)
                                 / (LEFT_TOP_JOYSTICK_CORNER.x - RIGTH_BOTTOM_JOYSTICK_CORNER.x))
                          > 0;

    /* Equation of straight line */
    bool is_right_or_down = (stick_pos.x * 10 - LEFT_BOTTOM_JOYSTICK_CORNER.x * 10)
                                    / (RIGHT_TOP_JOYSTICK_CORNER.x - LEFT_BOTTOM_JOYSTICK_CORNER.x)
                                - (stick_pos.y * 10 - LEFT_BOTTOM_JOYSTICK_CORNER.y * 10)
                                      / (RIGHT_TOP_JOYSTICK_CORNER.y - LEFT_BOTTOM_JOYSTICK_CORNER.y)

                            > 0;

    if (is_up_or_right && is_right_or_down) {
        return DIR_RIGHT;
    } else if (is_up_or_right && !is_right_or_down) {
        return DIR_UP;
    } else if (!is_up_or_right && is_right_or_down) {
        return DIR_DOWN;
    } else if (!is_up_or_right && !is_right_or_down) {
        return DIR_LEFT;
    }

    return DIR_NONE;
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
