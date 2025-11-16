#ifndef ESSENTIAL_H
#define ESSENTIAL_H

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(VEC_I) || defined(STRING) || defined(SB)
#include <stddef.h>
#endif

typedef enum : int { LOG_INFO = 0, LOG_WARN = 1, LOG_ERROR = 2 } LogLevel;

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define LAST_ERROR() strerror(errno)
#define FILE_LOC() __FILE__, __LINE__
#define SEPARATOR() println("───────────────────────────────────────")

#define UNUSED(x) (void)x

#include <time.h>

static inline char *current_date(char *time_buf, size_t buf_size) {
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

#define println(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#define eprintln(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)

#define fatalf(exit_code, fmt, ...)                                            \
    do {                                                                       \
        fprintf(stderr,                                                        \
                ANSI_GREY("%s:%d") " " ANSI_RED("Error") ": " fmt "\n",        \
                FILE_LOC(), ##__VA_ARGS__);                                    \
        exit(exit_code);                                                       \
    } while (0)

#define flog(fp, log_level, color, fmt, ...)                                   \
    do {                                                                       \
        char time_buf[64];                                                     \
        current_date(time_buf, sizeof(time_buf));                              \
        switch ((log_level)) {                                                 \
        case LOG_WARN:                                                         \
            if (color) {                                                       \
                fprintf(fp,                                                    \
                        "%s "                                                  \
                        "[" ANSI_YELLOW("WARN ") "] " fmt "\n",                \
                        time_buf, ##__VA_ARGS__);                              \
            } else {                                                           \
                fprintf(fp,                                                    \
                        "%s "                                                  \
                        "[WARN ] " fmt "\n",                                   \
                        time_buf, ##__VA_ARGS__);                              \
            }                                                                  \
            break;                                                             \
        case LOG_INFO:                                                         \
            if (color) {                                                       \
                fprintf(fp,                                                    \
                        "%s "                                                  \
                        "[" ANSI_GREY("INFO ") "] " fmt "\n",                  \
                        time_buf, ##__VA_ARGS__);                              \
            } else {                                                           \
                fprintf(fp,                                                    \
                        "%s "                                                  \
                        "[INFO ] " fmt "\n",                                   \
                        time_buf, ##__VA_ARGS__);                              \
            }                                                                  \
            break;                                                             \
        case LOG_ERROR:                                                        \
            if (color) {                                                       \
                fprintf(fp,                                                    \
                        "%s "                                                  \
                        "[" ANSI_RED("ERROR") "] " fmt "\n",                   \
                        time_buf, ##__VA_ARGS__);                              \
            } else {                                                           \
                fprintf(fp,                                                    \
                        "%s "                                                  \
                        "[ERROR] " fmt "\n",                                   \
                        time_buf, ##__VA_ARGS__);                              \
            }                                                                  \
            break;                                                             \
        default:                                                               \
            fatalf(1, "Not a valid log level");                                \
        };                                                                     \
    } while (0)

#define fflog(fp1, fp2, log_level, color_for_f1, color_for_f2, fmt, ...)       \
    do {                                                                       \
        flog(fp1, log_level, color_for_f1, fmt, ##__VA_ARGS__);                \
        flog(fp2, log_level, color_for_f2, fmt, ##__VA_ARGS__);                \
    } while (0)

#ifdef SB

typedef struct {
    char  *buf;
    size_t size;
    size_t capacity;

} StringBuilder;

static inline void sb_init(StringBuilder *sb) {
    sb->capacity = 16;
    sb->buf      = malloc(sb->capacity);
    if (!sb->buf) {
        fatalf(1, "memory allocation failed");
    }
    sb->size = 0;
}

void sb_append(StringBuilder *sb, char *str) {
    size_t n = strlen(str);

    while (sb->size + n > sb->capacity) {
        sb->capacity *= 2;
    }
    char *tmp = realloc(sb->buf, sb->capacity);
    if (!tmp) {
        fatalf(1, "Reallocation failed");
    }
    sb->buf = tmp;
    memcpy(sb->buf + sb->size, str, n);
    sb->size += n;
}

char *sb_to_string(StringBuilder *sb) {
    sb->buf[sb->size] = '\0';

    return sb->buf;
}

void sb_free(StringBuilder *sb) {
    free(sb->buf);
    sb->buf      = NULL;
    sb->capacity = 0;
    sb->size     = 0;
}

#endif /* ifdef SB */
#endif // ESSENTIAL_H
