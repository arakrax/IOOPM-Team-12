Title:       README.md
Author:      Axel Bergström, Andreas Larsson
Date:        2019-10-07

# Assignment 1

## Building and Testing Commands

- `make` or `make freq_count` build the `freq_count` executable
- `make freq_count_grpof` build `freq_count` with support for gprof
- `make libs` builds `hash_table.o` and `linked_list.o`
- `make test` run all the CUnit tests in valgrind. Tests are written for the hash table and the linked list.
- `make test_gcov` same as above, but also print test coverage and creates `hash_table.c.gcov` and `linked_list.c.gcov`
- `make clean` remove all executables and object files

## Test Coverage as of 2019-10-07

Data is from the output of `make test_gcov`.


File 'hash\_table.c'  
Lines executed:100.00% of 156  
Branches executed:100.00% of 52  
Taken at least once:100.00% of 52  
Calls executed:100.00% of 30

File 'linked\_list.c'  
Lines executed:100.00% of 147  
Branches executed:100.00% of 50  
Taken at least once:86.00% of 50  
Calls executed:78.57% of 28


'Branches taken at least once' and 'Calls executed' is affected by bound checking in asserts.



## User Guide

Full documentation available in the header files.

### header files
  - `hash_table.h`
    containts the function definitions of the hash table interface
  - `linked_list.h`
    contains the function definitions of the linked list interface
  - `iterator.h`
    contains the function definitions if the iterators interface
  - `common.h`
    contains the definition of elem_t and some function typedefs.
    It is already included in all the other header files.

### supported data

The hash_table and linked_list supports elements of type `elem_t` (defined in `common.h`) which is a union of the types:
`int`, `unsigned`, `bool`, `float`, `char*`, and `void*`
The union members are called `i`, `u`, `b`, `f`, `charp`, and `voidp`.

Convenience macros are provided to construct an element of a specific type.
Theese macros are called `ioopm_*_elem` where `*` is one of `int`, `unsigned`, `bool`, `float`, `charp`, or `voidp` e.g. `ioopm_int_elem`.

### function types

The hash table contructur takes one `ioopm_hash_function` and two `ioopm_eq_function`s as arguments.

The linked list constructor takes one `ioopm_eq_function` as an argument.

An `ioopm_eq_function` `eq` should take two arguments of the same `elem_t`-type and satisfy
  - `eq(a, a)` => `eq(a, a)`
  - `eq(a, b)` => `eq(b, a)`
  - `eq(a, b)` and `eq(b, c)` => `eq(a, c)`
  
An `ioopm_hash_function` `hash` should take two arguments of the same `elem_t`-type and satisfy
  - `eq(a, b)` => `hash(a) == hash(b)`

An 'ioopm_predicate' 'pred' should take a `elem_t` `key`, `elem_t` `value`, and a `void*` `extra` used to pass in extra arguments.
When applied on the linked list the key corresponds to the index.

An 'ioopm_predicate' 'pred' should take a `elem_t` `key`, `elem_t*` `value`, and a `void*` `extra` used to pass in extra arguments.
When applied on the linked list the key corresponds to the index.
`value` will contain the address of the element in the collection, the value can be modified.

### Error handling

Two different types of error handling is used.

In the linked list bounds are checked using `assert`. See the `@pre` tags in the documentation.
The program will crash if an attempt is made to access an out of bounds index.

In the hash table some functions give back multiple values.
A success status is returned from the function and the result of the operation, if successfull, is written to a supplied pointer.
For examples, see the below hash table example usage.



### List of features
#### hash table
- basic hash table functionality: insert, lookup, remove, and clear
- `O(1)` size check and is empty check
- ability to get a list of all keys or values
- checking if a table has a key or value
- checking if any or all entries satisfy a given predicate
- applying a function to all entries
- support for custom initial size and load factor limits using the advanced constructor

#### linked list
- basic linked list functionality: insert, remove, get, prepend, and clear
- `O(1)` append
- `O(1)` size check and is empty check
- ability to check if a list contains a value
- checking if any or all elements satisfy a given predicate
- applying a function to all elements

#### iterator
Basic forward iterator supporting next, has_next, remove, insert, current, and reset.
 


### example usage
```c
  char *values[4] = {"one", "two", "three", "four"};


  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq, str_eq);

  for (size_t i = 0; i < 4; ++i)
    ioopm_hash_table_insert(ht, ioopm_int_elem(i), ioopm_charp_elem(values[i]));


  elem_t result;
  if(ioopm_hash_table_lookup(ht, ioopm_int_elem(2), &result))
    printf("%s\n", result.charp); // prints "two\n"

  if(ioopm_hash_table_lookup(ht, ioopm_int_elem(-123), &result))
    printf("%s\n", result.charp); // not executed

  
  if(ioopm_hash_table_remove(ht, ioopm_int_elem(3), &result))
    printf("%s\n", result.charp); // prints "three\n"


  ioopm_list_t *list = ioopm_hash_table_keys(ht);

  if (ioopm_linked_list_contains(list, ioopm_int_elem(2)))
    printf("conatains 2!\n");


  ioopm_list_iterator_t *it = ioopm_list_iterator(list);

  int cur = ioopm_iterator_current(it).i;
  printf("curent val: %d\n", cur);

  cur = ioopm_iterator_next(it).i;
  printf("curent val: %d\n", cur);



  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
```
