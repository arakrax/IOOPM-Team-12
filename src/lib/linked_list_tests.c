#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "linked_list.h"

// Test data

// some test asume test values are unique
static int not_in_test_data = 1234;
static int test_values[] = {1, 2, -1, -2, 0, 123, 435, 95, 45, 67, 98, 34, 56};
static size_t test_values_cnt = sizeof(test_values) / sizeof(int);

// Helper functions

static bool int_eq(elem_t a, elem_t b)
{
  return a.i == b.i;
}

static ioopm_list_t *test_data_list()
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (size_t i = 0; i < test_values_cnt; ++i)
    ioopm_linked_list_insert(list, i, ioopm_int_elem(test_values[i]));
  
  return list;
}

// Setup functions

static int init_list_suite()
{
  return 0;
}

static int clean_list_suite()
{
 return 0;
}

// Test functions

void list_insert_get_test()
{
  ioopm_list_t *list = test_data_list();

  for (size_t i = 0; i < test_values_cnt; ++i)
    CU_ASSERT(ioopm_linked_list_get(list, i).i == test_values[i]);

  for (size_t i = 0; i < test_values_cnt; ++i)
    ioopm_linked_list_insert(list, 2*i, ioopm_int_elem(test_values[i]));

  for (size_t i = 0; i < test_values_cnt; ++i)
    CU_ASSERT(ioopm_linked_list_get(list, 2*i).i == test_values[i]);

  ioopm_linked_list_destroy(list);
}

void list_prepend_append_test()
{
  ioopm_list_t *list = test_data_list();

  for (size_t i = 0; i < test_values_cnt; ++i) {
    ioopm_linked_list_append(list, ioopm_int_elem(test_values[i]));

    size_t size = test_values_cnt + i + 1;
    CU_ASSERT(ioopm_linked_list_get(list, size - 1).i == test_values[i]);
  }

  for (size_t i = 0; i < test_values_cnt; ++i) {
    ioopm_linked_list_prepend(list, ioopm_int_elem(test_values[i]));

    CU_ASSERT(ioopm_linked_list_get(list, 0).i == test_values[i]);
  }

  ioopm_linked_list_destroy(list);
}

void list_contains_test()
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (size_t i = 0; i < test_values_cnt; ++i)
    CU_ASSERT_FALSE(ioopm_linked_list_contains(list, ioopm_int_elem(test_values[i])));

  for (size_t i = 0; i < test_values_cnt; ++i) {
    ioopm_linked_list_insert(list, i, ioopm_int_elem(test_values[i]));
    CU_ASSERT(ioopm_linked_list_contains(list, ioopm_int_elem(test_values[i])));
  }

  for (size_t i = 0; i < test_values_cnt; ++i)
    CU_ASSERT(ioopm_linked_list_contains(list, ioopm_int_elem(test_values[i])));

  ioopm_linked_list_destroy(list);
}

void list_remove_first_test()
{
  ioopm_list_t *list = test_data_list();

  for (size_t i = 0; i < test_values_cnt; ++i) {
    elem_t value = ioopm_linked_list_get(list, 0);
    elem_t ret = ioopm_linked_list_remove(list, 0);

    CU_ASSERT(value.i == ret.i);
    CU_ASSERT_FALSE(ioopm_linked_list_contains(list, value));
  }

  ioopm_linked_list_destroy(list);
}

void list_remove_middle_test()
{
  ioopm_list_t *list = test_data_list();
  int mid = test_values_cnt / 2;

  for (size_t i = mid; i < test_values_cnt; ++i) {
    elem_t value = ioopm_linked_list_get(list, mid);
    elem_t ret = ioopm_linked_list_remove(list, mid);

    CU_ASSERT(value.i == ret.i);
    CU_ASSERT_FALSE(ioopm_linked_list_contains(list, value));
  }

  ioopm_linked_list_destroy(list);
}

void list_remove_last_test()
{
  ioopm_list_t *list = test_data_list();

  for (size_t i = test_values_cnt - 1; i < test_values_cnt; --i) {
    elem_t value = ioopm_linked_list_get(list, i);
    elem_t ret = ioopm_linked_list_remove(list, i);

    CU_ASSERT(value.i == ret.i);
    CU_ASSERT_FALSE(ioopm_linked_list_contains(list, value));
  }

  ioopm_linked_list_destroy(list);
}

