#include "../src/environment.h"
#include "../src/refmem.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

static header_t *header_create(int count, function1_t destructor)
{
  header_t *new_header = malloc(sizeof(header_t));
  new_header->count = count;
  new_header->destroyer = &destructor;
  return new_header;
}

static void free_destructor(obj *object){
  free(object);
}

static int init_rc_suite()
{
  return 0;
}

static int clean_rc_suite()
{
 return 0;
}


static void retain_release_rc_test(){
  initialize_environment();
  obj *object = allocate(4, free_destructor);

  
  /*CU_ASSERT(rc(object) == 0);
  retain(object);
  CU_ASSERT(rc(object) == 1);
  retain(object);
  CU_ASSERT(rc(object) == 2);
  release(object);
  CU_ASSERT(rc(object) == 1);
  release(object);
  //TODO: ASSERT OBJECT IS REMOVED*/
}


int main()
{
    if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

    CU_pSuite pSuiteRC = CU_add_suite("retain, release, rc test suite", init_rc_suite, clean_rc_suite);

    if (pSuiteRC == NULL)
      {
	CU_cleanup_registry();
	return CU_get_error();
      }

    if(!CU_add_test(pSuiteRC, "retain, release and rc test", retain_release_rc_test))
      {
	CU_cleanup_registry();
	return CU_get_error();
      }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

