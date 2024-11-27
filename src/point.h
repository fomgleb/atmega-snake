#ifndef ATMEGA_SNAKE_POINT_H_
#define ATMEGA_SNAKE_POINT_H_

#include <inttypes.h>

#define DEFINE_POINT_TYPE_(TYPE, NAME)                                                                                 \
    typedef struct _point_##NAME##_s_ {                                                                                \
        TYPE x;                                                                                                        \
        TYPE y;                                                                                                        \
    } point_##NAME##_t;

DEFINE_POINT_TYPE_(uint8_t, u8)
DEFINE_POINT_TYPE_(int8_t, i8)

#endif /* ATMEGA_SNAKE_POINT_H_ */
