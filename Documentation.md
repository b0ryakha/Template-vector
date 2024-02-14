### :exclamation: Disclaimer:
  - In no case do not use default operators, all assign and copy are carried out only through special "functions".
  - Almost all "functions" are actually MACRO (inline).
  - Not all errors appear at compile-time, many errors will be handling only at run-time.
  - If necessary, the default constructor & destructor are called, if there are any, otherwise the garbage will be stored in memory.
  - By default lib has vector implementations for default types, to disable add #define NO_DEFAULT_IMPL before including the lib.
  - By default, the lib uses the DEPRECIATION_COEF value of 2, you can change it before including lib.

---

<details><summary>:house: Member functions</summary>

  - [constructor()](#constructor)
  - [copy_constructor()](#copy_constructor)
  - [destructor()](#destructor)
  - [assign()](#assign)
  - [moving()](#moving)
  </details>

<details><summary>:wrench: Element access</summary>

  - [at()](#at)
  - [front()](#front)
  - [back()](#back)
  - [data()](#data)
  </details>

<details><summary>:hammer: Iterators</summary>

  - [begin()](#begin)
  - [cbegin()](#cbegin)
  - [end()](#end)
  - [cend()](#cend)
  </details>

<details><summary>:floppy_disk: Capacity</summary>

  - [empty()](#empty)
  - [size()](#size)
  - [capacity()](#capacity)
  - [reserve()](#reserve)
  - [shrink_to_fit()](#shrink_to_fit)
  </details>

<details><summary>:pencil2: Modifiers</summary>

  - [clear()](#clear)
  - [insert()](#insert)
  - [emplace()](#emplace)
  - [erase()](#erase)
  - [push_back()](#push_back)
  - [emplace_back()](#emplace_back)
  - [pop_back()](#pop_back)
  - [resize()](#resize)
  - [swap()](#swap)
  </details>

<details><summary>:cyclone: Non-member functions</summary>

  - [vector()](#vector)
  - [vec_it()](#vec_it)
  - [vec_const_it()](#vec_const_it)
  - [vec_set_def_constructor()](#vec_set_def_constructor)
  - [vec_remove_elem_constructor()](#vec_remove_elem_constructor)
  - [vec_set_destructor()](#vec_set_destructor)
  - [vec_remove_elem_destructor()](#vec_remove_elem_destructor)
  </details>

---

#### <a name="constructor"></a> ```constructor```
```lua
//global area:
typedef type T;
vec_template_impl(T);
//

vec_create(T, value: T...): vector(T)
```
| Name  | Type           | Description      |
| :---: | :---:          | :---:            |
|       | ```template``` | Stored data type |
| value | ```T```        | Stored value(s)  |

Returns a pointer, which is a vector, by allocating memory in the heap.

#### <a name="copy_constructor"></a> ```copy_constructor```
```lua
vec_create_from(target: vector(T)): vector(T)
```
| Name   | Type            | Description         |
| :---:  | :---:           | :---:               |
| target | ```vector(T)``` | Vector to be copied |

Returns a copy of the vector, by allocating memory in the heap.

#### <a name="destructor"></a> ```destructor```
```lua
vec_remove(vec_ptr: vector(T))
```
| Name    | Type            | Description |
| :---:   | :---:           | :---:       |
| vec_ptr | ```vector(T)``` | Vector      |

Frees up memory, ignored if 'vec_ptr' is equal NULL.

#### <a name="assign"></a> ```assign```
```lua
vec_copy(this: vector(T), target: vector(T))
```
| Name   | Type            | Description             |
| :---:  | :---:           | :---:                   |
| this   | ```vector(T)``` | The object being copied |
| target | ```vector(T)``` | Target object           |

Copies all elements from 'this' to 'target'.

#### <a name="moving"></a> ```moving```
```lua
vec_move(this: vector(T), target: vector(T))
```
| Name   | Type            | Description |
| :---:  | :---:           | :---:       |
| this   | ```vector(T)``` | From        |
| target | ```vector(T)``` | To          |

Moves all elements from 'this' to the 'target' using pointer substitution, 'this' changes to NULL.


#### <a name="at"></a> ```at```
```lua
vec_at(this: vector(T), index: size_t): T&
vec_at(this: const vector(T), index: size_t): const T&
```
| Name  | Type            | Description              |
| :---: | :---:           | :---:                    |
| this  | ```vector(T)``` | Target                   |
| index | ```size_t```    | The index of the element |

Returns element under the specified index.

#### <a name="front"></a> ```front```
```lua
vec_front(this: vector(T)): T&
vec_front(this: const vector(T)): const T&
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns first element.

#### <a name="back"></a> ```back```
```lua
vec_back(this: vector(T)): T&
vec_back(this: const vector(T)): const T&
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns last element.

#### <a name="data"></a> ```data```
```lua
vec_data(this: vector(T)): T*
vec_data(this: const vector(T)): const T*
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns 'data' of vector.


#### <a name="begin"></a> ```begin```
```lua
vec_begin(this: vector(T)): T*
vec_begin(this: const vector(T)): const T*
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns iterator, pointing to the beginning of the vector.

#### <a name="cbegin"></a> ```cbegin```
```lua
vec_cbegin(this: vector(T)): const T*
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns const-iterator, pointing to the beginning of the vector.

#### <a name="end"></a> ```end```
```lua
vec_end(this: vector(T)): T*
vec_end(this: const vector(T)): const T*
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns iterator, pointing to the ending of the vector.

#### <a name="cend"></a> ```cend```
```lua
vec_cend(this: vector(T)): const T*
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns const-iterator, pointing to the ending of the vector.


#### <a name="empty"></a> ```empty```
```lua
vec_empty(this: vector(T)): bool
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns true if 'size' of vector is 0, else false.

#### <a name="size"></a> ```size```
```lua
vec_size(this: vector(T)): size_t
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns 'size' of vector.

#### <a name="capacity"></a> ```capacity```
```lua
vec_capacity(this: vector(T)): size_t
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Returns the number of elements that the container has currently allocated space for.

#### <a name="reserve"></a> ```reserve```
```lua
vec_reserve(this: vector(T), new_cap: size_t)
```
| Name    | Type            | Description  |
| :---:   | :---:           | :---:        |
| this    | ```vector(T)``` | Target       |
| new_cap | ```size_t```    | New capacity |

Increase the 'capacity' of the vector to 'new_cap', does not change its 'size'.

#### <a name="shrink_to_fit"></a> ```shrink_to_fit```
```lua
vec_shrink_to_fit(this: vector(T))
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Requests the removal of unused 'capacity'.


#### <a name="clear"></a> ```clear```
```lua
vec_clear(this: vector(T))
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

Erases all elements from the container, leaves the 'capacity' of the vector unchanged.

#### <a name="insert"></a> ```insert```
```lua
vec_insert(this: vector(T), pos: vec_it(this), value: T): vec_it(this)
vec_insert(this: vector(T), pos: vec_it(this), count: size_t, value: T): vec_it(this)
```
| Name  | Type               | Description                                     |
| :---: | :---:              | :---:                                           |
| this  | ```vector(T)```    | Target                                          |
| pos   | ```vec_it(this)``` | The place where the insertion will be performed |
| count | ```size_t```       | The number of objects to insert                 |
| value | ```T```            | Inserted value                                  |

(1) Inserts 'value' before 'pos', returns iterator pointing to the inserted element.
(2) Inserts 'count' copies of the 'value' before 'pos', returns iterator pointing to the first element inserted, or pos if 'count' == 0.

#### <a name="emplace"></a> ```emplace```
```lua
vec_emplace(this: vector(T), pos: vec_it(this), value: T): vec_it(this)
```
| Name  | Type               | Description                                   |
| :---: | :---:              | :---:                                         |
| this  | ```vector(T)```    | Target                                        |
| pos   | ```vec_it(this)``` | The place where the emplace will be performed |
| value | ```T```            | Emplaced value                                |

Warning: there is a discrepancy with the STL.
Emplace 'value' before 'pos', returns iterator pointing to the emplaced element.

#### <a name="erase"></a> ```erase```
```lua
vec_erase(this: vector(T), pos: vec_it(this)): vec_it(this)
vec_erase(this: vector(T), first: vec_it(this), last: vec_it(this)): vec_it(this)
```
| Name  | Type               | Description                                     |
| :---: | :---:              | :---:                                           |
| this  | ```vector(T)```    | Target                                          |
| pos   | ```vec_it(this)``` | The place to be removed                         |
| first | ```vec_it(this)``` | The starting point of the range, inclusive      |
| last  | ```vec_it(this)``` | The finishing point of the range, not inclusive |

(1) Removes the element at 'pos', returns iterator following the last removed element.
(2) Removes the elements in the range ['first', 'last'), returns iterator following the last removed element.

#### <a name="push_back"></a> ```push_back```
```lua
vec_push_back(this: vector(T), value: T)
```
| Name  | Type            | Description  |
| :---: | :---:           | :---:        |
| this  | ```vector(T)``` | Target       |
| value | ```T```         | Pushed value |

Appends the given element 'value' to the end of the container.

#### <a name="emplace_back"></a> ```emplace_back```
```lua
vec_emplace_back(this: vector(T), value: T)
```
| Name  | Type            | Description    |
| :---: | :---:           | :---:          |
| this  | ```vector(T)``` | Target         |
| value | ```T```         | Emplaced value |

Warning: there is a discrepancy with the STL.
Replaces the last value to new 'value'.

#### <a name="pop_back"></a> ```pop_back```
```lua
vec_pop_back(this: vector(T))
```
| Name  | Type            | Description    |
| :---: | :---:           | :---:          |
| this  | ```vector(T)``` | Target         |

Removes the last element of the container.

#### <a name="resize"></a> ```resize```
```lua
vec_resize(this: vector(T), count: size_t)
vec_resize(this: vector(T), count: size_t, value: T)
```
| Name  | Type            | Description                                               |
| :---: | :---:           | :---:                                                     |
| this  | ```vector(T)``` | Target                                                    |
| count | ```size_t```    | New size                                                  |
| value | ```T```         | The value that will be emplaced when the size is expanded |

Resizes the container to contain 'count' elements.

#### <a name="swap"></a> ```swap```
```lua
vec_swap(this: vector(T), target: vector(T))
```
| Name   | Type            | Description   |
| :---:  | :---:           | :---:         |
| this   | ```vector(T)``` | First vector  |
| target | ```vector(T)``` | Second vector |

Exchanges the contents and capacity of the container with those of other, does not deep copy.


#### <a name="vector"></a> ```vector```
```lua
vector(T)
```
| Name  | Type           | Description      |
| :---: | :---:          | :---:            |
|       | ```template``` | Stored data type |

The data type with the "template" parameter, is similar to std::vector<T>.

#### <a name="vec_it"></a> ```vec_it```
```lua
vec_it(this: vector(T))
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

The data type with the "template" parameter, is similar to std::vector<T>::iterator.

#### <a name="vec_const_it"></a> ```vec_const_it```
```lua
vec_const_it(this: vector(T))
```
| Name  | Type            | Description |
| :---: | :---:           | :---:       |
| this  | ```vector(T)``` | Target      |

The data type with the "template" parameter, is similar to std::vector<T>::const_iterator.

#### <a name="vec_set_def_constructor"></a> ```vec_set_def_constructor```
```lua
vec_set_def_constructor(this: vector(T), constructor: T (*)())
```
| Name        | Type            | Description                                                             |
| :---:       | :---:           | :---:                                                                   |
| this        | ```vector(T)``` | Target                                                                  |
| constructor | ```T (*)()```   | Pointer to the function that will be called during default construction |

Sets the default constructor for container elements, otherwise the default constructor will be called for default types.

#### <a name="vec_remove_elem_constructor"></a> ```vec_remove_elem_constructor```
```lua
vec_remove_elem_constructor(this: vector(T))
```
| Name        | Type            | Description |
| :---:       | :---:           | :---:       |
| this        | ```vector(T)``` | Target      |

Unsets the constructor for container elements.

#### <a name="vec_set_destructor"></a> ```vec_set_destructor```
```lua
vec_set_destructor(this: vector(T), destructor: void (*)(T*))
```
| Name       | Type               | Description                                                    |
| :---:      | :---:              | :---:                                                          |
| this       | ```vector(T)```    | Target                                                         |
| destructor | ```void (*)(T*)``` | Pointer to the function that will be called during destruction |

Sets the destructor for container elements.

#### <a name="vec_remove_elem_destructor"></a> ```vec_remove_elem_destructor```
```lua
vec_remove_elem_destructor(this: vector(T))
```
| Name        | Type            | Description |
| :---:       | :---:           | :---:       |
| this        | ```vector(T)``` | Target      |

Unsets the destructor for container elements.