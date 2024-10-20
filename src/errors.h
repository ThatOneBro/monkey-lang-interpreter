#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define CLEAN_ERRNO() (errno == 0 ? "None" : strerror(errno))
#define LOG_ERROR(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, CLEAN_ERRNO(), ##__VA_ARGS__)
#define ASSERT(A, M, ...)            \
    if (!(A)) {                      \
        LOG_ERROR(M, ##__VA_ARGS__); \
        assert(A);                   \
    }
