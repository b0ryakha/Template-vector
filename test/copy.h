#include "test.h"
#include "../vector.h"

void test_copy() {
    vector(int) original = vec_create(int, 1, 2, 3);
    vector(int) new = NULL;

    DUMP(original);
    DUMP(new);
    vec_copy(original, new);

    ASSERT(!vec_empty(original) && !vec_empty(new));
    DUMP(original);
    DUMP(new);

    vec_remove(original);
    vec_remove(new);
}