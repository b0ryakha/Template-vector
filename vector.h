#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//TODO: all input iterators to const iterators (emplace, insert, erase)

#ifndef _ASSERT
#define _ASSERT(assertion, message) ({                                                       \
    if (!(assertion)) {                                                                      \
        char buffer[256];                                                                    \
        snprintf(buffer, sizeof(buffer), "%s:%d: error: %s\n", __FILE__, __LINE__, message); \
        fprintf(stderr, "%s", buffer);                                                       \
        exit(EXIT_FAILURE);                                                                  \
    }                                                                                        \
})
#endif

#ifndef _OVERLOADING_3
#define _OVERLOADING_3(_1, _2, _3, NAME, ...) NAME
#endif

#ifndef _OVERLOADING_4
#define _OVERLOADING_4(_1, _2, _3, _4, NAME, ...) NAME
#endif

#ifndef _IS_COMPATIBLE
#define _IS_COMPATIBLE(T, U) \
    _Generic(*((T*)NULL), U: true, default: false)
#endif

#ifndef _IS_CONST
#define _IS_CONST(L_value) \
    _Generic(&(L_value), __typeof__(L_value) const*: true, default: false)
#endif

#ifndef _MEMORY_VALID_CHECK
#define _MEMORY_VALID_CHECK(ptr) \
    _ASSERT((ptr) != NULL, "memory exhaustion")
#endif

#ifndef _SWAP
#define _SWAP(x, y) ({                                                                 \
    _ASSERT(sizeof(x) == sizeof(y), "to swap, the size of 'x' and 'y' must be equal"); \
    unsigned char _buffer[sizeof(x)];                                                  \
    memcpy(_buffer, &(y), sizeof(x));                                                  \
    memcpy(&(y), &(x), sizeof(x));                                                     \
    memcpy(&(x), _buffer, sizeof(x));                                                  \
})
#endif


#define _VEC_V_TYPE(vec_ptr) __typeof__(*((vec_ptr)->_data))
#define _VEC_V_SIZE(vec_ptr) sizeof(*((vec_ptr)->_data))

#define _VEC_RAISE_CAP(vec_ptr) \
    (vec_ptr)->_capacity = ((vec_ptr)->_capacity == 0) ? 1 : (vec_ptr)->_capacity * 2;

#define _VEC_REALLOC_TO_CAP(vec_ptr) ({                                                        \
    (vec_ptr)->_data = realloc((vec_ptr)->_data, _VEC_V_SIZE(vec_ptr) * (vec_ptr)->_capacity); \
    _MEMORY_VALID_CHECK((vec_ptr)->_data);                                                     \
    (vec_ptr)->_cdata = (vec_ptr)->_data;                                                      \
})

#define _VEC_RAISE_SIZE(vec_ptr, count) ({         \
    (vec_ptr)->_size += (count);                   \
    if ((vec_ptr)->_size > (vec_ptr)->_capacity) { \
        (vec_ptr)->_capacity = (vec_ptr)->_size;   \
        _VEC_RAISE_CAP(vec_ptr);                   \
        _VEC_REALLOC_TO_CAP(vec_ptr);              \
    }                                              \
})

#define _VEC_VALID_CHECK(vec_ptr) \
    _ASSERT((vec_ptr) != NULL, "vector<T> expected, but got null")

#define _VEC_IT_VALID_CHECK(vec_ptr, it) ({                                                                                         \
    _ASSERT((it) != NULL, "vector<T>::iterator expected, but got null");                                                            \
    _ASSERT(_IS_COMPATIBLE(__typeof__(*(it)), _VEC_V_TYPE(vec_ptr)), "vector<T>::iterator expected, but got incompatible pointer"); \
    _ASSERT((it) >= vec_cbegin(vec_ptr) && (it) <= vec_cend(vec_ptr), "out of range");                                              \
})

#define _VEC_IT_NCONST_CHECK(it) \
    _ASSERT(!_IS_CONST(*(it)), "expected non-const iterator")

#define _VEC_COMP_CHECK(vec_ptr1, vec_ptr2) \
    _ASSERT(_IS_COMPATIBLE(_VEC_V_TYPE(vec_ptr1), _VEC_V_TYPE(vec_ptr2)), "vectors store incompatible types.")

#define _VEC_DESTRUCT_ALL(this) ({                        \
    if ((this)->_elem_destructor) {                       \
        for (size_t _i = 0; _i < (this)->_size; ++_i)     \
            (this)->_elem_destructor(&(this)->_data[_i]); \
    }                                                     \
})

typedef struct {
    size_t _size;
    size_t _capacity;
    void (*_elem_destructor)(void*);
    void* _data;
    const void* _cdata;
} __vec_unconst_hack;

