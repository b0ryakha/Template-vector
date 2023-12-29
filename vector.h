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

#define __MAX(x, y) \
    ((x) > (y) ? (x) : (y))

#define __SWAP(x, y) do { \
    __ASSERT(sizeof(x) == sizeof(y), "to swap, the sizes 'x' and 'y' must be equal"); \
    unsigned char buffer[sizeof(x)];                                                  \
    memcpy(buffer, &(y), sizeof(x));                                                  \
    memcpy(&(y), &(x), sizeof(x));                                                    \
    memcpy(&(x), buffer, sizeof(x));                                                  \
} while(0)

#define __ASSERT(assertion, message) \
    __print_error(__FILE__, __LINE__, assertion, message)

#define __RAISE_CAPACITY(capacity) \
    ((capacity) * 2)

#define __VAL_TYPE(vec_ptr) \
    typeof((vec_ptr)->__data[0])

#define __VAL_SIZE(vec_ptr) \
    sizeof((vec_ptr)->__data[0])

#define __IT_TYPE(vec_ptr) \
    typeof((vec_ptr)->__data)


#define vec_template_impl(T) \
    typedef struct {                                                \
        size_t __size;                                              \
        size_t __capacity;                                          \
        T* __data;                                                  \
    } vec__##T;                                                     \
    \
    vec__##T* __VECTOR_ALLOC_##T(const T* init_list, size_t size) { \
        vec__##T* vec = malloc(sizeof(vec__##T));                   \
        vec->__size = size;                                         \
        vec->__capacity = __RAISE_CAPACITY(size);                   \
        vec->__data = calloc(vec->__capacity, sizeof(T));           \
        memcpy(vec->__data, init_list, sizeof(T) * size);           \
        return vec;                                                 \
    }                                                               \

#define vector(T) vec__##T*

#define vec_create(T, ...) \
    __VECTOR_ALLOC_##T((T[]){ __VA_ARGS__ }, sizeof((T[]){ __VA_ARGS__ }) / sizeof(T))

#define vec_remove(vec_ptr) if (vec_ptr) { \
    free((vec_ptr)->__data);                     \
    (vec_ptr)->__data = NULL;                    \
    free(vec_ptr);                               \
    (vec_ptr) = NULL;                            \
}

#define vec_at(vec_ptr, index) \
    (__ASSERT(((index) >= 0 && (index) < (vec_ptr)->__size), "out of range"), (vec_ptr)->__data[index])

#define vec_size(vec_ptr) \
    ((vec_ptr)->__size)

#define vec_capacity(vec_ptr) \
    ((vec_ptr)->__capacity)

#define vec_reserve(vec_ptr, capacity) \
    ((vec_ptr)->__capacity = __MAX(0, capacity))

#define vec_push_back(vec_ptr, value) if (vec_ptr) { \
    ++((vec_ptr)->__size);                                                                           \
    if ((vec_ptr)->__size > (vec_ptr)->__capacity) {                                                 \
        (vec_ptr)->__capacity = __RAISE_CAPACITY((vec_ptr)->__size);                                 \
        (vec_ptr)->__data = realloc((vec_ptr)->__data, __VAL_SIZE(vec_ptr) * (vec_ptr)->__capacity); \
    }                                                                                                \
    if ((vec_ptr)->__data == NULL)                                                                   \
        (vec_ptr)->__data = calloc((vec_ptr)->__capacity, __VAL_SIZE(vec_ptr));                      \
    (vec_ptr)->__data[(vec_ptr)->__size - 1] = (value);                                              \
}                                                                                                    \

#define vec_empty(vec_ptr) \
    ((vec_ptr)->__size == 0)

#define vec_front(vec_ptr) \
    (__ASSERT(!vec_empty(vec_ptr), "out of range"), (vec_ptr)->__data[0])

#define vec_back(vec_ptr) \
    (__ASSERT(!vec_empty(vec_ptr), "out of range"), (vec_ptr)->__data[(vec_ptr)->__size - 1])

#define vec_data(vec_ptr) \
    ((vec_ptr)->__data)

#define vec_begin(vec_ptr) \
    ((vec_ptr)->__data)

#define vec_end(vec_ptr) \
    ((vec_ptr)->__data + (vec_ptr)->__size)

#define vec_cbegin(vec_ptr) \
    ((const __VAL_TYPE(vec_ptr)*)vec_begin(vec_ptr))

#define vec_cend(vec_ptr) \
    ((const __VAL_TYPE(vec_ptr)*)vec_end(vec_ptr))

#define vec_shrink_to_fit(vec_ptr) if (vec_ptr) { \
    (vec_ptr)->__capacity = (vec_ptr)->__size;                                                   \
    if (!vec_empty(vec_ptr))                                                                     \
        (vec_ptr)->__data = realloc((vec_ptr)->__data, __VAL_SIZE(vec_ptr) * (vec_ptr)->__size); \
}

