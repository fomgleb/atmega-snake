#include "joystick.h"

joystick_t
jk_create(analogue_source_t x_axis, analogue_source_t y_axis, button_t switch_btn) {
    adc_set_clock_prescaler(DIV_BY_2);
    adc_set_voltage_reference(AVCC_PIN);
    return (joystick_t){
        .x_axis = x_axis,
        .y_axis = y_axis,
        .switch_btn = switch_btn,
    };
}

point_u16_t
jk_get_stick_position(const joystick_t* joystick) {
    point_u16_t stick_position = {.x = 0, .y = 0};
    adc_set_enabled(true);

    adc_set_analogue_source(joystick->x_axis);
    stick_position.x = adc_wait_and_get_raw_value();

    adc_set_analogue_source(joystick->y_axis);
    stick_position.y = adc_wait_and_get_raw_value();

    adc_set_enabled(false);
    return stick_position;
}
