#ifndef CX_H
#define CX_H

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef CX_STRIP_PREFIX

#define LogLevel     CxLogLevel;
#define LOG_INFO     CX_LOG_INFO
#define LOG_WARN     CX_LOG_WARN
#define LOG_ERROR    CX_LOG_ERROR
#define UNUSED       CX_UNUSED
#define SWAP         CX_SWAP
#define LAST_ERROR   CX_LAST_ERROR
#define FILE_LOC     CX_FILE_LOC
#define current_date cx_current_date
#define eprintln     cx_eprintln
#define println      cx_println
#define fatal        cx_fatal
#define fatalc       cx_fatalc
#define LOG          CX_LOG
#define LOGFILE      CX_LOGFILE
#define LOGC         CX_LOGC
#define shift_args   cx_shift_args

#endif // CX_STRIP_PREFIX

typedef enum : int {
    CX_LOG_INFO  = 0,
    CX_LOG_WARN  = 1,
    CX_LOG_ERROR = 2
} CxLogLevel;

#define CX_ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define CX_LAST_ERROR() (strerror(errno))
#define CX_FILE_LOC()   __FILE__, __LINE__

#define CX_SWAP(T, a, b)                                                       \
    do {                                                                       \
        T t = a;                                                               \
        a   = b;                                                               \
        b   = t;                                                               \
    } while (0)

#define CX_UNUSED(x) (void)x

#include <time.h>

static inline char *cx_current_date(char *time_buf, size_t buf_size)
{
    time_t    t    = time(NULL);
    struct tm date = *localtime(&t);
    strftime(time_buf, buf_size, "%Y-%m-%d %I:%M:%S", &date);
    return time_buf;
}

/* Basic colors */
// clang-format off
#define ANSI_RESET "\x1b[0m"

#define ANSI_RED(x)                 "\x1b[0;31m" x ANSI_RESET
#define ANSI_RED_BOLD(x)            "\x1b[1;31m" x ANSI_RESET
#define ANSI_GREEN(x)               "\x1b[0;32m" x ANSI_RESET
#define ANSI_GREEN_BOLD(x)          "\x1b[1;32m" x ANSI_RESET
#define ANSI_GREY(x)                "\x1b[0;90m" x ANSI_RESET
#define ANSI_GREY_BOLD(x)           "\x1b[1;90m" x ANSI_RESET
#define ANSI_YELLOW(x)              "\x1b[0;33m" x ANSI_RESET
#define ANSI_YELLOW_BOLD(x)         "\x1b[1;33m" x ANSI_RESET
// clang-format on

#define cx_println(fmt, ...)  printf(fmt "\n", ##__VA_ARGS__)
#define cx_eprintln(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)

