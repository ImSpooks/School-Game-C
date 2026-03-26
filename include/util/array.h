#ifndef SCHOOLGAMEC_ARRAY_H
#define SCHOOLGAMEC_ARRAY_H

struct Array {
    void* data;
    int size;
    int capacity;
};

void array_allocate(struct Array* array, int element_size, int size);
void array_free(struct Array* array);
struct Array array_empty();

#endif //SCHOOLGAMEC_ARRAY_H