#include "util/array.h"

#include <stdlib.h>
#include <string.h>

void array_allocate(struct Array* array, int element_size, int size) {
    const int new_size = element_size * size;

    if (new_size == 0) {
        return;
    }

    if (array->data == NULL) {
        array->data = calloc(element_size, size);
        array->capacity = new_size;
        return;
    }

    if (array->capacity >= new_size) {
        return;
    }

    const int old_size = array->capacity;
    void* new_data = realloc(array->data, new_size);
    if (new_data == NULL) {
        return;
    }

    // make new data empty
    memset((char*)new_data + old_size, 0, (size_t)(new_size - old_size));

    array->data = new_data;
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