#define vec_template_impl(T)                                    \
    typedef struct {                                            \
        size_t _size;                                           \
        size_t _capacity;                                       \
        void (*_elem_destructor)(T*);                           \
        T* _data;                                               \
        const T* _cdata;                                        \
    } _vec_##T;                                                 \
    \
    _vec_##T* _VEC_ALLOC_##T(const T* init_list, size_t size) { \
        _vec_##T* vec = malloc(sizeof(_vec_##T));               \
        _MEMORY_VALID_CHECK(vec);                               \
        vec->_size = size;                                      \
        vec->_capacity = size;                                  \
        vec->_elem_destructor = NULL;                           \
        if (size > 0) {                                         \
            vec->_data = calloc(vec->_capacity, sizeof(T));     \
            _MEMORY_VALID_CHECK(vec->_data);                    \
            vec->_cdata = vec->_data;                           \
            memcpy(vec->_data, init_list, sizeof(T) * size);    \
        }                                                       \
        else {                                                  \
            vec->_data = NULL;                                  \
            vec->_cdata = NULL;                                 \
        }                                                       \
        return vec;                                             \
    }

#define vector(T) _vec_##T*

#define vec_create(T, ...) \
    _VEC_ALLOC_##T((T[]){ __VA_ARGS__ }, sizeof((T[]){ __VA_ARGS__ }) / sizeof(T))

#define vec_create_from(target) ({                                                         \
    _VEC_VALID_CHECK(target);                                                              \
    __typeof__(target) _new_vec = malloc(sizeof(__typeof__(target)));                      \
    _MEMORY_VALID_CHECK(_new_vec);                                                         \
    _new_vec->_size = (target)->_size;                                                     \
    _new_vec->_capacity = (target)->_size;                                                 \
    _new_vec->_elem_destructor = (target)->_elem_destructor;                               \
    if (_new_vec->_size > 0) {                                                             \
        _new_vec->_data = calloc(_new_vec->_capacity, _VEC_V_SIZE(target));                \
        _MEMORY_VALID_CHECK(_new_vec->_data);                                              \
        _new_vec->_cdata = _new_vec->_data;                                                \
        memcpy(_new_vec->_data, (target)->_data, _VEC_V_SIZE(_new_vec) * _new_vec->_size); \
    }                                                                                      \
    else {                                                                                 \
        _new_vec->_data = NULL;                                                            \
        _new_vec->_cdata = NULL;                                                           \
    }                                                                                      \
    (_new_vec);                                                                            \
})

#define vec_remove(this) ({       \
    if ((this) != NULL) {         \
        _VEC_DESTRUCT_ALL(this);  \
        free((void*)this->_data); \
        free((void*)this);        \
        (this) = NULL;            \
    }                             \
})

#define vec_size(this) ({   \
    _VEC_VALID_CHECK(this); \
    (this)->_size;          \
})

#define vec_capacity(this) ({ \
    _VEC_VALID_CHECK(this);   \
    (this)->_capacity;        \
})

#define vec_reserve(this, capacity) ({    \
    _VEC_VALID_CHECK(this);               \
    if ((capacity) > (this)->_capacity) { \
        (this)->_capacity = (capacity);   \
        _VEC_REALLOC_TO_CAP(this);        \
    }                                     \
})

#define vec_push_back(this, value) ({           \
    _VEC_VALID_CHECK(this);                     \
    _VEC_RAISE_SIZE(this, 1);                   \
    (this)->_data[(this)->_size - 1] = (value); \
})

#define vec_empty(this) ({  \
    _VEC_VALID_CHECK(this); \
    ((this)->_size == 0);   \
})

#define vec_it(this) __typeof__(vec_begin(this))
#define vec_const_it(this) __typeof__((this)->_cdata)

#define vec_data(this) ({                                                             \
    _VEC_VALID_CHECK(this);                                                           \
    _Generic(this, __typeof__(*this) const*: (this)->_cdata, default: (this)->_data); \
})

#define vec_at(this, index) (*({                                        \
    _VEC_VALID_CHECK(this);                                             \
    _ASSERT(((index) >= 0 && (index) < (this)->_size), "out of range"); \
    (vec_data(this) + (index));                                         \
}))

#define vec_cbegin(this) ({ \
    _VEC_VALID_CHECK(this); \
    (this)->_cdata;         \
})

#define vec_cend(this) ({             \
    _VEC_VALID_CHECK(this);           \
    ((this)->_cdata + (this)->_size); \
})

#define vec_begin(this) ({  \
    _VEC_VALID_CHECK(this); \
    vec_data(this);         \
})

#define vec_end(this) ({              \
    _VEC_VALID_CHECK(this);           \
    (vec_data(this) + (this)->_size); \
})

#define vec_front(this) (*({                   \
    _VEC_VALID_CHECK(this);                    \
    _ASSERT(!vec_empty(this), "out of range"); \
    vec_data(this);                            \
}))

