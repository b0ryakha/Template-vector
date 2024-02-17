#include "test.h"
#include "../vector.h"

typedef struct {
    int* ptr;
} Meta;
vec_template_impl(Meta);

Meta Meta_constructor(int n) {
    printf("[INFO]: Meta(int)\n");
    Meta this;
    this.ptr = malloc(sizeof(this.ptr));
    *(this.ptr) = n;
    return this;
}

static size_t destructor_call_count = 0;

void Meta_destructor(Meta* this) {
    printf("[INFO]: ~Meta()\n");
    ++destructor_call_count;
    free(this->ptr);
}

void test_create_meta() {
    vector(Meta) vec = vec_create(Meta);
    vec_set_destructor(vec, Meta_destructor);

    vec_push_back(vec, Meta_constructor(1337));
    ASSERT(*(vec_back(vec).ptr) == 1337);

    vec_resize(vec, 5);
    DUMP(vec);

    vec_remove(vec);

    const vector(Meta) const_vec = vec_create(Meta, Meta_constructor(10), Meta_constructor(228));
    vec_set_destructor(const_vec, Meta_destructor);
    vec_remove(const_vec);

    ASSERT(destructor_call_count == (5 + 2));
}