# cow_ptr

cow_ptr is a C++ smart pointer class that provides copy-on-write semantics. It allows multiple references to the same object, but only creates a copy of the object when it's accessed by non-const method and there are other instances refer to the same object.

## Usage
To use cow_ptr, include the header file and create an instance of the class with the appropriate type:
```c++
#include "cow_ptr.h"

// Create a cow_ptr with an initial value
nonstd::cow_ptr<int> ptr(new int(42));

// Access the value using the dereference operator
int value = *ptr;

// Modify the value (a copy is created)
*ptr = 100;

// Access the modified value
int modifiedValue = *ptr;

```
The cow_ptr class provides the following features:

- Construction: You can create a cow_ptr object by passing a raw pointer to the constructor.
- Copy Semantics: The class supports copy construction and copy assignment, creating a new reference to the same object.
- Move Semantics: The class supports move construction and move assignment, transferring ownership of the object to the new instance.
- Dereference Operator: You can use the dereference operator (*) to access the value pointed to by the cow_ptr. If the object is modified, a copy is created before modification.
- Arrow Operator: You can use the arrow operator (->) to access members of the object pointed to by the cow_ptr. If the object is modified, a copy is created before modification.
- Equality Comparison: The class supports equality (==) and inequality (!=) comparisons between cow_ptr objects.
- Reset Function: The reset function allows you to reset the cow_ptr to a new value or nullptr.
- get Function: The get function returns the raw pointer to the const object pointed to by the cow_ptr.
- getMutable Function: The getMutable function returns the raw pointer to the object pointed to by the cow_ptr. If the object is modified, a copy is created before modification.
## Thread Safety

cow_ptr provides thread safety for concurrent read access. Multiple threads can safely read from the same cow_ptr object without any synchronization. However, modifications to the object are not thread-safe and should be properly synchronized if multiple threads are involved.

## License

This library is released under the BSD License. See the LICENSE file for more information.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request on the GitHub repository.