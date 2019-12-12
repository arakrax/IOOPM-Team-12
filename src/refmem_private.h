#pragma once

#include "refmem.h"

/*
file for common structs and typedefs that should not be exported to the refmem interface (refmem.h)
 */


typedef struct header header_t;

struct header {
  int count;
  function1_t destroyer;
};

int refmem_get_count(header_t *header);
function1_t refmem_get_destroyer(header_t *header);
void destroy_environment();
