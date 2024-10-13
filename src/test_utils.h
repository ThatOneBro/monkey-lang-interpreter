#include <stdio.h>

#define MAX_TESTS 1000

typedef struct Test {
    void (*func)(void);
    const char *name;
} Test;

#define INIT_TEST_HARNESS()                                              \
    static Test tests[MAX_TESTS];                                        \
    static int test_count = 0;                                           \
    static void add_test(void (*test_func)(void), const char *test_name) \
    {                                                                    \
        if (test_count < MAX_TESTS) {                                    \
            tests[test_count].func = test_func;                          \
            tests[test_count].name = test_name;                          \
            test_count++;                                                \
        }                                                                \
    }

#define TEST_CASE(name)                                                  \
    static void test_##name(void);                                       \
    static const char *test_name_##name = #name;                         \
    static void register_test_##name(void) __attribute__((constructor)); \
    static void register_test_##name(void)                               \
    {                                                                    \
        add_test(test_##name, test_name_##name);                         \
    }                                                                    \
    static void test_##name(void)

#define RUN_TESTS()                                         \
    int main(void)                                          \
    {                                                       \
        int i;                                              \
        int passed = 0;                                     \
        for (i = 0; i < test_count; i++) {                  \
            printf("Running test_%s...\n", tests[i].name);  \
            tests[i].func();                                \
            passed++;                                       \
        }                                                   \
        printf("%d/%d tests passed\n", passed, test_count); \
        return (passed == test_count) ? 0 : 1;              \
    }
