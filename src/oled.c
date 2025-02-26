#include "oled.h"

#include "twi.h"

typedef enum {
    COMMAND_SET_BRIGHTNESS = 0x81,      // Sets the brightness of the display
    COMMAND_SET_ADDRESSING_MODE = 0x20, // Selects addressing mode
    COMMAND_SET_COLUMN_ADDRESS = 0x21,  // Set column start and end address (Only for horizontal and vertical modes)
    COMMAND_SET_PAGE_ADDRESS = 0x22,    // Set page start and end address (Only for horizontal and vertical modes)

    COMMAND_SET_DISPLAY_START_LINE = 0x40, /* Set display RAM display start line register */
    COMMAND_SET_DISPLAY_OFFSET = 0xD3,     /* Set vertical shift by COM */

    COMMAND_SET_MULTIPLEX_RATIO = 0xA8, /* Set number of rendering rows */
} COMMAND;

typedef enum {
    CB_ALL_NEXT_BYTES_ARE_COMMANDS = 0, /* 0000 0000 */
    CB_ALL_NEXT_BYTES_ARE_DATA = 0x40,  /* 0100 0000 */
    CB_NEXT_BYTE_IS_COMMAND = 0x80,     /* 1000 0000 */
    CB_NEXT_BYTE_IS_DATA = 0xC0,        /* 1100 0000 */
} CONTROL_BYTE;

void
oled_init(void) {
    twi_init();
}

void
oled_start(uint8_t twi_oled_address) {
    twi_start(twi_oled_address);
}

void
oled_send_command_once(uint8_t twi_oled_address, OLED_COMMAND command) {
    twi_start(twi_oled_address);
    twi_transmit(CB_NEXT_BYTE_IS_COMMAND);
    twi_transmit(command);
    twi_stop();
}

void
oled_send_command_brightness_once(uint8_t twi_oled_address, uint8_t brightness_value) {
    twi_start(twi_oled_address);
    twi_transmit(CB_ALL_NEXT_BYTES_ARE_COMMANDS);
    twi_transmit(COMMAND_SET_BRIGHTNESS);
    twi_transmit(brightness_value);
    twi_stop();
}

void
oled_send_command_addressing_mode_once(uint8_t twi_oled_address, OLED_ADDRESSING_MODE mode) {
    twi_start(twi_oled_address);
    twi_transmit(CB_ALL_NEXT_BYTES_ARE_COMMANDS);
    twi_transmit(COMMAND_SET_ADDRESSING_MODE);
    twi_transmit(mode);
    twi_stop();
}

// Only for horizontal and vertical modes
void
oled_send_command_column_address_once(uint8_t twi_oled_address, uint8_t start_column_address_0_127,
                                      uint8_t end_column_address_0_127) {
    twi_start(twi_oled_address);
    twi_transmit(CB_ALL_NEXT_BYTES_ARE_COMMANDS);
    twi_transmit(COMMAND_SET_COLUMN_ADDRESS);
    twi_transmit(start_column_address_0_127);
    twi_transmit(end_column_address_0_127);
    twi_stop();
}

// Only for horizontal and vertical modes
void
oled_send_command_page_address_once(uint8_t twi_oled_address, uint8_t start_page_address_0_7,
                                    uint8_t end_page_address_0_7) {
    twi_start(twi_oled_address);
    twi_transmit(CB_ALL_NEXT_BYTES_ARE_COMMANDS);
    twi_transmit(COMMAND_SET_PAGE_ADDRESS);
    twi_transmit(start_page_address_0_7);
    twi_transmit(end_page_address_0_7);
    twi_stop();
}

void
oled_send_command_display_start_line_once(uint8_t twi_oled_address, uint8_t start_line_0_63) {
    twi_start(twi_oled_address);
    twi_transmit(CB_NEXT_BYTE_IS_COMMAND);
    twi_transmit(COMMAND_SET_DISPLAY_START_LINE | start_line_0_63);
    twi_stop();
}

void
oled_send_command_segment_remap_once(uint8_t twi_oled_address, OLED_SEGMENT_REMAP_MODE mode) {
    twi_start(twi_oled_address);
    twi_transmit(CB_NEXT_BYTE_IS_COMMAND);
    twi_transmit(mode);
    twi_stop();
}

/**
 * \brief           Sets number of rendering rows
 * \param[in]       ratio: Value from `15` to `64` inclusively
 */
void
oled_set_multiplex_ratio(uint8_t twi_oled_address, uint8_t ratio) {
    twi_start(twi_oled_address);
    twi_transmit(CB_ALL_NEXT_BYTES_ARE_COMMANDS);
    twi_transmit(COMMAND_SET_MULTIPLEX_RATIO);
    twi_transmit(ratio - 1);
    twi_stop();
}

/**
 * \brief           Sets the start scanning row: bottom or top
 * \note            For example, if `OLED_SCAN_DIRECTION_TOP_TO_BOTTOM` is set
 *                      during `OLED_SCAN_DIRECTION_BOTTOM_TO_TOP` then the
 *                      display will be vertically flipped immediately
 */
void
oled_set_scan_direction(uint8_t twi_oled_address, OLED_SCAN_DIRECTION scan_direction) {
    twi_start(twi_oled_address);
    twi_transmit(CB_NEXT_BYTE_IS_COMMAND);
    twi_transmit(scan_direction);
    twi_stop();
}

/**
 * \brief           Sets vertical shift by row.
 * \param[in]       offset: Value from `0` to `63` inclusively
 */
void
oled_set_display_offset(uint8_t twi_oled_address, uint8_t offset) {
    twi_start(twi_oled_address);
    twi_transmit(CB_ALL_NEXT_BYTES_ARE_COMMANDS);
    twi_transmit(COMMAND_SET_DISPLAY_OFFSET);
    twi_transmit(offset);
    twi_stop();
}

void
oled_start_sending_data(uint8_t twi_oled_address) {
    twi_start(twi_oled_address);
    twi_transmit(CB_ALL_NEXT_BYTES_ARE_DATA);
}

void
oled_send_data(uint8_t data_byte) {
    twi_transmit(data_byte);
}

void
oled_stop_sending_data(void) {
    twi_stop();
}

void
oled_send_data_once(uint8_t data_byte) {
    twi_transmit(CB_NEXT_BYTE_IS_DATA);
    twi_transmit(data_byte);
}
