#include "refmem.h"
#include "refmem_private.h"
#include "environment.h"
#include <assert.h>

#define DEVILS_CONSTANT 666

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

void destroy_environment() {
  if(!environment)
    return;

  refmem_environment_destroy(environment);
}

int refmem_get_count(header_t *header) {
  return header->count;
}

function1_t refmem_get_destroyer(header_t *header) {
  return header->destroyer;
}

size_t rc(obj *object)
{
  header_t **header_location = malloc(sizeof(header_t*));
  if(!environment || !object)
    return DEVILS_CONSTANT;
  // if result == true then header_location will have relevant value
  bool result = refmem_environment_lookup(environment, object, header_location);

  assert(result);
  size_t count = (*header_location)->count;

  free(header_location);
  return count;
}

void retain(obj *object)
{


 header_t **header_location = malloc(sizeof(header_t*));
  bool result = refmem_environment_lookup(environment, object, header_location);

  if (result)
    {
      (*header_location)->count += 1;
    }

  //TODO: object not found?
  free(header_location);
}

void release(obj *object){

  header_t **header_location = malloc(sizeof(header_t*));
  bool result = refmem_environment_lookup(environment, object, header_location);

  assert(result);

  if (rc(object) == 0)
    {
      deallocate(object);
      free(header_location);
      return;
    }

  (*header_location)->count -= 1;

  //TODO: object not found?
  free(header_location);
}

static header_t *header_create(int count, function1_t destructor)
{
  header_t *new_header = malloc(sizeof(header_t));
  new_header->count = count;
  new_header->destroyer = *destructor;
  return new_header;
}

obj *allocate(size_t bytes, function1_t destructor){
  if(!environment)
    initialize_environment();
  if(!destructor)
    destructor = *free;

  obj *object_pointer = malloc(bytes);
  header_t *header = header_create(0, destructor);
  refmem_environment_insert(environment, object_pointer, header);
  return object_pointer;
}

void deallocate(obj *object)
{
  if(!environment || !object)
    return;

  header_t **header_location = malloc(sizeof(header_t*));
  if(refmem_environment_exists(environment, object))
    {
      if(rc(object) == 0)
	{
	  refmem_environment_remove(environment, object, header_location);
	  function1_t destructor = (*header_location)->destroyer;
	  destructor(object);
	  free(*header_location);
	  free(header_location);
	}
      else
	{
	  free(header_location);
	  return;
	}
    }
  else
    {
      free(header_location);
      return;
    }
}


/*
void deallocate(obj *object){
  header_t *header_location;

  if(!environment || !object || !header_location)
    return;

  bool result = false;
  result = refmem_environment_lookup(environment, object, header_location);

  if (result && header_location->destroyer)
    {
      if (header_location->count == 0 && header_location->destroyer){
	refmem_environment_remove(environment, object, header_location);
	if(object)
	  header_location->destroyer(object);
      }
    }
  else
    return;
  //TODO: object not found?
  //TODO: rc not 0?
}

*/
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
