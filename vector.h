#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static inline void __print_error(const char* file_path, size_t line_number, bool assertion, const char* message) {
    if (assertion) return;

    char buffer[256];
    snprintf(buffer, 256, "%s:%zu: error: %s\n", file_path, line_number, message);

    fprintf(stderr, "%s", buffer);
    exit(1);
}

#define __ASSERT(assertion, message) \
    __print_error(__FILE__, __LINE__, assertion, message)

#define __IS_COMPATIBLE(T, U) _Generic(*((T*)0), U: true, default: false)
#define __SCOPE(body, ...) do { body } while(0)
#define __MAX(x, y) ((x) > (y) ? (x) : (y))

#define __SWAP(x, y) __SCOPE(                                                           \
    __ASSERT(sizeof(x) == sizeof(y), "to swap, the size of 'x' and 'y' must be equal"); \
    unsigned char buffer[sizeof(x)];                                                    \
    memcpy(buffer, &(y), sizeof(x));                                                    \
    memcpy(&(y), &(x), sizeof(x));                                                      \
    memcpy(&(x), buffer, sizeof(x));                                                    \
)                                                                                       \

#define __IT_TYPE(vec_ptr) typeof((vec_ptr)->__data)
#define __VAL_TYPE(vec_ptr) typeof(*((vec_ptr)->__data))
#define __VAL_SIZE(vec_ptr) sizeof(*((vec_ptr)->__data))

#define __RAISE_CAPACITY(vec_ptr) \
    (vec_ptr)->__capacity = ((vec_ptr)->__size * 2)

#define __REALLOC_TO_CAP(vec_ptr) \
    (vec_ptr)->__data = realloc((vec_ptr)->__data, __VAL_SIZE(vec_ptr) * (vec_ptr)->__capacity)

#define __RAISE_SIZE(vec_ptr) __SCOPE(               \
    ++((vec_ptr)->__size);                           \
    if ((vec_ptr)->__size > (vec_ptr)->__capacity) { \
        __RAISE_CAPACITY(vec_ptr);                   \
        __REALLOC_TO_CAP(vec_ptr);                   \
    }                                                \
)                                                    \

#define __VEC_VALID_CHECK(vec_ptr) \
    __ASSERT((vec_ptr) != NULL, "vector<T> expected, but got null")

#define __IT_VALID_CHECK(vec_ptr, it) __SCOPE(                                                                               \
    __ASSERT((it) != NULL, "vector<T>::iterator expected, but got null");                                                    \
    __ASSERT(__IS_COMPATIBLE(typeof(it), __IT_TYPE(vec_ptr)), "vector<T>::iterator expected, but got incompatible pointer"); \
    __ASSERT((it) >= vec_begin(vec_ptr) && (it) < vec_end(vec_ptr), "out of range");                                         \
)                                                                                                                            \

#define __COMPATIBLE_CHECK(vec_ptr1, vec_ptr2) \
    __ASSERT(__IS_COMPATIBLE(__VAL_TYPE(vec_ptr1), __VAL_TYPE(vec_ptr2)), "vectors store incompatible types.")


#define vec_template_impl(T)                                        \
    typedef struct {                                                \
        size_t __size;                                              \
        size_t __capacity;                                          \
        T* __data;                                                  \
    } vec__##T;                                                     \
    \
    vec__##T* __VECTOR_ALLOC_##T(const T* init_list, size_t size) { \
        vec__##T* vec = malloc(sizeof(vec__##T));                   \
        vec->__size = size;                                         \
        vec->__capacity = size;                                     \
        vec->__data = calloc(vec->__capacity, sizeof(T));           \
        memcpy(vec->__data, init_list, sizeof(T) * size);           \
        return vec;                                                 \
    }                                                               \

#define vector(T) vec__##T*

#define vec_create(T, ...) \
    __VECTOR_ALLOC_##T((T[]){ __VA_ARGS__ }, sizeof((T[]){ __VA_ARGS__ }) / sizeof(T))

#define vec_remove(this) __SCOPE( \
    if (this) {                   \
        free((this)->__data);     \
        (this)->__data = NULL;    \
        free(this);               \
        (this) = NULL;            \
    }                             \
)                                 \

#define vec_at(this, index) (                                             \
    __VEC_VALID_CHECK(this),                                              \
    __ASSERT(((index) >= 0 && (index) < (this)->__size), "out of range"), \
    (this)->__data[index]                                                 \
)                                                                         \

#define vec_size(this) (     \
    __VEC_VALID_CHECK(this), \
    (this)->__size           \
)                            \

#define vec_capacity(this) ( \
    __VEC_VALID_CHECK(this), \
    (this)->__capacity       \
)                            \

#define vec_reserve(this, capacity) __SCOPE(   \
    __VEC_VALID_CHECK(this);                   \
    ((this)->__capacity = __MAX(0, capacity)); \
)                                              \

#define vec_push_back(this, value) __SCOPE(       \
    __VEC_VALID_CHECK(this);                      \
    __RAISE_SIZE(this);                           \
    (this)->__data[(this)->__size - 1] = (value); \
)                                                 \

#define vec_empty(this) (    \
    __VEC_VALID_CHECK(this), \
    (this)->__size == 0      \
)                            \

#define vec_begin(this) (    \
    __VEC_VALID_CHECK(this), \
    (this)->__data           \
)                            \

#define vec_end(this) (               \
    __VEC_VALID_CHECK(this),          \
    ((this)->__data + (this)->__size) \
)                                     \

#define vec_data(this) vec_begin(this)

#define vec_front(this) (                       \
    __VEC_VALID_CHECK(this),                    \
    __ASSERT(!vec_empty(this), "out of range"), \
    *vec_begin(this)                            \
)

