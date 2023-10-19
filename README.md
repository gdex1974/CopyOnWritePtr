# cow_ptr
cow_ptr is a C++ smart pointer class that provides copy-on-write semantics. It allows multiple references to the same object, but only creates a copy of the object when it's accessed by non-const method and there are other instances refer to the same object.
Traditionally such class is implemented by using an underlying shared_ptr which have more overhead than really necessary. This one is implemented using just an atomic reference count, so it works a bit faster and consume a bit less memory. :)
## Usage
To use cow_ptr, include the header file and create an instance of the class with the appropriate type:
```c++
#include "cow_ptr.h"

// Create a cow_ptr with an initial value
auto ptr = nonstd::make_cow<int>(42);
const auto& constPtr = ptr;
assert(*constPtr == 42); // access by const method
assert(constPtr.get() == ptr.get()); // no copy is created
// Modify the value (a copy is created)
*ptr = 100; // access by non-const method

assert(*constPtr == 42);
assert(*ptr.get() == 100);
```


The cow_ptr class provides the following features:

- Construction: You can create a cow_ptr object by passing a raw pointer to the constructor.
- Copy Semantics: The class supports copy construction and copy assignment, creating a new reference to the same object.
- Move Semantics: The class supports move construction and move assignment without changing the reference count.
- Dereference Operator: You can use the dereference operator (*) to access the value pointed to by the cow_ptr. Non-const access to the object will create a copy of the object if there are other instances refer to the same object.
- Arrow Operator: You can use the arrow operator (->) to access members of the object pointed to by the cow_ptr. Non-const access to the object will create a copy of the object if there are other instances refer to the same object.
- Equality Comparison: The class supports equality (==) and inequality (!=) comparisons between cow_ptr objects. The comparison is made between the stored pointers.
- reset(): The reset function allows you to reset the cow_ptr to a new value or nullptr. It'll start a new reference count for the new object.
- get(): The get function returns the raw pointer to the const object pointed to by the cow_ptr. It doesn't affect the reference count.
- getMutable(): The getMutable function returns the raw pointer to the object pointed to by the cow_ptr. If there are other instances refer to the same object, a copy of the object will be created.

The non-member helper functions:
- make_cow(): The make_cow function creates a new cow_ptr object passing its arguments to constructor. It works like std::make_shared.

### Circular References
cow_ptr doesn't handle circular references. If you need to handle circular references, you should use shared_ptr instead.

## Thread Safety

cow_ptr uses an atomic reference count to track the number of references to the object. The reference count is assigned to 1 for a new cow_ptr object and is incrementing if existing cow_ptr object is copied. The reference count is decremented when a cow_ptr object is destroyed. When the reference count reaches zero, the destructor of last instance delete the stored object.

## License

This library is released under the BSD 3-Clause License. See the LICENSE file for more information.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request on the GitHub repository.