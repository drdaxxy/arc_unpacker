#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "logger.h"
#include "assert.h"

static void fail(const char *file, int line, const char *format, ...);

static void fail(const char *file, int line, const char *format, ...)
{
    va_list args;
    char buffer[1024];

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    log_error("%s in %s:%d", buffer, file, line);
    abort();
}



void __assert_equali(const char *file, int line, int expected, int actual)
{
    if (actual != expected)
        fail(file, line, "Fatal: expected %d, got %d", expected, actual);
}

void __assert_equals(
    const char *file,
    int line,
    const char *expected,
    const char *actual)
{
    if (expected == NULL && actual == NULL)
        return;

    if (expected == NULL || actual == NULL || strcmp(actual, expected) != 0)
        fail(file, line, "Fatal: expected %s, got %s", expected, actual);
}

void __assert_equalsn(
    const char *file,
    int line,
    const char *expected,
    const char *actual,
    size_t size)
{
    if (expected == NULL && actual == NULL)
        return;

    if (expected == NULL
        || actual == NULL
        || strncmp(actual, expected, size) != 0)
    {
        fail(file, line, "Fatal: expected %s, got %s", expected, actual);
    }
}

void __assert_equalp(
    const char *file,
    int line,
    const void *expected,
    const void *actual)
{
    if (actual != expected)
        fail(file, line, "Fatal: expected %p, got %p", expected, actual);
}

void __assert_null(const char *file, int line, const void *data)
{
    if (data != NULL)
        fail(file, line, "Fatal: expected NULL, got %p", data);
}

void __assert_not_null(const char *file, int line, const void *data)
{
    if (data == NULL)
        fail(file, line, "Fatal: expected not NULL, got NULL.");
}

void __assert_that(const char *file, int line, bool expected)
{
    if (!expected)
        fail(file, line, "Fatal: expected true, got false.");
}
