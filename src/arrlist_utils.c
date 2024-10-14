#include "arrlist_utils.h"
#include <stddef.h>

void *realloc_backing_array(void *array, size_t current_size, size_t new_capacity, size_t type_size)
{
    void *new_array = realloc(array, new_capacity * type_size);
    memset(new_array + current_size, 0, (new_capacity - current_size) * type_size);
    return new_array;
}
