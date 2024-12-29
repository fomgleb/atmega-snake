#ifndef ATMEGA_SNAKE_MATRIX_H_
#define ATMEGA_SNAKE_MATRIX_H_

#include <inttypes.h>
#include <stdlib.h>
#include "real_inline.h"

#define DEFINE_MATRIX_TYPE(TYPE, SUFFIX)                                                                               \
    typedef struct {                                                                                                   \
        TYPE* _arr;                                                                                                    \
        uint8_t width;                                                                                                 \
        uint8_t height;                                                                                                \
    } matrix_##SUFFIX##_t;                                                                                             \
                                                                                                                       \
    static REAL_INLINE matrix_##SUFFIX##_t mx_##SUFFIX##_create(uint8_t w, uint8_t h) {                                \
        TYPE* arr = (TYPE*)malloc(w * (size_t)h * sizeof(TYPE));                                                       \
        matrix_##SUFFIX##_t matrix = {                                                                                 \
            ._arr = arr,                                                                                               \
            .width = w,                                                                                                \
            .height = h,                                                                                               \
        };                                                                                                             \
        for (uint8_t x = 0; x < w; x++) {                                                                              \
            for (uint8_t y = 0; y < h; y++) {                                                                          \
                arr[y * w + x] = 0;                                                                                    \
            }                                                                                                          \
        }                                                                                                              \
        return matrix;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    static REAL_INLINE void mx_##SUFFIX##_delete(const matrix_##SUFFIX##_t* m) { free(m->_arr); }                      \
                                                                                                                       \
    static REAL_INLINE void mx_##SUFFIX##_assign(const matrix_##SUFFIX##_t* m, uint8_t x, uint8_t y, TYPE value) {     \
        m->_arr[y * m->width + x] = value;                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    static REAL_INLINE TYPE mx_##SUFFIX##_get(const matrix_##SUFFIX##_t* m, uint8_t x, uint8_t y) {                    \
        return m->_arr[y * m->width + x];                                                                              \
    }

DEFINE_MATRIX_TYPE(uint8_t, u8)
DEFINE_MATRIX_TYPE(uint16_t, u16)

#define mx_create(type, width, height)                                                                                 \
    _Generic(((type)0), uint8_t: mx_u8_create, uint16_t: mx_u16_create)(width, height)

#define mx_delete(matrix_ptr)                                                                                          \
    _Generic((matrix_ptr), matrix_u8_t *: mx_u8_delete, matrix_u16_t *: mx_u16_delete)(matrix_ptr)

#define mx_set(matrix_ptr, x, y, value)                                                                                \
    _Generic((matrix_ptr), matrix_u8_t *: mx_u8_assign, matrix_u16_t *: mx_u16_assign)(matrix_ptr, x, y, value)

#define mx_get(matrix_ptr, x, y)                                                                                       \
    _Generic((matrix_ptr),                                                                                             \
        matrix_u8_t *: mx_u8_get,                                                                                      \
        matrix_u16_t *: mx_u16_get,                                                                                    \
        const matrix_u8_t*: mx_u8_get,                                                                                 \
        const matrix_u16_t*: mx_u16_get)(matrix_ptr, x, y)

#endif /* ATMEGA_SNAKE_MATRIX_H_ */
