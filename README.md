# Snek Objects with Reference Counting

## Overview

This project implements a polymorphic object system in C with reference counting–based memory management. It models dynamic language behavior (like Python) using a tagged union approach and supports multiple object types including integers, floats, strings, vectors, and arrays.

## Features

* Polymorphic object representation (`snek_object_t`)
* Reference counting garbage collection
* Supported types:

  * Integer, Float
  * String (heap-allocated)
  * Vector3 (composite type)
  * Dynamic Array
* Core operations:

  * Object creation (`new_snek_*`)
  * Reference management (`refcount_inc`, `refcount_dec`)
  * Polymorphic addition (`snek_add`)
  * Length calculation (`snek_length`)
  * Array access (`snek_array_set`, `snek_array_get`)

## Project Structure

```
.
├── snekobject.h   # Object definitions and API
├── snekobject.c   # Implementation
├── main.c         # Test suite (munit)
```

## Memory Management

* Each object maintains a `refcount`
* Increment on new references
* Decrement on release
* Automatically freed when count reaches zero
* Composite types recursively manage contained objects

## Build & Run

```bash
gcc main.c snekobject.c -o snek -lmunit
./snek
```

## Limitations

* No thread safety in reference counting
* Minimal error handling (mostly NULL returns)
* No strict encapsulation or modular layering
* Limited test coverage for edge cases

## Future Improvements

* Thread-safe reference counting
* Better error reporting and logging
* Stronger API boundaries and encapsulation
* Memory safety checks and leak detection
* Extended test coverage


