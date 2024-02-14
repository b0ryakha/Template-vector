#include "test.h"
#include "../vector.h"

void test_access() {
    vector(int) vec = vec_create(int, 1, 2, 3, 4, 5);
    const vector(int) const_vec = vec_create(int, 10, 20, 30);

    DUMP(vec);
    ASSERT(vec_at(vec, 0) == 1);
    ASSERT(vec_at(vec, 3) == 4);
    ASSERT(*vec_begin(vec) == 1);
    ASSERT(*(vec_cbegin(vec) + 4) == 5);

    vec_push_back(vec, 6);
    ASSERT(vec_back(vec) == 6);

    DUMP(const_vec);
    vec_it(const_vec) const_it = vec_begin(const_vec);
    ASSERT(*const_it == 10);

    vec_remove(vec);
}