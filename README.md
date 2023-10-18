# cow_ptr

cow_ptr is a C++ smart pointer class that provides copy-on-write semantics. It allows multiple references to the same object, but only creates a copy of the object when it's accessed by non-const method and there are other instances refer to the same object.

## Usage
To use cow_ptr, include the header file and create an instance of the class with the appropriate type:
```c++
#include "cow_ptr.h"

// Create a cow_ptr with an initial value
auto ptr = nonstd::make_cow<int>(42);

// Access the value using the dereference operator
int value = *ptr;

const auto& constPtr = ptr;
// Modify the value (a copy is created)
*ptr = 100;

// Access the modified value
int modifiedValue = *ptr;

// Access the original value
int originalValue = *constPtr;
```


The cow_ptr class provides the following features:

- Construction: You can create a cow_ptr object by passing a raw pointer to the constructor.
- Copy Semantics: The class supports copy construction and copy assignment, creating a new reference to the same object.
- Move Semantics: The class supports move construction and move assignment, transferring ownership of the object to the new instance.
- Dereference Operator: You can use the dereference operator (*) to access the value pointed to by the cow_ptr. Non-const access to the object will create a copy of the object if there are other instances refer to the same object.
- Arrow Operator: You can use the arrow operator (->) to access members of the object pointed to by the cow_ptr. Non-const access to the object will create a copy of the object if there are other instances refer to the same object.
- Equality Comparison: The class supports equality (==) and inequality (!=) comparisons between cow_ptr objects. The comparison is made between the stored pointers.
- Reset: The reset function allows you to reset the cow_ptr to a new value or nullptr.
- get: The get function returns the raw pointer to the const object pointed to by the cow_ptr.
- getMutable: The getMutable function returns the raw pointer to the object pointed to by the cow_ptr. If there are other instances refer to the same object, a copy of the object will be created.

The non-member helper functions:
- make_cow: The make_cow function creates a new cow_ptr object passing its arguments to constructor.

## Thread Safety

cow_ptr uses an atomic reference count to track the number of references to the object. The reference count is assigned to 1 for a new cow_ptr object and is incrementing if existing cow_ptr object is copied. The reference count is decremented when a cow_ptr object is destroyed. When the reference count reaches zero, the destructor of last instance delete the stored object.

## License

This library is released under the BSD License. See the LICENSE file for more information.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request on the GitHub repository.