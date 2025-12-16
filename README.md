# Introduction #

C header for some essential macros and functions.

# Quick Start #

```bash
 wget https://raw.githubusercontent.com/BayonetArch/essen.h/refs/heads/master/essen.h
```

in a c file: 

```c
 #include "essen.h"
```


# Useful Macros and Functions #

1. Dynamic Array

header contains dynamic array which we can append to and iterate over.firstly define `ESL_DA` before include

```c
#define ESL_DA
#include "essen.h"
```

DynamicArray is an Struct like this :

```c
typedef struct {
    int   *items;
    size_t count;
    size_t capacity;
} Da;

```

we can append to it

```c
    Da xs = {0};

    esl_da_append(&xs, 20);
    esl_da_append(&xs, 40);
    esl_da_append(&xs, 60);
    
    esl_da_free(&xs);

```

We can also iterate over dynamic array 

```c
    esl_da_foreach(int, it, &xs)
    {
        size_t indx = it - xs.items; // 'it' is the iterator starting from pointer to first element
        esl_println("%ld:%d", indx, *it);
    }

```

2. StringBuilder

Similar to Dynamic Arrays it has StringBuilder.

Full example :

```c

#define ESL_SB
#include "essen.h"

int main(void)
{
    EslStringBuilder sb = {0};
    esl_sb_init(&sb);
    esl_sb_append(&sb, "Foo");
    esl_sb_append(&sb, "Bar");

    esl_println("%s", esl_sb_to_string(&sb));
    esl_sb_free(&sb);
    return EXIT_SUCCESS;
}

```


> [!TIP]
> you can define `ESL_SHORT_NAMES` to get rid of esl prefix.
> for eg: esl_da_append -> da_append
