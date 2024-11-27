#include "bytes_matrix.h"

#include <stdlib.h>

bytes_matrix_t
bm_create(uint8_t width, uint8_t height) {
    uint8_t* arr = (uint8_t*)malloc(width * (size_t)height * sizeof(uint8_t));
    bytes_matrix_t bytes_matrix = {
        ._arr = arr,
        .width = width,
        .height = height,
    };
    bm_clear(&bytes_matrix);
    return bytes_matrix;
}

void
bm_delete(const bytes_matrix_t* bytes_matrix) {
    free(bytes_matrix->_arr);
}
