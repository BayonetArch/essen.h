#define CX_STRIP_PREFIX
#define CX_DA
#define CX_SB
#include "cx.h"

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} Da;

#define LOG_TESTFUNCTION() (LOGC(LOG_INFO, "%s", __FUNCTION__))

void test_logging()
{
    LOG_TESTFUNCTION();
    const char *path = "test.log";

    LOG(LOG_INFO, "Some useful info..");
    FILE *fp = fopen("./foo.txt", "w");
    if (!fp)
        fatal("Could not open file:%s", LAST_ERROR());

    LOGFILE(fp, LOG_WARN, "the file name is %s", path);
    fclose(fp);

    LOGC(LOG_ERROR, "Something went wrong Error:%d", 420);
}
void test_da()
{

    LOG_TESTFUNCTION();
    Da da = {0};

    da_append(&da, "foo");
    da_append(&da, "bar");
    da_append(&da, "baz");

    da_foreach(char *, x, &da)
    {
        size_t index = x - da.items;
        println("item at index %ld = %s", index, (*x));
    }
    da_free(&da);
}

void test_sb()
{

    LOG_TESTFUNCTION();
    StringBuilder sb = {0};
    sb_init(&sb);
    sb_append(&sb, "foo");

    sb_append(&sb, " ");

    sb_append(&sb, "bar");
    println("%s", sb_tostring(&sb));
    sb_free(&sb);
}

void test_swap()
{

    LOG_TESTFUNCTION();
    int a = 10;
    int b = 20;

    println("before swapping a:b %d:%d", a, b);
    SWAP(int, a, b);

    println("after swapping a:b %d:%d", a, b);
}

int main(int argc, char *argv[])
{
    test_logging();
    test_da();
    test_sb();
    test_swap();

    if (argc > 1) {
        const char *program = shift_args(&argc, argv);
        println("%s", program);
        println("%s", argv[0]);
    } else {
        fatalc("No argument provided.");
    }

    return 0;
}
