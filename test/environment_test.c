#include "../src/environment.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define ARRAY_LENGTH 18

static char *objects[ARRAY_LENGTH] = {"Would", "it", "save", "you", "a", "lot", "of", "time", "if", "I", "just", "gave", "up", "and", "went", "mad", "now", "?"};

static const size_t objects_count = sizeof(objects) / sizeof(char*);

// Helper functions
static void destructor(obj *object)
{
  free(object);
}

static header_t *header_create(int count, function1_t destructor)
{
  header_t *new_header = malloc(sizeof(header_t));
  new_header->count = count;
  new_header->destroyer = &destructor;
  return new_header;
}

static header_t **headers_array(int number_of_headers)
{
  header_t **headers = calloc(number_of_headers, sizeof(header_t));
  for(size_t i = 0; i < number_of_headers; ++i)
    {
      headers[i] = header_create(1, destructor);
    }
  return headers;
}

static bool header_eq(header_t *a, header_t *b)
{
  return a->count == b->count
      && a->destroyer == b->destroyer;
}

static void headers_array_destroy(header_t **array, size_t length)
{
  for(size_t i = 0; i < length; ++i)
    free(array[i]);
  free(array);
}

// Setup functions
static int init_ht_suite()
{
  return 0;
}

static int clean_ht_suite()
{
 return 0;
}

// tests for hash table integration to the environment interface
static void environment_insert_lookup_test() {
  environment_t *environment = refmem_environment_create();
  header_t **headers = headers_array(ARRAY_LENGTH);
  header_t *result = malloc(sizeof(header_t));

  for(size_t i = 0; i < objects_count; ++i)
    {
      refmem_environment_insert(environment, objects[i], headers[i]);

      CU_ASSERT(refmem_environment_lookup(environment, objects[i], result));
      CU_ASSERT(header_eq(result, headers[i]));
    }

  free(result);
  headers_array_destroy(headers, ARRAY_LENGTH);
  refmem_environment_destroy(environment);
}

static void environment_exists_test() {
  environment_t *environment = refmem_environment_create();
  header_t **headers = headers_array(ARRAY_LENGTH);

  for(size_t i = 0; i < objects_count; ++i)
    {
      refmem_environment_insert(environment, objects[i], headers[i]);
      CU_ASSERT(refmem_environment_exists(environment, objects[i]));
    }

  headers_array_destroy(headers, ARRAY_LENGTH);
  refmem_environment_destroy(environment);
}

static void environment_remove_test() {
  environment_t *environment = refmem_environment_create();
  header_t **headers = headers_array(ARRAY_LENGTH);
  header_t *result = malloc(sizeof(header_t));

  for(size_t i = 0; i < objects_count; ++i)
    {
      refmem_environment_insert(environment, objects[i], headers[i]);

      CU_ASSERT(refmem_environment_remove(environment, objects[i], result));
      CU_ASSERT(header_eq(result, headers[i]));
    }

  free(result);
  headers_array_destroy(headers, ARRAY_LENGTH);
  refmem_environment_destroy(environment);
}

static void environment_clear_test() {
  environment_t *environment = refmem_environment_create();
  header_t **headers = headers_array(ARRAY_LENGTH);

  for(size_t i = 0; i < objects_count; ++i)
    refmem_environment_insert(environment,objects[i], headers[i]);

  refmem_environment_clear(environment);
  CU_ASSERT_NOT_EQUAL(environment, NULL);

  for(size_t i = 0; i < objects_count; ++i)
    CU_ASSERT_FALSE(refmem_environment_exists(environment, objects[i]));

  headers_array_destroy(headers, ARRAY_LENGTH);
  refmem_environment_destroy(environment);
}

int main()
{
    if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

    CU_pSuite pSuiteEnvironment = CU_add_suite("environment and hash table integrations test suit", init_ht_suite, clean_ht_suite);

    if (pSuiteEnvironment == NULL)
      {
	CU_cleanup_registry();
	return CU_get_error();
      }

    if(!CU_add_test(pSuiteEnvironment, "environment insert and lookup test", environment_insert_lookup_test)
       || !CU_add_test(pSuiteEnvironment, "environemnt exists test", environment_exists_test)
       || !CU_add_test(pSuiteEnvironment, "enviromnent remove test", environment_remove_test)
       || !CU_add_test(pSuiteEnvironment, "environemnt clear test", environment_clear_test))
      {
	CU_cleanup_registry();
	return CU_get_error();
      }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
