#include "test.h"
#include "../vector.h"

typedef union {
    int a;
    double b;
} Variant;
vec_template_impl(Variant);

void test_unions() {
    vector(Variant) vec = vec_create(Variant);
    DUMP(vec);

    vec_remove(vec);
}