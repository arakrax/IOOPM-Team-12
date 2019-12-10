#include "refmem.h"
#include "refmem_private.h"
#include "environment.h"
#include <assert.h>


struct cell
{
  struct cell *cell;
  int i;
  char *string;
};

environment_t *environment = NULL;

void initialize_environment(){
  environment = refmem_environment_create();
}

int refmem_get_count(header_t *header) {
  return header->count;
}

function1_t *refmem_get_destroyer(header_t *header) {
  return header->destroyer;
}

size_t rc(obj *object){
  header_t *header_location;
  bool result = refmem_environment_lookup(environment, object, header_location);

  if (result)
    {
      return refmem_get_count(header_location);
    }
  //TODO: object not found?
}

void retain(obj *object)
{
  header_t *header_location;
  bool result = refmem_environment_lookup(environment, object, header_location);

  if (result)
    {
      header_location->count++;
    }
  //TODO: object not found?
}

void release(obj *object){

  header_t *header_location;
  bool result = refmem_environment_lookup(environment, object, header_location);

  if (result)
    {
      header_location->count--;
      if (rc(header_location) == 0){
	deallocate(object);
      }
    }
  //TODO: object not found?
}

void deallocate(obj *object){
  header_t *header_location;
  bool result = refmem_environment_lookup(environment, object, header_location);

  if (result)
    {
      if (rc(header_location) == 0){
	refmem_environment_remove(environment, object, header_location);
      }
    }
  //TODO: object not found?
  //TODO: rc not 0?
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
