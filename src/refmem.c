#include "refmem.h"
#include "refmem_private.h"
#include <assert.h>

struct cell
{
  struct cell *cell;
  int i;
  char *string;
};

int refmem_get_count(header_t *header) {
  return header->count;
}

function1_t *refmem_get_destroyer(header_t *header) {
  return header->destroyer;
}

/*
void cell_destructor(obj c)
{
  release(((struct cell *) c)->cell);
}

int main(void)
{
  struct cell *c = allocate(sizeof(struct cell), cell_destructor);
  assert(rc(c) == 0);
  retain(c);
  assert(rc(c) == 1);

  c->cell = allocate(sizeof(struct cell), cell_destructor);
  assert(rc(c->cell) == 0);
  retain(c->cell);
  assert(rc(c->cell) == 1);

  c->cell->cell = NULL;

  release(c);

  return 0;
}
*/