void list_size_test()
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  CU_ASSERT(ioopm_linked_list_size(list) == 0);

  for (size_t i = 0; i < test_values_cnt; ++i) {
    ioopm_linked_list_insert(list, i, ioopm_int_elem(test_values[i]));

    size_t expected_size = i + 1;
    CU_ASSERT(ioopm_linked_list_size(list) == expected_size);
  }

  for (size_t i = 0; i < test_values_cnt; ++i) {
    ioopm_linked_list_remove(list, 0);

    size_t expected_size = test_values_cnt - i - 1;
    CU_ASSERT(ioopm_linked_list_size(list) == expected_size);
  }

  ioopm_linked_list_destroy(list);
}

void list_is_empty_test()
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  CU_ASSERT(ioopm_linked_list_is_empty(list));
  ioopm_linked_list_prepend(list, ioopm_int_elem(0));
  CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));
  ioopm_linked_list_remove(list, 0);
  CU_ASSERT(ioopm_linked_list_is_empty(list));

  ioopm_linked_list_destroy(list);
}

void list_clear_test()
{
  ioopm_list_t *list = test_data_list();

  CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));

  ioopm_linked_list_clear(list);
  CU_ASSERT(ioopm_linked_list_size(list) == 0);
  CU_ASSERT(ioopm_linked_list_is_empty(list));

  ioopm_linked_list_destroy(list);
}

static bool val_eq_pred(elem_t key, elem_t value, void *cmp_to)
{
  return value.i == ((elem_t*) cmp_to)->i;
}

void list_any_test()
{
  ioopm_list_t *list = test_data_list();

  for (size_t i = 0; i < test_values_cnt; ++i)
    CU_ASSERT(ioopm_linked_list_any(list, val_eq_pred, &ioopm_int_elem(test_values[i])));

  CU_ASSERT_FALSE(ioopm_linked_list_any(list, val_eq_pred, &ioopm_int_elem(not_in_test_data)));

  ioopm_linked_list_destroy(list);
}

static bool in_test_values_pred(elem_t key, elem_t value, void *extra)
{
  for (size_t i = 0; i < test_values_cnt; ++i)
    if (value.i == test_values[i])
      return true;

  return false;
}

void list_all_test()
{
  ioopm_list_t *list = test_data_list();

  CU_ASSERT(ioopm_linked_list_all(list, in_test_values_pred, NULL));
  CU_ASSERT_FALSE(ioopm_linked_list_all(list, val_eq_pred, &ioopm_int_elem(test_values[1])));

  ioopm_linked_list_destroy(list);
}

static void add_one_apply_fun(elem_t key, elem_t *val, void *extra)
{
  val->i += 1;
}

void list_apply_test()
{
  ioopm_list_t *list = test_data_list();

  ioopm_linked_list_apply_to_all(list, add_one_apply_fun, NULL);

  for (size_t i = 0; i < test_values_cnt; ++i)
    CU_ASSERT(ioopm_linked_list_get(list, i).i == test_values[i] + 1);

  ioopm_linked_list_destroy(list);
}

void list_iterator_current_next_test()
{
  ioopm_list_t *list = test_data_list();
  ioopm_list_iterator_t *it = ioopm_list_iterator(list);

  for (size_t i = 0; i < test_values_cnt - 1; ++i) {
    CU_ASSERT(ioopm_iterator_current(it).i == test_values[i]);
    elem_t next = ioopm_iterator_next(it);
    CU_ASSERT(next.i == test_values[i + 1]);
  }

  CU_ASSERT(ioopm_iterator_current(it).i == test_values[test_values_cnt - 1]);

  ioopm_iterator_destroy(it);
  ioopm_linked_list_destroy(list);
}

void list_iterator_has_next_test()
{
  ioopm_list_t *list = test_data_list();
  ioopm_list_iterator_t *it = ioopm_list_iterator(list);

  for (size_t i = 0; i < test_values_cnt - 1; ++i) {
    CU_ASSERT(ioopm_iterator_has_next(it));
    ioopm_iterator_next(it);
  }

  CU_ASSERT_FALSE(ioopm_iterator_has_next(it));

  ioopm_iterator_destroy(it);
  ioopm_linked_list_destroy(list);
}