#define vec_back(this) (*({                    \
    _VEC_VALID_CHECK(this);                    \
    _ASSERT(!vec_empty(this), "out of range"); \
    (vec_data(this) + (this)->_size - 1);      \
}))

#define vec_shrink_to_fit(this) ({                                               \
    _VEC_VALID_CHECK(this);                                                      \
    if ((this)->_capacity != (this)->_size) {                                    \
        (this)->_capacity = (this)->_size;                                       \
        vec_it(this) _tmp_data = calloc((this)->_capacity, _VEC_V_SIZE(this));   \
        _MEMORY_VALID_CHECK(_tmp_data);                                          \
        memcpy(_tmp_data, (this)->_data, _VEC_V_SIZE(this) * (this)->_capacity); \
        free((this)->_data);                                                     \
        (this)->_data = _tmp_data;                                               \
    }                                                                            \
})

#define vec_clear(this) ({   \
    _VEC_VALID_CHECK(this);  \
    _VEC_DESTRUCT_ALL(this); \
    (this)->_size = 0;       \
})

#define vec_emplace(this, pos, value) ({              \
    _VEC_VALID_CHECK(this);                           \
    _VEC_IT_VALID_CHECK(this, pos);                   \
    if ((pos) < vec_cend(this)) {                     \
        if ((this)->_elem_destructor)                 \
            (this)->_elem_destructor(pos);            \
        *(pos) = (value);                             \
        (vec_data(this) + ((pos) - vec_begin(this))); \
    }                                                 \
})

#define vec_emplace_back(this, value) ({                             \
    _VEC_VALID_CHECK(this);                                          \
    if ((this)->_elem_destructor)                                    \
        (this)->_elem_destructor(&(this)->_data[(this)->_size - 1]); \
    (this)->_data[(this)->_size - 1] = (value);                      \
})

#define vec_insert(...) \
    _OVERLOADING_4(__VA_ARGS__, _VEC_INSERT_COUNT, _VEC_INSERT)(__VA_ARGS__)

#define _VEC_INSERT(this, pos, value) \
    _VEC_INSERT_COUNT(this, pos, 1, value)

#define _VEC_INSERT_COUNT(this, pos, count, value) ({                                                                 \
    _VEC_VALID_CHECK(this);                                                                                           \
    _VEC_IT_VALID_CHECK(this, pos);                                                                                   \
    size_t _offset = 0;                                                                                               \
    if ((count) > 0) {                                                                                                \
        if ((pos) >= vec_end(this))                                                                                   \
            _offset = (count);                                                                                        \
        _VEC_RAISE_SIZE(this, count);                                                                                 \
        memmove((pos) + (count) - _offset, (pos) - _offset, _VEC_V_SIZE(this) * (vec_end(this) - ((pos) - _offset))); \
        vec_it(this) _it = (pos) - _offset;                                                                           \
        vec_it(this) _end = _it + (count);                                                                            \
        for (; _it != _end; ++_it) *_it = (value);                                                                    \
    }                                                                                                                 \
    ((count > 0) ? (vec_begin(this) + ((pos) - vec_begin(this))) : pos);                                              \
})

#define vec_erase(...) \
    _OVERLOADING_3(__VA_ARGS__, _VEC_ERASE_RANGE, _VEC_ERASE_POS)(__VA_ARGS__)

#define _VEC_ERASE_POS(this, pos) ({                                                \
    _VEC_VALID_CHECK(this);                                                         \
    _VEC_IT_VALID_CHECK(this, pos);                                                 \
    _VEC_IT_NCONST_CHECK(pos);                                                      \
    if ((pos) < vec_cend(this)) {                                                   \
        if ((this)->_elem_destructor)                                               \
            (this)->_elem_destructor(pos);                                          \
        memmove(pos, (pos) + 1, _VEC_V_SIZE(this) * ((vec_end(this) - (pos)) - 1)); \
        --((this)->_size);                                                          \
    }                                                                               \
    (vec_begin(this) + ((pos) - vec_begin(this)));                                  \
})

#define _VEC_ERASE_RANGE(this, first, last) ({                              \
    _VEC_VALID_CHECK(this);                                                 \
    _VEC_IT_VALID_CHECK(this, first);                                       \
    _VEC_IT_VALID_CHECK(this, last);                                        \
    _VEC_IT_NCONST_CHECK(first);                                            \
    _VEC_IT_NCONST_CHECK(last);                                             \
    _ASSERT((last) >= (first), "'last' must be the final iterator");        \
    if ((this)->_elem_destructor) {                                         \
        for (vec_it(this) _it = (first); _it < (last); ++_it)               \
            (this)->_elem_destructor(_it);                                  \
    }                                                                       \
    memmove(first, (last), _VEC_V_SIZE(this) * ((vec_end(this) - (last)))); \
    const size_t _diff = (last) - (first);                                  \
    (this)->_size -= _diff;                                                 \
    (vec_begin(this) + (((last) - vec_begin(this)) - _diff));               \
})

