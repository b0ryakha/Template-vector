## Template vector
> A template vector in the C programming language using a preprocessor.

---

## API
#### constructor:
```c++
// global area ===============
typedef T T_typedef;
vec_template_impl(T_typedef);
// ===========================

vector(T_typedef) vec_name = vec_create(T_typedef, value...);
```
#### destructor:
```c++
vec_remove(vec_name);
```
#### element by index:
```c++
vec_at(vec_name, index) = value;
T value = vec_at(vec_name, index);
```
#### get size:
```c++
size_t size = vec_size(vec_name);
```
#### get capacity:
```c++
size_t capacity = vec_capacity(vec_name);
```
#### reserve:
```c++
vec_reserve(vec_name, capacity);
```
#### push back:
```c++
vec_push_back(vec_name, value);
```
#### is empty:
```c++
bool is_empty = vec_empty(vec_name);
```
#### get first element:
```c++
T value = vec_front(vec_name);
```
#### get last element:
```c++
T value = vec_back(vec_name);
```
#### get data pointer:
```c++
T* data = vec_data(vec_name);
```
#### get begin iterator:
```c++
T* begin = vec_begin(vec_name);
```
#### get end iterator:
```c++
T* end = vec_end(vec_name);
```
#### get const begin iterator:
```c++
const T* cbegin = vec_cbegin(vec_name);
```
#### get const end iterator:
```c++
const T* cend = vec_cend(vec_name);
```
#### clear:
```c++
vec_clear(vec_name);
```
#### shrink to fit:
```c++
vec_shrink_to_fit(vec_name);
```
#### emplace element:
```c++
vec_emplace(pos_it, value);
```
#### insert element:
```c++
vec_insert(pos_it, value);
```
#### erase:
```c++
vec_erase(vec_name, first_it[, last_it]);
```
#### pop back:
```c++
vec_pop_back(vec_name);
```
#### swap:
```c++
vec_swap(vec1_name, vec2_name);
```
#### resize:
```c++

```

---

## License
This project is licensed under the GNU GPLv3 License - see the [LICENSE.md](LICENSE.md) file for details
