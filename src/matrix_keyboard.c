#include "matrix_keyboard.h"

void
mk_init(matrix_keyboard_t* matrix_keyboard) {
    leg_set(&matrix_keyboard->row1);
    leg_set(&matrix_keyboard->row2);
    leg_set(&matrix_keyboard->row3);
    leg_set(&matrix_keyboard->row4);
}

bool
mk_button_is_pressed(input_leg_t* column, output_leg_t* row) {
    leg_drop(row);
    bool button_is_pressed = leg_get(column) == 0;
    leg_set(row);
    return button_is_pressed;
}