#define vec_clear(vec_ptr) if (vec_ptr) { \
    free((vec_ptr)->__data);  \
    (vec_ptr)->__data = NULL; \
    (vec_ptr)->__size = 0;    \
}

#define vec_emplace(vec_ptr, it_pos, value) if ((vec_ptr) && (it_pos)) { \
    __ASSERT((it_pos) >= vec_begin(vec_ptr) && (it_pos) < vec_end(vec_ptr), "out of range"); \
    *(it_pos) = value;                                                                       \
}

#define vec_insert(vec_ptr, it_pos, value) if ((vec_ptr) && (it_pos)) { \
    __ASSERT((it_pos) >= vec_begin(vec_ptr) && (it_pos) < vec_end(vec_ptr), "out of range"); \
    __IT_TYPE(vec_ptr) new_data = calloc(++((vec_ptr)->__size), __VAL_SIZE(vec_ptr));        \
    __IT_TYPE(vec_ptr) it = new_data;                                                        \
    __IT_TYPE(vec_ptr) begin = vec_begin(vec_ptr);                                           \
    __IT_TYPE(vec_ptr) end = vec_end(vec_ptr);                                               \
    for (; begin != (it_pos); ++begin, ++it)                                                 \
        *it = *begin;                                                                        \
    *(it++) = value;                                                                         \
    for (; begin != end; ++begin, ++it)                                                      \
        *it = *begin;                                                                        \
    free((vec_ptr)->__data);                                                                 \
    (vec_ptr)->__data = new_data;                                                            \
}

#define vec_erase(vec_ptr, ...) if ((vec_ptr) && !vec_empty(vec_ptr)) { \
    __IT_TYPE(vec_ptr) its[] = { __VA_ARGS__ };                                                  \
    const size_t its_count = sizeof(its) / sizeof(its[0]);                                       \
    __ASSERT(its_count > 0, "too few arguments to 'vec_erase(vec_ptr, first, last = first)'");   \
    __ASSERT(its_count <= 2, "too many arguments to 'vec_erase(vec_ptr, first, last = first)'"); \
    __IT_TYPE(vec_ptr) first = its[0];                                                           \
    __IT_TYPE(vec_ptr) last = (first);                                                           \
    if (its_count == 2) (last) = its[1];                                                         \
    __ASSERT(last >= first, "'last' must be the final iterator");                                \
    __ASSERT((first) >= vec_begin(vec_ptr) && (first) < vec_end(vec_ptr), "out of range");       \
    __ASSERT((last) >= vec_begin(vec_ptr) && (last) < vec_end(vec_ptr), "out of range");         \
    __IT_TYPE(vec_ptr) begin = vec_begin(vec_ptr);                                               \
    __IT_TYPE(vec_ptr) end = vec_end(vec_ptr);                                                   \
    (vec_ptr)->__size -= (last - first) + 1;                                                     \
    __IT_TYPE(vec_ptr) new_data = calloc((vec_ptr)->__size, __VAL_SIZE(vec_ptr));                \
    __IT_TYPE(vec_ptr) it = new_data;                                                            \
    for (; begin != end; ++begin) {                                                              \
        if (begin < first || begin > last)                                                       \
            *(it++) = *begin;                                                                    \
    }                                                                                            \
    free((vec_ptr)->__data);                                                                     \
    (vec_ptr)->__data = new_data;                                                                \
}

#define pop_back(vec_ptr) if (!vec_empty(vec_ptr)) { \
    __IT_TYPE(vec_ptr) new_data = calloc(--((vec_ptr)->__size), __VAL_SIZE(vec_ptr)); \
    memcpy(new_data, (vec_ptr)->__data, __VAL_SIZE(vec_ptr) * (vec_ptr)->__size);     \
    free((vec_ptr)->__data);                                                          \
    (vec_ptr)->__data = new_data;                                                     \
}

#define vec_resize(vec_ptr, size) if (vec_ptr) { \
    \
}

#define vec_swap(vec_ptr1, vec_ptr2) do { \
    __ASSERT((vec_ptr1) && (vec_ptr2), "one of the passed 'vector_ptrs' is NULL"); \
    __SWAP((vec_ptr1)->__size, (vec_ptr2)->__size);                                \
    __SWAP((vec_ptr1)->__capacity, (vec_ptr2)->__capacity);                        \
    __SWAP((vec_ptr1)->__data, (vec_ptr2)->__data);                                \
}                                                                                  \
while(0);


vec_template_impl(int);
vec_template_impl(short);
vec_template_impl(long);
vec_template_impl(float);
vec_template_impl(size_t);
vec_template_impl(double);
vec_template_impl(char);
vec_template_impl(bool);