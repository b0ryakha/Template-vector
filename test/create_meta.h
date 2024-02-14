#include "test.h"
#include "../vector.h"

typedef struct {
    int* ptr;
} Meta;
vec_template_impl(Meta);

Meta Meta_def_constructor() {
    INFO("default Meta()");
    Meta this;
    this.ptr = calloc(1, sizeof(this.ptr));
    return this;
}

Meta Meta_constructor(int n) {
    INFO("Meta(int)");
    Meta this;
    this.ptr = malloc(sizeof(this.ptr));
    *(this.ptr) = n;
    return this;
}

void Meta_destructor(Meta* this) {
    INFO("~Meta()");
    free(this->ptr);
}

void test_create_meta() {
    vector(Meta) vec = vec_create(Meta);
    vec_set_def_constructor(vec, Meta_def_constructor);
    vec_set_destructor(vec, Meta_destructor);

    Meta meta = Meta_constructor(1337);
    ASSERT(*(meta.ptr) == 1337);

    vec_push_back(vec, meta);
    ASSERT(*(vec_back(vec).ptr) == 1337);

    vec_resize(vec, 10);

    DUMP(vec);

    vec_erase(vec, vec_begin(vec) + 3, vec_end(vec));

    DUMP(vec);

    vec_remove(vec);
}