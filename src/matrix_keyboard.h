#ifndef ATMEGA_SNAKE_MATRIX_KEYBOARD_H_
#define ATMEGA_SNAKE_MATRIX_KEYBOARD_H_

#include "avr-utils/common/leg.h"

typedef struct {
    output_leg_t row1;
    output_leg_t row2;
    output_leg_t row3;
    output_leg_t row4;
    input_leg_t column1;
    input_leg_t column2;
    input_leg_t column3;
} matrix_keyboard_t;

void mk_init(matrix_keyboard_t* matrix_keyboard);
bool mk_button_is_pressed(input_leg_t* column, output_leg_t* row);

#endif /* ATMEGA_SNAKE_MATRIX_KEYBOARD_H_ */
