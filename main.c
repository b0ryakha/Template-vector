#include <stdio.h>

#include "vector.h"

typedef char* char_ptr;
vec_template_impl(char_ptr);

int main() {
    vector(int) vec = vec_create(int, 1, 2, 3, 4);

    printf("size: %zu, capacity: %zu\n", vec_size(vec), vec_capacity(vec));

    for (size_t i = 0; i < 5; ++i)
        vec_push_back(vec, i * 10);

    printf("size: %zu, capacity: %zu\n", vec_size(vec), vec_capacity(vec));
    vec_shrink_to_fit(vec);
    printf("size: %zu, capacity: %zu\n", vec_size(vec), vec_capacity(vec));

    for (size_t i = 0; i < vec_size(vec); ++i)
        printf("vec[%zu] = %d\n", i, vec_at(vec, i));

    printf("\nfront: %d\n", vec_front(vec));
    printf("back: %d\n", vec_back(vec));
    
    
    vec_clear(vec);
    printf("\nclear - size: %zu, capacity: %zu\n", vec_size(vec), vec_capacity(vec));

    vec_shrink_to_fit(vec);
    printf("shrink_to_fit - size: %zu, capacity: %zu\n\n", vec_size(vec), vec_capacity(vec));

    for (size_t i = 1; i < 5; ++i) {
        vec_push_back(vec, i * 20);
        printf("[%zu] = %zu: size: %zu, capacity: %zu\n", i, (i * 20), vec_size(vec), vec_capacity(vec));
    }

    vec_insert(vec, vec_begin(vec) + 3, 228);

    printf("\ninserted: %d, size: %zu", vec_at(vec, 3), vec_size(vec));

    printf("\nconst iterators loop:\n");
    for (const int* it = vec_cbegin(vec), *end = vec_cend(vec); it != end; ++it)
        printf("*it = %d\n", *it);

    vec_erase(vec, vec_begin(vec), vec_begin(vec) + 1);
    pop_back(vec);
    printf("\nerased & poped vec:\n");
    for (const int* it = vec_cbegin(vec), *end = vec_cend(vec); it != end; ++it)
        printf("*it = %d\n", *it);

    vec_remove(vec);

    printf("-------------------------------------------------\n");

    vector(char_ptr) strings = vec_create(char_ptr, "str1", "test", "работает!!");
    vector(char_ptr) strings2 = vec_create(char_ptr, "123", "%%%%");

    printf("strings: ");
    for (size_t i = 0; i < vec_size(strings); ++i)
        printf("\"%s\" ", vec_at(strings, i));

    printf("\nstrings2: ");
    for (size_t i = 0; i < vec_size(strings2); ++i)
        printf("\"%s\" ", vec_at(strings2, i));

    printf("\nswap:\n");
    vec_swap(strings, strings2);

    printf("strings: ");
    for (size_t i = 0; i < vec_size(strings); ++i)
        printf("\"%s\" ", vec_at(strings, i));

    printf("\nstrings2: ");
    for (size_t i = 0; i < vec_size(strings2); ++i)
        printf("\"%s\" ", vec_at(strings2, i));

    vec_remove(strings);
}