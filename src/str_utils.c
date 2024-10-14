#include "str_utils.h"
#include "arrlist_utils.h"
#include "globals.h"
#include <assert.h>
#include <stddef.h>

#define INITIAL_STRING_CAPACITY 12
#define INITIAL_STR_ARRAYLIST_CAPACITY 50

char *concat_cstrs(const char **strings, size_t count)
{
    size_t total_length = 0;
    for (int i = 0; i < count; i++) {
        total_length += strlen(strings[i]);
    }

    char *result = malloc(total_length + 1);
    if (!result)
        return NULL;

    result[0] = '\0';
    for (int i = 0; i < count; i++) {
        strcat(result, strings[i]);
    }

    return result;
}

String *make_string(void)
{
    String *string = malloc(sizeof(String));
    string->capacity = INITIAL_STRING_CAPACITY;
    string->size = 1;
    string->array = malloc(string->capacity * sizeof(char));
    string->array[0] = '\0';
    return string;
}

void cleanup_string(String *str)
{
    free(str->array);
    free(str);
}

void concat_strings(String *target, String *source)
{
    bool should_realloc = FALSE;
    while (target->size + source->size > target->capacity) {
        target->capacity *= 2;
        should_realloc = TRUE;
    }
    if (should_realloc) {
        target->array = realloc_backing_array(target->array, target->size, target->capacity, sizeof(char));
    }
    strcpy(&target->array[target->size - 1], source->array); // -1 to account for writing over the sentinel character
    target->size += source->size - 1; // -1 to account for removal of extra sentinel char
    cleanup_string(source);
}

void copy_str_into_string(String *target, char *source)
{
    size_t source_len = strlen(source) + 1; // strlen doesn't include null terminator
    bool should_realloc = FALSE;
    while (target->size + source_len > target->capacity) {
        target->capacity *= 2;
        should_realloc = TRUE;
    }
    if (should_realloc) {
        target->array = realloc_backing_array(target->array, target->size, target->capacity, sizeof(char));
    }
    strcpy(&target->array[target->size - 1], source); // -1 to account for writing over the sentinel character
    target->size += source_len - 1; // -1 to account for removal of extra sentinel char
    // We don't free the source since it might be borrowed
}

char *get_str_from_string(String *source)
{
    char *str = malloc(source->size * sizeof(char));
    strcpy(str, source->array);
    return str;
}

StrArrayList *make_str_arraylist(size_t *initial_capacity)
{
    StrArrayList *list = malloc(sizeof(StrArrayList));
    list->capacity = initial_capacity == NULL ? INITIAL_STR_ARRAYLIST_CAPACITY : *initial_capacity;
    list->size = 0;
    list->array = malloc(list->capacity * sizeof(char *));
}

void cleanup_str_arraylist(StrArrayList *list)
{
    for (size_t i = 0; i < list->size; i++) {
        free(list->array[i]);
    }
    free(list->array);
    free(list);
}

void add_str_to_arraylist(StrArrayList *list, char *str)
{
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = realloc_backing_array(list->array, list->size, list->capacity, sizeof(char *));
    }
    list->array[list->size++] = str;
}

char *get_nth_str_from_arraylist(StrArrayList *list, size_t n)
{
    assert(n >= 0 && n < list->size);
    return &list->array[n];
}
