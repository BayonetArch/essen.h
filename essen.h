#ifndef ESSENTIAL_H
#define ESSENTIAL_H

#include <stdio.h>
#include <stdlib.h>

#if defined(VEC_I) || defined(STRING) || defined(SB)
#include <stddef.h>
#endif

#define println(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

typedef enum { WARN, INFO, ERROR } LogLevel;

#define logger(log_level, fmt, ...)                                            \
    do {                                                                       \
        switch ((log_level)) {                                                 \
        case WARN:                                                             \
            println("[\x1b[1;33mw\x1b[0m]: " fmt, ##__VA_ARGS__);              \
            break;                                                             \
        case INFO:                                                             \
            println("[\x1b[90mi\x1b[0m]: " fmt, ##__VA_ARGS__);                \
            break;                                                             \
        case ERROR:                                                            \
            println("[\x1b[1;31me\x1b[0m]: " fmt, ##__VA_ARGS__);              \
            break;                                                             \
        default:                                                               \
            println("Err:Not a valid log level");                              \
            exit(1);                                                           \
        };                                                                     \
    } while (0)

#define print_stderr(fmt, ...)                                                 \
    logger(ERROR, fmt, ##__VA_ARGS__);                                         \
    exit(1)

#define LOCATION() __FILE__, __LINE__
#define SEP printf("───────────────────────────────────────\n")

#ifdef VEC_I
typedef struct {
    int *items;
    size_t size;
    size_t capacity;
} VecInt;

static inline void veci_init(VecInt *v, size_t initial_cap) {
    v->items = (int *)malloc(sizeof(int) * initial_cap);
    if (v->items == NULL) {
        print_stderr("Memory allocation failed");
    }
    v->capacity = initial_cap;
    v->size = 0;
}

static inline void veci_push(VecInt *s, int item) {
    if (s->size == s->capacity) {
        s->capacity *= 2;
        int *data = (int *)realloc(s->items, s->capacity * sizeof(int));
        if (data == NULL) {
            print_stderr("Memory allocation failed");
        }
        s->items = data;
    }

    s->items[s->size++] = item;
}

static inline int veci_get(VecInt *s, size_t indx) {
    if (indx < 0 || indx >= s->size) {
        print_stderr("Trying to access out of bound index");
    }

    return s->items[indx];
}

static inline void veci_free(VecInt *s) {
    free(s->items);
    s->items = NULL;
    s->capacity = 0;
    s->size = 0;
}

#endif // VEC_I

#ifdef STRING
typedef struct {
    char **str;
    size_t size;
    size_t capacity;
} String;

static inline void string_init(String *s, int initial_cap) {
    s->str = (char **)malloc(sizeof(char *) * initial_cap);
    if (s->str == NULL) {
        print_stderr("Memory allocation failed");
    }
    s->capacity = initial_cap;
    s->size = 0;
}

static inline void string_append(String *s, char *str) {
    if (s->capacity == s->size) {
        s->capacity *= 2;
        char **i = (char **)realloc(s->str, s->capacity * sizeof(char *));
        if (i == NULL) {
            print_stderr("Memory allocation failed");
        }
        s->str = i;
    }
    s->str[s->size] = strdup(str);
    if (!s->str[s->size]) {
        print_stderr("strdup failed");
    }
    s->size++;
}

static inline char *string_get(String *s, size_t indx) {
    if (indx < 0 || indx >= s->size) {
        print_stderr("Invalid index");
    }

    return s->str[indx];
}

static inline void string_free(String *s) {
    for (size_t i = 0; i < s->size; i++) {
        free(s->str[i]);
    }
    free(s->str);

    s->str = NULL;
    s->size = 0;
    s->capacity = 0;
}

#endif // STRING

#ifdef SB

typedef struct {
    char *buf;
    size_t size;
    size_t capacity;

} StringBuilder;

static inline void sb_init(StringBuilder *sb) {
    sb->capacity = 16;
    sb->buf = malloc(sb->capacity);
    if (!sb->buf) {
        print_stderr("memory allocation failed");
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
        print_stderr("%d Reallocation failed", __LINE__);
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
    sb->buf = NULL;
    sb->capacity = 0;
    sb->size = 0;
}

#endif /* ifdef SB */
#endif // ESSENTIAL_H
