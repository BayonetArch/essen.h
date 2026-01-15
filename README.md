# Introduction #

C header for some essential macros and functions.

# Quick Start #

```bash
wget https://raw.githubusercontent.com/BayonetArch/cx.h/refs/heads/master/cx.h
```

in a c file: 

```c
#include "cx.h"
```


# Useful Macros and Functions #

1. Dynamic Array

header contains dynamic array which we can append to and iterate over.firstly define `CX_DA` before include

```c
#define CX_DA
#include "cx.h"
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

    cx_da_append(&xs, 20);
    cx_da_append(&xs, 40);
    cx_da_append(&xs, 60);
    
    cx_da_free(&xs);

```

We can also iterate over dynamic array 

```c
    cx_da_foreach(int, it, &xs)
    {
        size_t indx = it - xs.items; // 'it' is the iterator starting from pointer to first element
        cx_println("%ld:%d", indx, *it);
    }

```

2. StringBuilder

Similar to Dynamic Arrays it has StringBuilder.

Full example :

```c

#define CX_SB
#include "cx.h"

int main(void)
{
    CxStringBuilder sb = {0};
    cx_sb_init(&sb);
    cx_sb_append(&sb, "Foo");
    cx_sb_append(&sb, "Bar");

    cx_println("%s", cx_sb_to_string(&sb));
    cx_sb_free(&sb);
    return EXIT_SUCCESS;
}

```


> [!TIP]
> you can define `CX_STRIP_PREFIX` to get rid of cx prefix.
> for eg: cx_da_append -> da_append
