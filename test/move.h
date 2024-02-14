#include "test.h"
#include "../vector.h"

void test_move() {
    vector(int) original = vec_create(int, 1, 2, 3);
    vector(int) new = NULL;

    DUMP(original);
    DUMP(new);
    vec_move(original, new);

    ASSERT(original == NULL);
    DUMP(original);
    DUMP(new);

    vec_remove(original);
    vec_remove(new);
}