#define vec_resize(...) \
    _OVERLOADING_3(__VA_ARGS__, _VEC_RESIZE_WITH_VALUE, _VEC_RESIZE)(__VA_ARGS__)

#define _VEC_RESIZE(this, count) \
    _VEC_RESIZE_WITH_VALUE(this, count, (_VEC_V_TYPE(this)){0});

#define _VEC_RESIZE_WITH_VALUE(this, count, value) ({           \
    _VEC_VALID_CHECK(this);                                     \
    if ((this)->_size != (count)) {                             \
        if ((count) > (this)->_size) {                          \
            while ((this)->_capacity < (count))                 \
                _VEC_RAISE_CAP(this);                           \
            _VEC_REALLOC_TO_CAP(this);                          \
            for (size_t _i = (this)->_size; _i < (count); ++_i) \
                (this)->_data[_i] = value;                      \
        }                                                       \
        else if ((this)->_elem_destructor) {                    \
            for (size_t _i = (count); _i < (this)->_size; ++_i) \
                (this)->_elem_destructor(&(this)->_data[_i]);   \
        }                                                       \
        (this)->_size = (count);                                \
    }                                                           \
})

#define vec_pop_back(this) ({                            \
    _VEC_VALID_CHECK(this);                              \
    if (!vec_empty(this)) {                              \
        if ((this)->_elem_destructor)                    \
            (this)->_elem_destructor(vec_end(this) - 1); \
        --((this)->_size);                               \
    }                                                    \
})

#define vec_swap(this, target) ({                                    \
    if ((this) != (target)) {                                        \
        _VEC_VALID_CHECK(this);                                      \
        _VEC_VALID_CHECK(target);                                    \
        _VEC_COMP_CHECK(this, target);                               \
        _SWAP((this)->_size, (target)->_size);                       \
        _SWAP((this)->_capacity, (target)->_capacity);               \
        _SWAP((this)->_data, (target)->_data);                       \
        _SWAP((this)->_elem_destructor, (target)->_elem_destructor); \
        (this)->_cdata = (this)->_data;                              \
    }                                                                \
})

#define vec_move(this, target) ({                              \
    if ((this) != (target)) {                                  \
        _VEC_VALID_CHECK(this);                                \
        _VEC_COMP_CHECK(this, target);                         \
        if ((target) == NULL)                                  \
            (target) = malloc(sizeof(__typeof__(target)));     \
        free((target)->_data);                                 \
        (target)->_data = (this)->_data;                       \
        (target)->_cdata = (target)->_data;                    \
        (target)->_size = (this)->_size;                       \
        (target)->_capacity = (this)->_capacity;               \
        (target)->_elem_destructor = (this)->_elem_destructor; \
        (this) = NULL;                                         \
    }                                                          \
})

#define vec_copy(this, target) ({                                                    \
    if ((this) != (target)) {                                                        \
        _VEC_VALID_CHECK(this);                                                      \
        _VEC_COMP_CHECK(this, target);                                               \
        if ((target) == NULL) {                                                      \
            (target) = malloc(sizeof(__typeof__(target)));                           \
            (target)->_capacity = (this)->_size;                                     \
            (target)->_data = malloc(_VEC_V_SIZE(target) * (target)->_capacity);     \
        }                                                                            \
        else if ((target)->_capacity < (this)->_size) {                              \
            (target)->_capacity = (this)->_size;                                     \
            _VEC_REALLOC_TO_CAP(target);                                             \
        }                                                                            \
        (target)->_size = (this)->_size;                                             \
        memcpy((target)->_data, (this)->_data, _VEC_V_SIZE(this) * (target)->_size); \
        (target)->_cdata = (target)->_data;                                          \
        (target)->_elem_destructor = (this)->_elem_destructor;                       \
    }                                                                                \
})

#define vec_set_destructor(this, destructor_ptr) ({                                          \
    _VEC_VALID_CHECK(this);                                                                  \
    _ASSERT((destructor_ptr) != NULL, "void (*destructor)(T* elem) expected, but got null"); \
    ((__vec_unconst_hack*)this)->_elem_destructor = (void (*)(void*))(destructor_ptr);       \
})

#define vec_remove_elem_destructor(this) ({ \
    _VEC_VALID_CHECK(this);                 \
    (this)->_elem_destructor = NULL;        \
})


#ifndef NO_DEFAULT_IMPL
vec_template_impl(int);
vec_template_impl(short);
vec_template_impl(long);
vec_template_impl(float);
vec_template_impl(size_t);
vec_template_impl(double);
vec_template_impl(char);
vec_template_impl(bool);
#endif