#define vec_back(this) (                        \
    __VEC_VALID_CHECK(this),                    \
    __ASSERT(!vec_empty(this), "out of range"), \
    *(vec_end(this) - 1)                        \
)                                               \

#define vec_cbegin(this) \
    ((const __IT_TYPE(this))vec_begin(this))

#define vec_cend(this) \
    ((const __IT_TYPE(this))vec_end(this))

#define vec_shrink_to_fit(this) __SCOPE(        \
    __VEC_VALID_CHECK(this);                    \
    if ((this)->__capacity != (this)->__size) { \
        (this)->__capacity = (this)->__size;    \
        __REALLOC_TO_CAP(this);                 \
    }                                           \
)                                               \

#define vec_clear(this) __SCOPE( \
    __VEC_VALID_CHECK(this);     \
    (this)->__size = 0;          \
)                                \

#define vec_emplace(this, it_pos, value) __SCOPE( \
    __VEC_VALID_CHECK(this);                      \
    __IT_VALID_CHECK(this, it_pos);               \
    *(it_pos) = value;                            \
)                                                 \

#define vec_insert(this, it_pos, value) __SCOPE(          \
    __VEC_VALID_CHECK(this);                              \
    __IT_VALID_CHECK(this, it_pos);                       \
    __RAISE_SIZE(this);                                   \
    const size_t offset = (it_pos) - vec_begin(this);     \
    for (size_t i = (this)->__size - 1; i >= offset; --i) \
        (this)->__data[i] = (this)->__data[i - 1];        \
    (this)->__data[offset] = (value);                     \
)                                                         \

#define vec_erase(this, ...) do {                                                            \
    __VEC_VALID_CHECK(this);                                                                 \
    if (!vec_empty(this)) {                                                                  \
        __IT_TYPE(this) argv[] = { __VA_ARGS__ };                                            \
        const size_t argc = sizeof(argv) / sizeof(*argv);                                    \
        __ASSERT(argc > 0, "too few arguments to 'vec_erase(this, first, last = first)'");   \
        __ASSERT(argc <= 2, "too many arguments to 'vec_erase(this, first, last = first)'"); \
        if (argc == 1) { __ERASE_POS(this, argv[0]); }                                       \
        else { __ERASE_RANGE(this, argv[0], argv[1]); }                                      \
    }                                                                                        \
}                                                                                            \
while(0);                                                                                    \

//TODO:
#define __ERASE_POS(this, position) __SCOPE( \
    __IT_VALID_CHECK(this, position);        \
)                                            \

//TODO: improve calloc(size, ...)
//https://blog.weghos.com/skia/include/c++/9/bits/vector.tcc.html#_ZNSt6vector8_M_eraseEN9__gnu_cxx17__normal_iteratorINSt12_Vector_baseIT_T0_E7pointerESt6vectorIS3_S4_EEE
#define __ERASE_RANGE(this, first, last) __SCOPE(                        \
    __IT_VALID_CHECK(this, first);                                       \
    __IT_VALID_CHECK(this, last);                                        \
    __ASSERT(last > first, "'last' must be the final iterator");         \
    __IT_TYPE(this) begin = vec_begin(this);                             \
    __IT_TYPE(this) end = vec_end(this);                                 \
    (this)->__size -= ((last) - (first)) + 1;                            \
    __IT_TYPE(this) new_data = calloc((this)->__size, __VAL_SIZE(this)); \
    for (__IT_TYPE(this) it = new_data; begin != end; ++begin) {         \
        if (begin < first || begin > last)                               \
            *(it++) = *begin;                                            \
    }                                                                    \
    free((this)->__data);                                                \
    (this)->__data = new_data;                                           \
)                                                                        \

//TODO: #define vec_resize(vec_ptr, size)

#define vec_pop_back(this) __SCOPE( \
    __VEC_VALID_CHECK(this);        \
    if (!vec_empty(this))           \
        --((this)->__size);         \
)                                   \

#define vec_swap(this, target) __SCOPE(               \
    __VEC_VALID_CHECK(this);                          \
    __VEC_VALID_CHECK(target);                        \
    __COMPATIBLE_CHECK(this, target);                 \
    __SWAP((this)->__size, (target)->__size);         \
    __SWAP((this)->__capacity, (target)->__capacity); \
    __SWAP((this)->__data, (target)->__data);         \
)                                                     \

#define vec_move(this, target) __SCOPE(        \
    __VEC_VALID_CHECK(this);                   \
    __VEC_VALID_CHECK(target);                 \
    __COMPATIBLE_CHECK(this, target);          \
    free((target)->__data);                    \
    (target)->__data = (this)->__data;         \
    (target)->__size = (this)->__size;         \
    (target)->__capacity = (this)->__capacity; \
    (this) = NULL;                             \
)                                              \

#define vec_copy(this, target) __SCOPE(                                                    \
    __VEC_VALID_CHECK(this);                                                               \
    __VEC_VALID_CHECK(target);                                                             \
    __COMPATIBLE_CHECK(this, target);                                                      \
    if ((target)->__capacity < (this)->__size) {                                           \
        (target)->__capacity = (this)->__size;                                             \
        __REALLOC_TO_CAP(target);                                                          \
    }                                                                                      \
    (target)->__size = (this)->__size;                                                     \
    memcpy((target)->__data, (this)->__data, sizeof(__VAL_SIZE(this)) * (target)->__size); \
)                                                                                          \


vec_template_impl(int);
vec_template_impl(short);
vec_template_impl(long);
vec_template_impl(float);
vec_template_impl(size_t);
vec_template_impl(double);
vec_template_impl(char);
vec_template_impl(bool);