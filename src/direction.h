#ifndef ATMEGA_SNAKE_DIRECTION_H_
#define ATMEGA_SNAKE_DIRECTION_H_

#define DIR_UP_MACRO    0 /* 0b00 */
#define DIR_RIGHT_MACRO 1 /* 0b01 */
#define DIR_DOWN_MACRO  2 /* 0b10 */
#define DIR_LEFT_MACRO  3 /* 0b11 */
#define DIR_NONE_MACRO  4 /* 0b100 */

typedef enum {
    DIR_UP = DIR_UP_MACRO,       /* 0b00 */
    DIR_RIGHT = DIR_RIGHT_MACRO, /* 0b01 */
    DIR_DOWN = DIR_DOWN_MACRO,   /* 0b10 */
    DIR_LEFT = DIR_LEFT_MACRO,   /* 0b11 */
    DIR_NONE = DIR_NONE_MACRO,   /* 0b100 */
} direction_t;

#endif /* ATMEGA_SNAKE_DIRECTION_H_ */
