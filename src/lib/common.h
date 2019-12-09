#pragma once
#include <stdbool.h>

/**
 * @file iterator.h
 * @author Axel Bergström, Andreas Larsson
 * @date 03 Okt 2019
 * @brief Common definition for hash_table.h, linked_list.h, and iterator.h
 *
 */

/// @typedef Union of the types int, unsigned, bool, float, char*, and void*
///          The union members are called i, u, b, f, charp, and voidp.
///          Convenience macros are provided to construct an element of a specific type.
///          Theese macros are called 'ioopm_*_elem' where '*' is one of
///          int, unsigned, bool, float, charp, or voidp.
typedef union elem elem_t;
#define ioopm_int_elem(e) ((elem_t) {.i = e})
#define ioopm_unsigned_elem(e) ((elem_t) {.u = e})
#define ioopm_bool_elem(e) ((elem_t) {.b = e})
#define ioopm_float_elem(e) ((elem_t) {.f = e})
#define ioopm_charp_elem(e) ((elem_t) {.charp = e})
#define ioopm_voidp_elem(e) ((elem_t) {.voidp = e})


/// @typedef predicate used in some functions on the hash table and linked list types
typedef bool(*ioopm_predicate)(elem_t key, elem_t value, void *extra);

/// @typedef apply function used in some functions on the hash table and linked list types
typedef void(*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);

/// @typedef function used to compare two elem_t:s
typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

/// @typedef function used to give a hash value for a elem_t
typedef int(*ioopm_hash_function)(elem_t elem);

union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  char *charp;
  void *voidp;
};
