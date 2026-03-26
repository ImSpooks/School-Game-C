#include "util/array.h"

#include <stdlib.h>

void array_allocate(struct Array* array, int element_size, int size) {
    const int new_size = element_size * size;

    if (new_size == 0) {
        return;
    }

    if (array->data == NULL) {
        array->data = malloc(new_size);
        array->capacity = new_size;
        return;
    }

    if (array->capacity >= new_size) {
        return;
    }

    array->data = realloc(array->data, new_size);
    array->capacity = new_size;
}

void array_free(struct Array *array) {
    array->capacity = 0;
    if (array->data !=  NULL) {
        free(array->data);
    }

}

struct Array array_empty() {
    return (struct Array) {};
}
