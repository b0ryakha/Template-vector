#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//TODO: remove __MARK_AS_NON_CONST where the compiler can handle
//TODO: add vec_equal(this, target)

static inline void __print_error(const char* file_path, size_t line_number, bool assertion, const char* message) {
    if (assertion) return;

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s:%zu: error: %s\n", file_path, line_number, message);

    fprintf(stderr, "%s", buffer);
    exit(1);
}

#define __ASSERT(assertion, message) \
    __print_error(__FILE__, __LINE__, assertion, message)

#define __IS_COMPATIBLE(T, U) \
    _Generic(*((T*)NULL), U: true, default: false)

#define __IS_CONST(L_value) \
    _Generic(&(L_value), typeof(L_value) const*: true, default: false)

#define __SCOPE(body, ...) do { body } while(0)
#define __MAX(x, y) ((x) > (y) ? (x) : (y))

#define __SWAP(x, y) __SCOPE(                                                           \
    __ASSERT(sizeof(x) == sizeof(y), "to swap, the size of 'x' and 'y' must be equal"); \
    unsigned char __buffer[sizeof(x)];                                                  \
    memcpy(__buffer, &(y), sizeof(x));                                                  \
    memcpy(&(y), &(x), sizeof(x));                                                      \
    memcpy(&(x), __buffer, sizeof(x));                                                  \
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

#define __IT_VALID_CHECK(vec_ptr, it) __SCOPE(                                                                                 \
    __ASSERT((it) != NULL, "vector<T>::iterator expected, but got null");                                                      \
    __ASSERT(__IS_COMPATIBLE(typeof(*it), __VAL_TYPE(vec_ptr)), "vector<T>::iterator expected, but got incompatible pointer"); \
    __ASSERT((it) >= vec_cbegin(vec_ptr) && (it) < vec_cend(vec_ptr), "out of range");                                         \
)                                                                                                                              \

#define __IT_NON_CONST_CHECK(it) \
    __ASSERT(!__IS_CONST(*(it)), "expected read-only iterator")

#define __COMPATIBLE_CHECK(vec_ptr1, vec_ptr2) \
    __ASSERT(__IS_COMPATIBLE(__VAL_TYPE(vec_ptr1), __VAL_TYPE(vec_ptr2)), "vectors store incompatible types.")

#define __MARK_AS_NON_CONST(vec_ptr) \
    __ASSERT(!__IS_CONST(*vec_ptr), "attempt to call non-const method on a read-only object")

#define __DESTRUCT_ALL(this) __SCOPE(                        \
    if ((this)->__elem_destructor != NULL) {                 \
        for (size_t __i = 0; __i < (this)->__size; ++__i)    \
            (this)->__elem_destructor(&(this)->__data[__i]); \
    }                                                        \
)                                                            \


#define vec_template_impl(T)                                        \
    typedef struct {                                                \
        size_t __size;                                              \
        size_t __capacity;                                          \
        void (*__elem_destructor)(T* elem);                         \
        T* __data;                                                  \
    } vec__##T;                                                     \
    \
    vec__##T* __VECTOR_ALLOC_##T(const T* init_list, size_t size) { \
        vec__##T* vec = malloc(sizeof(vec__##T));                   \
        vec->__size = size;                                         \
        vec->__capacity = size;                                     \
        vec->__elem_destructor = NULL;                              \
        vec->__data = calloc(vec->__capacity, sizeof(T));           \
        memcpy(vec->__data, init_list, sizeof(T) * size);           \
        return vec;                                                 \
    }                                                               \

#define vector(T) vec__##T*

#define vec_create(T, ...) \
    __VECTOR_ALLOC_##T((T[]){ __VA_ARGS__ }, sizeof((T[]){ __VA_ARGS__ }) / sizeof(T))

#define vec_remove(this) __SCOPE(  \
    if (this != NULL) {            \
        __DESTRUCT_ALL(this);      \
        free((void*)this->__data); \
        free((void*)this);         \
        this = NULL;               \
    }                              \
)                                  \

//TODO: need return const __data if 'this' is const
#define __AT(this, index) (                                               \
    __VEC_VALID_CHECK(this),                                              \
    __ASSERT(((index) >= 0 && (index) < (this)->__size), "out of range"), \
    ((this)->__data + (index))                                            \
)                                                                         \

#define vec_at(this, index) (*__AT(this, index))