void list_iterator_insert_test()
{
  ioopm_list_t *list = test_data_list();
  ioopm_list_iterator_t *it = ioopm_list_iterator(list);

  size_t idx = test_values_cnt / 2;
  int val = 123321;

  for (size_t i = 0; i < idx; ++i)
    ioopm_iterator_next(it);

  ioopm_iterator_insert(it, ioopm_int_elem(val));
  CU_ASSERT(ioopm_iterator_current(it).i == val);
  CU_ASSERT(ioopm_linked_list_get(list, idx).i == val);

  size_t expected_size = test_values_cnt + 1;
  CU_ASSERT(ioopm_linked_list_size(list) == expected_size);

  for (size_t i = 0; i < test_values_cnt; ++i) {
    ioopm_iterator_insert(it, ioopm_int_elem(test_values[i]));
    ioopm_iterator_next(it);

    expected_size += 1;
    CU_ASSERT(ioopm_linked_list_size(list) == expected_size);
  }

  for (size_t i = 0; i < test_values_cnt; ++i)
    CU_ASSERT(ioopm_linked_list_get(list, idx + i).i == test_values[i]);

  ioopm_iterator_destroy(it);
  ioopm_linked_list_destroy(list);
}

void list_iterator_reset_test()
{
  ioopm_list_t *list = test_data_list();
  ioopm_list_iterator_t *it = ioopm_list_iterator(list);

  size_t mid = test_values_cnt / 2;

  for (size_t i = 0; i < mid; ++i)
    ioopm_iterator_next(it);

  ioopm_iterator_reset(it);
  CU_ASSERT(ioopm_iterator_current(it).i == test_values[0]);

  ioopm_iterator_next(it);
  CU_ASSERT(ioopm_iterator_current(it).i == test_values[1]);

  ioopm_iterator_destroy(it);
  ioopm_linked_list_destroy(list);
}

void list_iterator_remove_test()
{
  ioopm_list_t *list = test_data_list();
  ioopm_list_iterator_t *it = ioopm_list_iterator(list);

  size_t mid = test_values_cnt / 2;

  for (size_t i = 0; i < mid; ++i)
    ioopm_iterator_next(it);

  ioopm_iterator_remove(it);
  ioopm_iterator_reset(it);

  size_t size = ioopm_linked_list_size(list);
  CU_ASSERT(size == test_values_cnt - 1);

  for (size_t i = 0; i < size - 1; ++i) {
    CU_ASSERT(ioopm_iterator_current(it).i != test_values[mid]);
    ioopm_iterator_next(it);
  }

  ioopm_iterator_remove(it);
  ioopm_iterator_reset(it);

  size = ioopm_linked_list_size(list);
  CU_ASSERT(size == test_values_cnt - 2);

  for (size_t i = 0; i < size - 2; ++i) {
    CU_ASSERT(ioopm_iterator_current(it).i != test_values[mid]);
    ioopm_iterator_next(it);
  }

  ioopm_iterator_destroy(it);
  ioopm_linked_list_destroy(list);
}

int main()
{
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite pSuiteList = CU_add_suite("linked list test suite", init_list_suite, clean_list_suite);

  if (pSuiteList == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (!CU_add_test(pSuiteList, "basic prepend_append_tests", list_prepend_append_test)
      || !CU_add_test(pSuiteList, "basic insert and get test", list_insert_get_test)
      || !CU_add_test(pSuiteList, "list remove test", list_remove_first_test)
      || !CU_add_test(pSuiteList, "list remove last test", list_remove_last_test)
      || !CU_add_test(pSuiteList, "list remove middle test", list_remove_middle_test)
      || !CU_add_test(pSuiteList, "list size test", list_size_test)
      || !CU_add_test(pSuiteList, "list is empty test", list_is_empty_test)
      || !CU_add_test(pSuiteList, "list clear test", list_clear_test)
      || !CU_add_test(pSuiteList, "list any test", list_any_test)
      || !CU_add_test(pSuiteList, "list all test", list_all_test)
      || !CU_add_test(pSuiteList, "list apply test", list_apply_test)
      || !CU_add_test(pSuiteList, "list contains test", list_contains_test)
      || !CU_add_test(pSuiteList, "iterator current and next test", list_iterator_current_next_test)
      || !CU_add_test(pSuiteList, "iterator has next test", list_iterator_has_next_test)
      || !CU_add_test(pSuiteList, "iterator insert test", list_iterator_insert_test)
      || !CU_add_test(pSuiteList, "iterator remove test", list_iterator_remove_test)
      || !CU_add_test(pSuiteList, "iterator reset test", list_iterator_reset_test)) {
    CU_cleanup_registry();
    return CU_get_error();
  }
    
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
