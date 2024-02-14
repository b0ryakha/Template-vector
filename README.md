## Template vector
> A header-only implementation of std::vector on C with GNU extensions.

A type-safe and easy-to-use dynamic array with an interface similar to the STL interface, as well as error handling

Any non-zero vector looks like this in memory:

                                      const
                                    / ‾ ‾ ‾ \   | user's const pointer & it
                                   v         \  v
              +------+----------+---------+ - - - ÷ - - - - - - - - - - -÷ - - - - - - - - ÷
              | size | capacity | data... | cdata : elem_def_constructor : elem_destructor :
              +------+----------+---------+ - - - ÷ - - - - - - - - - - -÷ - - - - - - - - ÷
                                  ^
                                  | user's pointer & it
The total overhead is `2 * sizeof(size_t) + sizeof(void (*)()) + sizeof(void (*)(void*)) + sizeof(void*)` per vector.

---

## Example
```cpp
vector(int) vec = vec_create(int, 1, 2, 3);

vec_insert(vec, vec_begin(vec) + 1, 10);

for (size_t i = 0; i < vec_size(vec); ++i)
    printf("%d ", vec_at(vec, i));

//output: 1, 10, 2, 3

vec_remove(vec);
```

---

## Documentation
For information about methods and functions, see [Documentation.md](Documentation.md) file.  

---

## License
This project is licensed under the GNU GPLv3 License - see the [LICENSE.md](LICENSE.md) file for details