#define cx_fatal(fmt, ...)                                                     \
    do {                                                                       \
        fprintf(stderr,                                                        \
                "%s:%d:"                                                       \
                " "                                                            \
                "fatal error: " fmt "\n",                                      \
                CX_FILE_LOC(), ##__VA_ARGS__);                                 \
        exit(1);                                                               \
    } while (0)

#define cx_fatalc(fmt, ...)                                                    \
    do {                                                                       \
        fprintf(stderr,                                                        \
                ANSI_GREY("%s:%d:") " " ANSI_RED("fatal error") ": " fmt "\n",  \
                CX_FILE_LOC(), ##__VA_ARGS__);                                 \
        exit(1);                                                               \
    } while (0);

// file logging
static void cx__logfile__(FILE *fp, CxLogLevel level, bool color,
                          const char *fmt, ...)
{
    char time_buf[64];
    cx_current_date(time_buf, sizeof(time_buf));
    va_list args;
    va_start(args, fmt);

    switch (level) {
    case CX_LOG_WARN:
        if (color) {
            fprintf(fp,
                    "%s "
                    "[" ANSI_YELLOW("WARN") "] ",
                    time_buf);
        } else {
            fprintf(fp,
                    "%s "
                    "[WARN] ",
                    time_buf);
        }
        break;
    case CX_LOG_INFO:
        if (color) {
            fprintf(fp,
                    "%s "
                    "[" ANSI_GREEN("INFO") "] ",
                    time_buf);
        } else {
            fprintf(fp,
                    "%s "
                    "[INFO] ",
                    time_buf);
        }
        break;
    case CX_LOG_ERROR:
        if (color) {
            fprintf(fp,
                    "%s "
                    "[" ANSI_RED("ERROR") "] ",
                    time_buf);
        } else {
            fprintf(fp,
                    "%s "
                    "[ERROR] ",
                    time_buf);
        }
        break;
    default:
        cx_fatal("Not a valid log level");
    }

    vfprintf(fp, fmt, args);
    fprintf(fp, "\n");
    va_end(args);
}

#define CX_LOG(level, fmt, ...)                                                \
    cx__logfile__(stderr, level, false, fmt, ##__VA_ARGS__)

#define CX_LOGC(level, fmt, ...)                                               \
    cx__logfile__(stderr, level, true, fmt, ##__VA_ARGS__)

#define CX_LOGFILE(fp, level, fmt, ...)                                        \
    cx__logfile__(fp, level, false, fmt, ##__VA_ARGS__)

static inline const char *cx_shift_args(int *argc, char **argv)
{
    if (*argc == 1)
        return NULL;

    const char *program = argv[0];
    for (int i = 0; i < *argc - 1; ++i) {
        argv[i] = argv[i + 1];
    }

    (*argc)--;
    return program;
}

#ifdef CX_DA

#define CX_DA_DEF_CAP 256

#define cx_da_append(xs, x)                                                    \
    do {                                                                       \
        if ((xs)->capacity == 0) {                                             \
            (xs)->capacity = CX_DA_DEF_CAP;                                    \
            (xs)->items    = malloc((xs)->capacity * sizeof(*(xs)->items));    \
            if (!(xs)->items)                                                  \
                cx_fatal("memory allocation failed");                          \
        } else if ((xs)->count >= (xs)->capacity) {                            \
            (xs)->capacity *= 2;                                               \
            void *tmp =                                                        \
                realloc((xs)->items, (xs)->capacity * sizeof(*(xs)->items));   \
            if (!tmp)                                                          \
                cx_fatal("memory reallocation failed");                        \
            (xs)->items = tmp;                                                 \
        }                                                                      \
        (xs)->items[(xs)->count++] = x;                                        \
    } while (0)

#define cx_da_free(xs)                                                         \
    do {                                                                       \
        free((xs)->items);                                                     \
        ((xs))->items    = NULL;                                               \
        ((xs))->capacity = 0;                                                  \
        ((xs))->count    = 0;                                                  \
    } while (0)

// index = it - da.items
#define cx_da_foreach(Type, it, da)                                            \
    for (Type *it = (da)->items; it < (da)->items + (da)->count; ++it)

#define cx_da_free_heap(type, xs)                                              \
    do {                                                                       \
        da_foreach(type, it, xs) { free(*it); }                                \
        da_free((xs));                                                         \
        (xs)->items    = NULL;                                                 \
        (xs)->capacity = 0;                                                    \
        (xs)->count    = 0;                                                    \
    } while (0);

#ifdef CX_STRIP_PREFIX

#define da_append(xs, x)         cx_da_append(xs, x)
#define da_free                  cx_da_free
#define da_foreach(Type, it, da) cx_da_foreach(Type, it, da)
#define da_free_heap(type, xs)   cx_da_free_heap(type, xs)

#endif /* ifdef CX_STRIP_PREFIX */

#endif // CX_DA

// #ifdef CX_SB

#define CX_DEF_SB_CAP 64
#include <stddef.h>

typedef struct {
    char  *buf;
    size_t size;
    size_t capacity;

} CxStringBuilder;

static inline void cx_sb_init(CxStringBuilder *sb)
{
    sb->capacity = CX_DEF_SB_CAP;
    sb->buf      = (char *)malloc(sb->capacity);
    if (!sb->buf) {
        cx_fatal("memory allocation failed");
    }
    sb->size = 0;
}

static inline void cx_sb_append(CxStringBuilder *sb, char *str)
{
    size_t n = strlen(str);

    while (sb->size + n > sb->capacity) {
        sb->capacity *= 2;
    }
    char *tmp = (char *)realloc(sb->buf, sb->capacity);
    if (!tmp) {
        cx_fatal("Reallocation failed");
    }
    sb->buf = tmp;
    memcpy(sb->buf + sb->size, str, n);
    sb->size += n;
}

static inline char *cx_sb_tostring(CxStringBuilder *sb)
{
    sb->buf[sb->size] = '\0';

    return sb->buf;
}

static inline void cx_sb_free(CxStringBuilder *sb)
{
    free(sb->buf);
    sb->buf      = NULL;
    sb->capacity = 0;
    sb->size     = 0;
}

#ifdef CX_STRIP_PREFIX

#define sb_init     cx_sb_init
#define sb_append   cx_sb_append
#define sb_tostring cx_sb_tostring
#define sb_free     cx_sb_free
typedef CxStringBuilder StringBuilder;

#endif /* ifdef CX_STRIP_PREFIX */

// #endif /* ifdef CX_SB */
#endif // CX_H
