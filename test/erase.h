#include "test.h"
#include "../vector.h"

void test_erase() {
    vector(int) vec = vec_create(int, 10, 20, 30, 40, 50, 60, 70);

    vec_it(vec) it = vec_erase(vec, vec_begin(vec));
    DUMP(vec);
    ASSERT(*it == 20);
    ASSERT(vec_front(vec) == 20);

    it = vec_erase(vec, vec_begin(vec) + 2);
    DUMP(vec);
    ASSERT(*it == 50);

    it = vec_erase(vec, vec_end(vec) - 1);
    DUMP(vec);
    ASSERT(vec_back(vec) == 60);

    it = vec_erase(vec, vec_begin(vec), vec_begin(vec) + 1);
    DUMP(vec);
    printf("[INFO]: *it = %d\n", *it);
    //ASSERT(*it == 30);

    vec_push_back(vec, 100);
    vec_push_back(vec, 80);
    vec_push_back(vec, 90);
    DUMP(vec);

    it = vec_erase(vec, vec_begin(vec) + 2, vec_begin(vec) + 3);
    printf("[INFO]: erase(vec_begin(vec) + 2, vec_begin(vec) + 3):\n");
    DUMP(vec);
    printf("[INFO]: *it = %d\n", *it);
    ASSERT(*it == 100);

    it = vec_erase(vec, vec_begin(vec), vec_begin(vec) + 3);
    printf("[INFO]: erase(vec_begin(vec), vec_begin(vec) + 3):\n");
    DUMP(vec);
    printf("[INFO]: *it = %d\n", *it);
    ASSERT(vec_front(vec) == 80);
    ASSERT(*it == 80);

    *it = 228;
    DUMP(vec);
    ASSERT(vec_front(vec) == 228);

    vec_push_back(vec, 1);
    vec_push_back(vec, 2);
    vec_push_back(vec, 3);
    DUMP(vec);

    it = vec_erase(vec, vec_begin(vec));
    DUMP(vec);
    ASSERT(*it == 90);

    it = vec_erase(vec, vec_begin(vec) + 2);
    DUMP(vec);
    ASSERT(*it == 3);

    it = vec_erase(vec, vec_end(vec) - 1);
    DUMP(vec);
    ASSERT(*it == 1);

    vec_erase(vec, vec_begin(vec), vec_end(vec));
    ASSERT(vec_empty(vec));

    vec_remove(vec);
}