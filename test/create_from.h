#include "test.h"
#include "../vector.h"

void test_create_from() {
    vector(int) original = vec_create(int, 1, 2, 3);
    vector(int) copy = vec_create_from(original);

    vec_at(original, 0) = 0;
    vec_push_back(copy, 4);

    ASSERT(vec_size(original) == 3 && vec_capacity(original) == 3);
    ASSERT(vec_size(copy) > 3 && vec_capacity(copy) > 3);

    DUMP(original);
    DUMP(copy);

    vec_remove(original);
    vec_remove(copy);
}