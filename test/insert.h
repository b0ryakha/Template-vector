#include "test.h"
#include "../vector.h"

void test_insert() {
    vector(int) vec = vec_create(int, 1, 2, 3);
    DUMP(vec);

    vec_it(vec) it = vec_insert(vec, vec_begin(vec), 0);
    DUMP(vec);

    printf("[INFO]: *it = %d\n", *it);
    printf("[INFO]: *(it + 1) = %d\n", *(it + 1));
    printf("[INFO]: *(begin + 1) = %d\n", *(vec_begin(vec) + 1));
    ASSERT(vec_front(vec) == 0);
    ASSERT(*(it + 1) == 1);

    *it = -1;
    DUMP(vec);

    it = vec_insert(vec, vec_begin(vec), 3, 5);
    DUMP(vec);

    ASSERT(vec_front(vec) == 5);
    ASSERT(*(it + 3) == -1);

    printf("[INFO]: *it = %d\n", *it);
    *it = -5;

    DUMP(vec);
    ASSERT(vec_front(vec) == -5);

    it = vec_insert(vec, vec_begin(vec) + 2, 2, 228);
    DUMP(vec);
    ASSERT(*(it - 1) == 5);
    ASSERT(*(it + 2) == 5);

    it = vec_insert(vec, vec_end(vec), -50);
    DUMP(vec);
    ASSERT(vec_back(vec) == -50);

    it = vec_insert(vec, vec_end(vec), 3, -10);
    DUMP(vec);

    it = vec_insert(vec, vec_begin(vec) + vec_size(vec), 69);
    DUMP(vec);
    ASSERT(vec_back(vec) == 69);

    it = vec_insert(vec, vec_begin(vec) + vec_size(vec), 100);
    DUMP(vec);
    ASSERT(vec_back(vec) == 100);

    it = vec_insert(vec, vec_begin(vec) + 3, 5, 3030);
    DUMP(vec);
    printf("[INFO]: *it = %d\n", *it);
    ASSERT(*it == 3030);
    ASSERT(*(it + 3) == 3030);

    vec_remove(vec);
}