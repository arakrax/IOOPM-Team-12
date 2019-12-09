#include "environment.h"
#include "lib/common.h"
#include "lib/hash_table.h"

/**
 * @file environment.c
 * @author Team 12 of IOOPM 19
 * @date 08 Dec 2019
 * @brief The interaction funcitons between the reference counter and the data structure.
 *
 */

struct environment
{
  ioopm_hash_table_t *environment;
};

/// @brief Compares if a & b points to the same object (place in memory)
static bool object_eq(elem_t a, elem_t b)
{
  return a.voidp == b.voidp;
}

/// @brief Gives the memory address as hash value for an object
static int object_hash(elem_t object)
{
  return (size_t)(object.voidp);
}
environment_t *refmem_environment_create()
{
  environment_t *environment = malloc(sizeof(environment_t));
  environment->environment = ioopm_hash_table_create(object_hash, object_eq, object_eq);
  return environment;
}

void refmem_environment_insert(environment_t *environment, void *object, header_t *header)
{
  ioopm_hash_table_insert(environment->environment, ioopm_voidp_elem(object), ioopm_voidp_elem(header));
}

bool refmem_environment_exists(environment_t *environment, void *object)
{
  return ioopm_hash_table_has_key(environment->environment, ioopm_voidp_elem(object));
}

bool refmem_environment_lookup(environment_t *environment, void *object, header_t *header_location)
{
  elem_t location;
  bool result = ioopm_hash_table_lookup(environment->environment, ioopm_voidp_elem(object), &location);
  header_location->count     = ((header_t*)location.voidp)->count;
  header_location->destroyer = ((header_t*)location.voidp)->destroyer;
  return result;
}

bool refmem_environment_remove(environment_t *environment, void *object, header_t *header_location)
{
  elem_t location;
  bool result = ioopm_hash_table_remove(environment->environment, ioopm_voidp_elem(object), &location);
  header_location->count     = ((header_t*)location.voidp)->count;
  header_location->destroyer = ((header_t*)location.voidp)->destroyer;
  return result;
}

void refmem_environment_clear(environment_t *environment)
{
  ioopm_hash_table_clear(environment->environment);
}

void refmem_environment_destroy(environment_t *environment)
{
  ioopm_hash_table_destroy(environment->environment);
  free(environment);
}
