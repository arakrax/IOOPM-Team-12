#pragma once
#include <stdbool.h>
#include "common.h"

/**
 * @file iterator.h
 * @author Axel Bergström, Andreas Larsson
 * @date 03 Okt 2019
 * @brief A forward iterator for ioopm_list_t
 *
 */

typedef struct list_iterator ioopm_list_iterator_t;

/// @brief Checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if iterator has next
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one step.
/// @pre ioopm_iterator_has_next(iter) is true
/// @param iter the iterator
/// @return the element pointed to by the iterator after stepping forward
elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter);

/// @brief Remove the current element from the underlying list.
///        After this operation the current element will be
///        the next element after the removed one.
/// @pre The underlying list is not empty
/// @param iter the iterator
/// @return the removed element
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter);

/// @brief Insert a new element into the underlying list making the current element it's next.
///        After this operation the inserted element will be the current element.
/// @param iter the iterator
/// @param element the element to be inserted
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element);

/// @brief Reposition the iterator at the start of the underlying list
/// @param iter the iterator
void ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @pre The underlying list is not empty
/// @param iter the iterator
/// @return the current element
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter);

/// @brief Destroy the iterator and return its resources.
///        Does not destroy the list.
/// @param iter the iterator
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);
