#include "test.h"

#include "create.h"
#include "create_from.h"
#include "const_create.h"
#include "create_meta.h"
#include "remove.h"
#include "move.h"
#include "copy.h"
#include "access.h"

int main() {
    CHECK(test_create);
    CHECK(test_create_from);
    CHECK(test_const_create);
    CHECK(test_create_meta);
    CHECK(test_remove);
    CHECK(test_move);
    CHECK(test_copy);
    CHECK(test_access);
}