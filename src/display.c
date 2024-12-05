#include "display.h"

#include "oled.h"

// static void
// initialize_oled(void) {
//     oled_init();
//     _delay_ms(100); // Wait for OLED power-up
//     oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_DISABLE_DISPLAY);
//     oled_send_command_brightness_once(OLED_ADDRESS, 127);
//     oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_RESUME_RAM_CONTENT_DISPALY);
//     oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_INVERSE_MODE);
//     oled_send_command_addressing_mode_once(OLED_ADDRESS, OLED_ADDRESSING_MODE_HORIZONTAL);
//     oled_send_command_column_address_once(OLED_ADDRESS, 0, 127);
//     oled_send_command_page_address_once(OLED_ADDRESS, 0, 7);
//     /* something else */
//     oled_send_command_display_start_line_once(OLED_ADDRESS, 0);
//     oled_send_command_segment_remap_once(OLED_ADDRESS, OLED_SEGMENT_REMAP_MODE_RIGHT_TO_LEFT);
//     oled_set_multiplex_ratio(OLED_ADDRESS, 64);
//     oled_set_scan_direction(OLED_ADDRESS, OLED_SCAN_DIRECTION_BOTTOM_TO_TOP);
//     oled_set_display_offset(OLED_ADDRESS, 0);

//     oled_send_command_once(OLED_ADDRESS, 0xD5); // Set display clock divide ratio
//     oled_send_command_once(OLED_ADDRESS, 0x80); // Suggested ratio
//     oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_SET_CHARGE_PUMP);
//     oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_ENABLE_CHARGE_PUMP);
//     oled_send_command_once(OLED_ADDRESS, 0xDA); // Set COM pins hardware configuration
//     oled_send_command_once(OLED_ADDRESS, 0x12); // Alternative COM pin config
//     oled_send_command_once(OLED_ADDRESS, OLED_COMMAND_ENABLE_DISPLAY);
// }

display_t
dpl_create(uint8_t width, uint8_t height) {

    display_t display = {
        ._frame_buffer = bm_create(width, height),
        ._is_rendered = false,
    };
    return display;
}

void
dpl_delete(const display_t* display) {
    bm_delete(&display->_frame_buffer);
}
