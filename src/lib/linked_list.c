#include "linked_list.h"
#include "iterator.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>


struct node {
  elem_t val;
  struct node *next;
};

typedef struct node node_t;


struct list {
  size_t count;
  node_t *first;
  node_t *last;
  ioopm_eq_function eq;
};


struct list_iterator {
  ioopm_list_t *list;
  node_t *cur;
  node_t *prev;
  // &list->first or &prev->next
  node_t **cur_pp;
};


static node_t *create_node(elem_t val, node_t *next)
{
  node_t *result = malloc(sizeof(node_t));
  *result = (node_t) {.val = val, .next = next};
  return result;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq)
{
  ioopm_list_t *result = malloc(sizeof(ioopm_list_t));
  *result = (ioopm_list_t) {.first = NULL, .last = NULL, .count = 0, .eq = eq};
  return result;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
  return list->count;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  return ioopm_linked_list_size(list) == 0;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  ioopm_linked_list_clear(list);
  free(list);
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
  node_t *cur = list->first;

  while (cur != NULL) {
    node_t *next = cur->next;
    free(cur);
    cur = next;
  }

    list->count = 0;
  list->first = NULL;
  list->last = NULL;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index)
{
#ifdef ASSERT_BOUNDS
  assert(index < ioopm_linked_list_size(list));
#endif
  node_t *cur = list->first;
  for (size_t i = 0; i < index; ++i)
    cur = cur->next;

  return cur->val;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
  node_t *cur = list->first;

  while (cur != NULL) {
    if (list->eq(cur->val, element))
      return true;
	
    cur = cur->next;
  }

  return false;
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
  node_t *new = create_node(value, NULL);

  if (list->count == 0)
    list->first = new;
  else
    list->last->next = new;

  list->last = new;
  list->count += 1;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
  node_t *new = create_node(value, list->first);

  if (list->count == 0)
    list->last = new;

  list->first = new;
  list->count += 1;
}

static void list_insert_middle(ioopm_list_t *list, size_t index, elem_t value)
{
    node_t *prev = list->first;
    for (size_t i = 1; i < index; ++i)
      prev = prev->next;

    node_t *new = create_node(value, prev->next);
    prev->next = new;
    list->count += 1;
}

void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
#ifdef ASSERT_BOUNDS
  assert(index <= ioopm_linked_list_size(list));
#endif
  if (index == 0)
    ioopm_linked_list_prepend(list, value);
  else if (index == list->count)
    ioopm_linked_list_append(list, value);
  else
    list_insert_middle(list, index, value);
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
#ifdef ASSERT_BOUNDS
  assert(index < ioopm_linked_list_size(list));
#endif
  node_t *prev = NULL;
  node_t *cur = list->first;
  node_t **cur_pp = &list->first;

  for (size_t i = 0; i < index; ++i) {
    cur_pp = &cur->next;
    prev = cur;
    cur = cur->next;
  }

  if (cur == list->last)
    list->last = prev;

  elem_t retval = cur->val;
  *cur_pp = cur->next;
  free(cur);
  list->count -= 1;
  return retval;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate pred, void *extra)
{
  node_t *cur = list->first;
  
  for (size_t idx = 0; idx < ioopm_linked_list_size(list); ++idx) {
    if (!pred(ioopm_unsigned_elem(idx), cur->val, extra))
      return false;

    cur = cur->next;
  }

  return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate pred, void *extra)
{
  node_t *cur = list->first;
  
  for (size_t idx = 0; idx < ioopm_linked_list_size(list); ++idx) {
    if (pred(ioopm_unsigned_elem(idx), cur->val, extra))
      return true;

    cur = cur->next;
  }

  return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function fun, void *extra)
{
  node_t *cur = list->first;
  
  for (size_t idx = 0; idx < ioopm_linked_list_size(list); ++idx) {
    fun(ioopm_unsigned_elem(idx), &cur->val, extra);

    cur = cur->next;
  }
}

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  ioopm_list_iterator_t *result = malloc(sizeof(ioopm_list_iterator_t));
  *result = (ioopm_list_iterator_t) {.list = list,
				     .cur = list->first,
				     .prev = NULL,
				     .cur_pp = &list->first};
  return result;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
  return iter->cur != NULL && iter->cur->next != NULL;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
#ifdef ASSERT_BOUNDS
  assert(ioopm_iterator_has_next(iter));
#endif
  iter->cur_pp = &iter->cur->next;
  iter->prev = iter->cur;
  iter->cur = iter->cur->next;
  return iter->cur->val;
}

elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
#ifdef ASSERT_BOUNDS
  assert(!ioopm_linked_list_is_empty(iter->list));
#endif
  if (iter->cur == iter->list->last)
    iter->list->last = iter->prev;

  elem_t retval = iter->cur->val;
  *iter->cur_pp = iter->cur->next;
  free(iter->cur);
  iter->cur = *iter->cur_pp;
  iter->list->count -= 1;
  return retval;
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
  node_t *new = create_node(element, iter->cur);
  iter->cur = new;
  *iter->cur_pp = new;
  iter->list->count += 1;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
  iter->cur = iter->list->first;
  iter->cur_pp = &iter->list->first;
  iter->prev = NULL;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
#ifdef ASSERT_BOUNDS
  assert(!ioopm_linked_list_is_empty(iter->list));
#endif
  return iter->cur->val;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
  free(iter);
}