#define vec_size(this) (     \
    __VEC_VALID_CHECK(this), \
    (this)->__size           \
)                            \

#define vec_capacity(this) ( \
    __VEC_VALID_CHECK(this), \
    (this)->__capacity       \
)                            \

#define vec_reserve(this, capacity) __SCOPE(                    \
    __VEC_VALID_CHECK(this);                                    \
    __MARK_AS_NON_CONST(this);                                  \
    ((this)->__capacity = __MAX((this)->__capacity, capacity)); \
    __REALLOC_TO_CAP(this);                                     \
)                                                               \

#define vec_push_back(this, value) __SCOPE(       \
    __VEC_VALID_CHECK(this);                      \
    __MARK_AS_NON_CONST(this);                    \
    __RAISE_SIZE(this);                           \
    (this)->__data[(this)->__size - 1] = (value); \
)                                                 \

#define vec_empty(this) (    \
    __VEC_VALID_CHECK(this), \
    (this)->__size == 0      \
)                            \

#define vec_cbegin(this) \
    ((const __VAL_TYPE(this)*)(this)->__data)

#define vec_cend(this) \
    ((const __VAL_TYPE(this)*)((this)->__data + (this)->__size))

#define vec_begin(this) (      \
    __VEC_VALID_CHECK(this),   \
    __MARK_AS_NON_CONST(this), \
    (this)->__data             \
)                              \

#define vec_end(this) (               \
    __VEC_VALID_CHECK(this),          \
    __MARK_AS_NON_CONST(this),        \
    ((this)->__data + (this)->__size) \
)                                     \

//TODO: need return const __data if 'this' is const
#define vec_data(this) ((this)->__data)

#define vec_front(this) (                       \
    __VEC_VALID_CHECK(this),                    \
    __ASSERT(!vec_empty(this), "out of range"), \
    *vec_cbegin(this)                           \
)

#define vec_back(this) (                        \
    __VEC_VALID_CHECK(this),                    \
    __ASSERT(!vec_empty(this), "out of range"), \
    *(vec_cend(this) - 1)                       \
)                                               \

#define vec_shrink_to_fit(this) __SCOPE(                                           \
    __VEC_VALID_CHECK(this);                                                       \
    __MARK_AS_NON_CONST(this);                                                     \
    if ((this)->__capacity != (this)->__size) {                                    \
        (this)->__capacity = (this)->__size;                                       \
        __IT_TYPE(this) __tmp_data = calloc((this)->__capacity, __VAL_SIZE(this)); \
        memcpy(__tmp_data, (this)->__data, __VAL_SIZE(this) * (this)->__capacity); \
        free((this)->__data);                                                      \
        (this)->__data = __tmp_data;                                               \
    }                                                                              \
)                                                                                  \

#define vec_clear(this) __SCOPE( \
    __VEC_VALID_CHECK(this);     \
    __MARK_AS_NON_CONST(this);   \
    __DESTRUCT_ALL(this);        \
    (this)->__size = 0;          \
)                                \

#define vec_emplace(this, position, value) __SCOPE( \
    __VEC_VALID_CHECK(this);                        \
    __IT_VALID_CHECK(this, position);               \
    __IT_NON_CONST_CHECK(position);                 \
    if ((this)->__elem_destructor != NULL)          \
        (this)->__elem_destructor(position);        \
    *(position) = value;                            \
)                                                   \

//TODO: overloading vec_insert(this, position, count, value)
//TODO: mb overloading vec_insert(this, first, last)
#define vec_insert(this, position, value) __SCOPE(                                      \
    __VEC_VALID_CHECK(this);                                                            \
    __IT_VALID_CHECK(this, position);                                                   \
    __IT_NON_CONST_CHECK(position);                                                     \
    __RAISE_SIZE(this);                                                                 \
    memmove((position) + 1, position, __VAL_SIZE(this) * (vec_end(this) - (position))); \
    if ((this)->__elem_destructor != NULL)                                              \
        (this)->__elem_destructor(position);                                            \
    *(position) = (value);                                                              \
)                                                                                       \

#define __ERASE_OVERLOADING(_1, _2, _3, NAME, ...) NAME
#define vec_erase(...) __ERASE_OVERLOADING(__VA_ARGS__, __ERASE_RANGE, __ERASE_POS)(__VA_ARGS__)

