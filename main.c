#include <stdio.h>

#include "vector.h"

typedef char* char_ptr;
vec_template_impl(char_ptr);

typedef int* int_ptr;
vec_template_impl(int_ptr);
void destructor_int(int_ptr* elem) {
    free(*elem);
}

int main() {
    vector(int) vec = vec_create(int, 1, 2, 3, 4);

    printf("size: %zu, capacity: %zu\n", vec_size(vec), vec_capacity(vec));

    for (size_t i = 0; i < 5; ++i)
        vec_push_back(vec, i * 10);

    printf("size: %zu, capacity: %zu\n", vec_size(vec), vec_capacity(vec));

    for (size_t i = 0; i < vec_size(vec); ++i)
        printf("vec[%zu] = %d\n", i, vec_at(vec, i));

    printf("\nfront: %d\n", vec_front(vec));
    printf("back: %d\n", vec_back(vec));
    
    
    vec_clear(vec);
    printf("\nclear - size: %zu, capacity: %zu\n", vec_size(vec), vec_capacity(vec));

    vec_shrink_to_fit(vec);
    printf("shrink_to_fit - size: %zu, capacity: %zu\n\n", vec_size(vec), vec_capacity(vec));

    for (size_t i = 1; i < 10; ++i) {
        vec_push_back(vec, i * 20);
        printf("[%zu] = %zu: size: %zu, capacity: %zu\n", i, (i * 20), vec_size(vec), vec_capacity(vec));
    }

    vec_insert(vec, vec_begin(vec) + 2, 228);

    printf("\ninserted: %d, size: %zu", vec_at(vec, 3), vec_size(vec));

    printf("\nconst iterators loop:\n");
    for (const int* it = vec_cbegin(vec), *end = vec_cend(vec); it != end; ++it)
        printf("*it = %d\n", *it);

    vec_erase(vec, vec_begin(vec) + 2, vec_begin(vec) + 5);
    printf("\nerased(begin + 2, begin + 5)\n");

    for (const int* it = vec_cbegin(vec), *end = vec_cend(vec); it != end; ++it)
        printf("*it = %d\n", *it);

    vec_erase(vec, vec_begin(vec) + 2);
    printf("\n\nerased(begin + 2)\n");

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
    vec_remove(strings2);

    vector(int) vec2 = vec_create(int, 1, 2, 3, 4);

    printf("\nemplace test:\n");
    for (size_t i = 0; i < vec_size(vec2); ++i)
        printf("%d ", vec_at(vec2, i));

    printf("\n");
    vec_emplace(vec2, vec_begin(vec2) + 1, 10);

    for (size_t i = 0; i < vec_size(vec2); ++i)
        printf("%d ", vec_at(vec2, i));

    vec_remove(vec2);

    printf("\n\nmove & copy:\n");
    vector(int) a = vec_create(int, 1, 2, 3);
    vector(int) b = vec_create(int, 6, 7, 8, 9, 10);

    printf("a: ");
    for (size_t i = 0; i < vec_size(a); ++i)
        printf("%d ", vec_at(a, i));

    printf("\nb  (sz: %zu, cp: %zu): ", vec_size(b), vec_capacity(b));
    for (size_t i = 0; i < vec_size(b); ++i)
        printf("%d ", vec_at(b, i));

    vector(int) b_copy = vec_create(int);
    printf("\nb_copy (sz: %zu, cp: %zu)", vec_size(b_copy), vec_capacity(b_copy));
    vec_copy(b, b_copy);
    vec_move(a, b);

    printf("\n\nb (a -> b) (sz: %zu, cp: %zu): ", vec_size(b), vec_capacity(b));
    for (size_t i = 0; i < vec_size(b); ++i)
        printf("%d ", vec_at(b, i));

    printf("\nb_copy (sz: %zu, cp: %zu): ", vec_size(b_copy), vec_capacity(b_copy));
    for (size_t i = 0; i < vec_size(b_copy); ++i)
        printf("%d ", vec_at(b_copy, i));

    vec_remove(a);
    vec_remove(b);
    vec_remove(b_copy);

    const vector(int) const_vec = vec_create(int, -1, -2, -3);

    printf("\n\nconst_vec.begin() = %d\n", *vec_cbegin(const_vec));

    volatile int_ptr ptr1 = malloc(sizeof(int));
    *ptr1 = 10;

    volatile int_ptr ptr2 = malloc(sizeof(int));
    *ptr2 = 20;

    volatile int_ptr ptr3 = malloc(sizeof(int));
    *ptr3 = 30;

    vector(int_ptr) ptrs = vec_create(int_ptr, ptr1, ptr2);
    vec_set_elem_destructor(ptrs, destructor_int);

    vec_push_back(ptrs, ptr3);

    for (size_t i = 0; i < vec_size(ptrs); ++i)
        printf("%d ", *vec_at(ptrs, i));

    vec_remove(ptrs);
}