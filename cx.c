#define CX_STRIP_PREFIX
#define CX_DA
#define CX_SB

#include "cx.h"
#include <check.h>

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} Da;

#define LOG_TESTFUNCTION() LOGC(LOG_INFO, "%s", __FUNCTION__)

START_TEST(test_logging)
{
    const char *path = "test.log";

    LOG(LOG_INFO, "Some useful info..");
    FILE *fp = fopen("./foo.txt", "w");
    if (!fp)
        fatal("Could not open file:%s", LAST_ERROR());

    LOGFILE(fp, LOG_WARN, "the file name is %s", path);
    fclose(fp);
    LOGC(LOG_ERROR, "Something went wrong Error: %d", 420);
}

START_TEST(test_da)
{

    Da da = {0};

    da_append(&da, "foo");
    da_append(&da, "bar");
    da_append(&da, "baz");

    da_foreach(char *, x, &da)
    {
        size_t index = x - da.items;
        ck_assert(da.items[index] == (*x));
    }

    da_free(&da);
    ck_assert(da.items == NULL);
}

START_TEST(test_sb)
{

    StringBuilder sb = {0};
    sb_init(&sb);
    sb_append(&sb, "foo");
    sb_append(&sb, " ");
    sb_append(&sb, "bar");

    ck_assert(strcmp(sb_tostring(&sb), "foo bar") == 0);
    sb_free(&sb);
    ck_assert(sb.buf == NULL);
}

START_TEST(test_swap)
{

    int a = 10;
    int b = 20;
    SWAP(int, a, b);
    ck_assert(a == 20 && b == 10);
}
END_TEST

int main(int argc, char *argv[])
{
    Suite *s  = suite_create("Cx");
    TCase *tc = tcase_create("Core");

    tcase_add_test(tc, test_logging);
    tcase_add_test(tc, test_da);
    tcase_add_test(tc, test_sb);
    tcase_add_test(tc, test_swap);

    suite_add_tcase(s, tc);

    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    if (argc > 1) {
        const char *program = shift_args(&argc, argv);
        println("%s", program);
        println("%s", argv[0]);
    } else {
        fatalc("No argument provided.");
    }
    return failed;
}
