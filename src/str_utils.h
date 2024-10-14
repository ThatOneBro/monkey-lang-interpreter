#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

typedef struct String {
    char *array;
    size_t size;
    size_t capacity;
} String;

typedef struct StrArrayList {
    char **array;
    size_t size;
    size_t capacity;
} StrArrayList;

extern char *concat_cstrs(const char **strings, size_t count);

extern String *make_string(void);
extern void cleanup_string(String *str);
extern void concat_strings(String *target, String *source);
extern void copy_str_into_string(String *target, char *source);
extern char *get_str_from_string(String *source);

extern StrArrayList *make_str_arraylist(size_t *initial_capacity);
extern void cleanup_str_arraylist(StrArrayList *list);
extern void add_str_to_arraylist(StrArrayList *list, char *str);
extern char *get_nth_str_from_arraylist(StrArrayList *list, size_t n);

#endif // UTILS_H
