#ifndef ATMEGA_SNAKE_JOYSTICK_H_
#define ATMEGA_SNAKE_JOYSTICK_H_

#include "adc.h"
#include "avr-utils/common/button.h"
#include "avr-utils/common/point.h"

#define MAX_AXIS_VALUE    ADC_MAX_RAW_VALUE
#define MIN_AXIS_VALUE    0

#define IDLE_STICK_POS_XY 511

typedef struct {
    analogue_source_t x_axis;
    analogue_source_t y_axis;
    button_t switch_btn;
} joystick_t;

joystick_t jk_create(analogue_source_t x_axis, analogue_source_t y_axis, button_t switch_btn);
point_u16_t jk_get_stick_position(const joystick_t* joystick);

static REAL_INLINE bool
jk_switch_is_pressed(const joystick_t* joystick) {
    return btn_is_pressed(&joystick->switch_btn);
}

#endif /* ATMEGA_SNAKE_JOYSTICK_H_ */
