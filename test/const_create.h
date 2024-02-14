#include "test.h"
#include "../vector.h"

struct Userdata;
struct string;

void test_const_create() {
    const vector(int) empty_nums = vec_create(int);
    const vector(UserData) empty_userdata_vec = vec_create(UserData);
    const vector(string) empty_strings = vec_create(string);

    const vector(float) numbers = vec_create(float, 1.0, 15.5, 5.0 / 2.0);
    const vector(string) strings = vec_create(string, "", "str", "", "2 + 2");
    const vector(char) chars = vec_create(char, '@', '%', '*');
    const vector(UserData) userdata_vec = vec_create(UserData,
        (UserData){ .n = 0, .ch = '@' },
        (UserData){ .n = 10, .ch = '#' }
    );

    ASSERT(vec_empty(empty_nums));
    ASSERT(vec_empty(empty_userdata_vec));
    ASSERT(vec_empty(empty_strings));
    ASSERT(vec_at(userdata_vec, 0).n == 0 && vec_at(userdata_vec, 0).ch == '@');
    ASSERT(vec_at(userdata_vec, 1).n == 10 && vec_at(userdata_vec, 1).ch == '#');

    volatile size_t tmp1 = 10;
    volatile size_t* tmp2 = malloc(sizeof(size_t)); *tmp2 = 100;
    const vector(size_t) runtime_nums = vec_create(size_t, tmp1, *tmp2, *tmp2 - tmp1);

    volatile string tmp3 = "hello";
    volatile string tmp4 = " world";
    volatile string tmp5 = malloc(strlen(tmp3) + strlen(tmp4) + 1);
    strcpy(tmp5, tmp3); strcat(tmp5, tmp4);
    const vector(string) runtime_strings = vec_create(string, tmp3, tmp4, tmp5);

    ASSERT(vec_at(runtime_nums, 0) == 10);
    ASSERT(vec_at(runtime_nums, 1) == 100);
    ASSERT(vec_at(runtime_nums, 2) == 90);

    DUMP(empty_nums);
    DUMP(empty_userdata_vec);
    DUMP(empty_strings);

    DUMP(numbers);
    DUMP(strings);
    DUMP(runtime_nums);
    DUMP(chars);
    DUMP(userdata_vec);
    DUMP(runtime_strings);

    vec_remove(empty_nums);
    vec_remove(empty_strings);
    vec_remove(numbers);
    vec_remove(strings);
    vec_remove(runtime_nums);
    vec_remove(chars);
    vec_remove(empty_userdata_vec);
    vec_remove(userdata_vec);
    free(tmp2);
    vec_remove(runtime_strings);
    free(tmp5);
}