#define __ERASE_POS(this, position) __SCOPE(                                                  \
    __VEC_VALID_CHECK(this);                                                                  \
    __IT_VALID_CHECK(this, position);                                                         \
    __IT_NON_CONST_CHECK(position);                                                           \
    if ((this)->__elem_destructor != NULL)                                                    \
        (this)->__elem_destructor(position);                                                  \
    memmove(position, (position) + 1, __VAL_SIZE(this) * ((vec_end(this) - (position)) - 1)); \
    --((this)->__size);                                                                       \
)                                                                                             \

#define __ERASE_RANGE(this, first, last) __SCOPE(                                  \
    __VEC_VALID_CHECK(this);                                                       \
    __IT_VALID_CHECK(this, first);                                                 \
    __IT_VALID_CHECK(this, last);                                                  \
    __IT_NON_CONST_CHECK(first);                                                   \
    __IT_NON_CONST_CHECK(last);                                                    \
    __ASSERT((last) >= (first), "'last' must be the final iterator");              \
    if ((this)->__elem_destructor != NULL) {                                       \
        for (__IT_TYPE(this) __it = (first); __it <= (last); ++__it)               \
            (this)->__elem_destructor(__it);                                       \
    }                                                                              \
    memmove(first, (last) + 1, __VAL_SIZE(this) * ((vec_end(this) - (last)) - 1)); \
    (this)->__size -= ((last) - (first)) + 1;                                      \
)                                                                                  \

//TODO: vec_resize(vec_ptr, count)
//TODO: overloading vec_resize(vec_ptr, count, value)

#define vec_pop_back(this) __SCOPE(                       \
    __VEC_VALID_CHECK(this);                              \
    __MARK_AS_NON_CONST(this);                            \
    if (!vec_empty(this)) {                               \
        if ((this)->__elem_destructor != NULL)            \
            (this)->__elem_destructor(vec_end(this) - 1); \
        --((this)->__size);                               \
    }                                                     \
)                                                         \

#define vec_swap(this, target) __SCOPE(               \
    __VEC_VALID_CHECK(this);                          \
    __VEC_VALID_CHECK(target);                        \
    __MARK_AS_NON_CONST(this);                        \
    __MARK_AS_NON_CONST(target);                      \
    __COMPATIBLE_CHECK(this, target);                 \
    __SWAP((this)->__size, (target)->__size);         \
    __SWAP((this)->__capacity, (target)->__capacity); \
    __SWAP((this)->__data, (target)->__data);         \
)                                                     \

#define vec_move(this, target) __SCOPE(        \
    __VEC_VALID_CHECK(this);                   \
    __VEC_VALID_CHECK(target);                 \
    __MARK_AS_NON_CONST(target);               \
    __COMPATIBLE_CHECK(this, target);          \
    free((target)->__data);                    \
    (target)->__data = (this)->__data;         \
    (target)->__size = (this)->__size;         \
    (target)->__capacity = (this)->__capacity; \
    (this) = NULL;                             \
)                                              \

#define vec_copy(this, target) __SCOPE(                                            \
    __VEC_VALID_CHECK(this);                                                       \
    __VEC_VALID_CHECK(target);                                                     \
    __MARK_AS_NON_CONST(target);                                                   \
    __COMPATIBLE_CHECK(this, target);                                              \
    if ((target)->__capacity < (this)->__size) {                                   \
        (target)->__capacity = (this)->__size;                                     \
        __REALLOC_TO_CAP(target);                                                  \
    }                                                                              \
    (target)->__size = (this)->__size;                                             \
    memcpy((target)->__data, (this)->__data, __VAL_SIZE(this) * (target)->__size); \
)                                                                                  \

#define vec_set_elem_destructor(this, destructor_ptr) __SCOPE(                                \
    __VEC_VALID_CHECK(this);                                                                  \
    __MARK_AS_NON_CONST(this);                                                                \
    __ASSERT((destructor_ptr) != NULL, "void (*destructor)(T* elem) expected, but got null"); \
    (this)->__elem_destructor = (destructor_ptr);                                             \
)                                                                                             \

#define vec_remove_elem_destructor(this) __SCOPE( \
    __VEC_VALID_CHECK(this);                      \
    __MARK_AS_NON_CONST(this);                    \
    (this)->__elem_destructor = NULL;             \
)                                                 \


vec_template_impl(int);
vec_template_impl(short);
vec_template_impl(long);
vec_template_impl(float);
vec_template_impl(size_t);
vec_template_impl(double);
vec_template_impl(char);
vec_template_impl(bool);