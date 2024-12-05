#ifndef ATMEGA_SNAKE_OLED_H_
#define ATMEGA_SNAKE_OLED_H_

#include <inttypes.h>
#include "twi.h"

typedef enum {
    OLED_COMMAND_DISABLE_DISPLAY = 0xAE, // Turns the OLED panel OFF (sleep mode)
    OLED_COMMAND_ENABLE_DISPLAY = 0xAF,  // Turns the OLED panel ON (wake up from sleep mode)

    OLED_COMMAND_LIGHT_UP_ALL_PIXELS = 0xA5,        // Forces all pixels to light up, ignoring RAM contents
    OLED_COMMAND_RESUME_RAM_CONTENT_DISPALY = 0xA4, // Resumes display from RAM contents

    OLED_COMMAND_INVERSE_MODE = 0xA7,     // Inverses the display (0 = ON, 1 = OFF)
    OLED_COMMAND_NOT_INVERSE_MODE = 0xA6, // Sets normal display mode ( 0 = OFF, 1 = ON)

    OLED_COMMAND_SET_CHARGE_PUMP = 0x8D,
    OLED_COMMAND_ENABLE_CHARGE_PUMP = 0x14,
    OLED_COMMAND_DISABLE_CHARGE_PUMP = 0x10,

} OLED_COMMAND;

typedef enum {
    OLED_SEGMENT_REMAP_MODE_RIGHT_TO_LEFT = 0xA0, /* Start column is on the right side */
    OLED_SEGMENT_REMAP_MODE_LEFT_TO_RIGHT = 0xA1, /* Start column is on the left side  */
} OLED_SEGMENT_REMAP_MODE;

typedef enum {
    OLED_SCAN_DIRECTION_BOTTOM_TO_TOP = 0xC0, /* Start row is at the bottom */
    OLED_SCAN_DIRECTION_TOP_TO_BOTTOM = 0xC8, /* Start row is at the top */
} OLED_SCAN_DIRECTION;

typedef enum {
    OLED_ADDRESSING_MODE_HORIZONTAL = 0,
    OLED_ADDRESSING_MODE_VERTICAL = 1,
    OLED_ADDRESSING_MODE_PAGE = 2,
} OLED_ADDRESSING_MODE;

void oled_init(void);

void oled_send_command_once(uint8_t twi_oled_address, OLED_COMMAND command);
void oled_send_command_brightness_once(uint8_t twi_oled_address, uint8_t brightness_value);
void oled_send_command_addressing_mode_once(uint8_t twi_oled_address, OLED_ADDRESSING_MODE mode);
void oled_send_command_column_address_once(uint8_t twi_oled_address, uint8_t start_column_address_0_127,
                                           uint8_t end_column_address_0_127);
void oled_send_command_page_address_once(uint8_t twi_oled_address, uint8_t start_page_address_0_7,
                                         uint8_t end_page_address_0_7);

void oled_send_command_display_start_line_once(uint8_t twi_oled_address, uint8_t start_line_0_63);

void oled_send_command_segment_remap_once(uint8_t twi_oled_address, OLED_SEGMENT_REMAP_MODE mode);

void oled_set_multiplex_ratio(uint8_t twi_oled_address, uint8_t ratio);
void oled_set_scan_direction(uint8_t twi_oled_address, OLED_SCAN_DIRECTION scan_direction);
void oled_set_display_offset(uint8_t twi_oled_address, uint8_t offset);

void oled_start_rendering_and_wait(uint8_t twi_oled_address);
void oled_render_byte(uint8_t byte_value);

// static REAL_INLINE bool
// oled_byte_is_rendered(void) {
//     return twi_read_flag_and_clear()
//     return twi_state_data_byte_sent();
// }

void oled_stop_sending_data(void);

#endif // ATMEGA_SNAKE_OLED_H_
