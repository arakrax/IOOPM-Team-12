#pragma once
#include "refmem.h"
#include "refmem_private.h"
#include <stdbool.h>

/**
 * @file environment.h
 * @author Team 12 of IOOPM 19
 * @date 08 Dec 2019
 * @brief Interface for the environment in which the reference counter stores its data.
 *
 */


typedef struct environment environment_t;

/// @brief Create a new environment
/// @return A new empty environment
environment_t *refmem_environment_create();

/// @brief Insert an object with belonging header into the enviromnent
/// @param enviromnent the environment to hold the object
/// @param object the object to be inserted
/// @param header the heater to the object
void refmem_environment_insert(environment_t *environment, void *object, header_t *header);

/// @brief Check if an object exists in the environment
/// @param enviromnent the environment to hold the object
/// @param object the object to be looked up
/// @return true if object exists in environment
bool refmem_environment_exists(environment_t *environment, void *object);

/// @brief If a object exists in the enviromnent the object's header is returned
/// @param enviromnent the environment to hold the object
/// @param object the object to be looked up
/// @param header_location the location to place the hedaer if such exists
/// @return true if object exists in environment
/// @note The function uses a pointer to put the result in to handle bad in-data. The boolean returned from the function can be used by the user of the function for error-handling.
bool refmem_environment_lookup(environment_t *environment, void *object, header_t *header_location);

/// @brief Removed an object from the environment
/// @param enviromnent the environment where the object is stored
/// @param object the object to be removed
/// @param header the location where the removed header will be placed
/// @return true if object couble be found and removed
/// @note The function uses a pointer to put the result in to handle bad in-data. The boolean returned from the function can be used by the user of the function for error-handling.
bool refmem_environment_remove(environment_t *environment, void *object, header_t *header_location);

/// @brief Clears the environment from all entries
void refmem_environment_clear(environment_t *environment);

/// @brief Clears the environment from all entries and destroys the data structure
void refmem_environment_destroy(environment_t *environment);
