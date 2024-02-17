#include "test.h"
#include "../vector.h"

typedef enum { Ready, Work, Error } State;
vec_template_impl(State);

void test_enums() {
    vector(State) states = vec_create(State, Ready, Error, Error);
    DUMP(states);

    vec_remove(states);
}