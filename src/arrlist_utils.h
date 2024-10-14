#ifndef ARRLIST_UTILS_H
#define ARRLIST_UTILS_H

#include <memory.h>
#include <stddef.h>
#include <stdlib.h>

void *realloc_backing_array(void *array, size_t current_size, size_t new_capacity, size_t type_size);

#endif // ARRLIST_UTILS_H
