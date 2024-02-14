#include "test.h"
#include "../vector.h"

void destructor(int* this) {
    INFO("~int()");
}

void test_remove() {
    vector(int) nums = vec_create(int, 10, 20, 30);
    vec_set_destructor(nums, destructor);

    DUMP(nums);
    vec_remove(nums);

    const vector(int) const_nums = vec_create(int, 10, 20, 30);
    vec_set_destructor(const_nums, destructor);

    DUMP(const_nums);
    vec_remove(const_nums);

    ASSERT(nums == NULL);
}