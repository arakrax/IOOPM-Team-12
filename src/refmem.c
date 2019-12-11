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

function1_t refmem_get_destroyer(header_t *header) {
  return header->destroyer;
}

size_t rc(obj *object){
  header_t *header_location = malloc(sizeof(header_t));
  // if result == true then header_location will have relevant value
  bool result = refmem_environment_lookup(environment, object, header_location);

  assert (result);
  int count = refmem_get_count(header_location);
  free(header_location);
  return count;
}

void retain(obj *object)
{
  header_t *header_location = malloc(sizeof(header_t));  
  bool result = refmem_environment_lookup(environment, object, header_location);

  assert(result);

  // increment count
  // discussing lookup should return pointer not copy
  
  free(header_location);
}

void release(obj *object){

  header_t *header_location = malloc(sizeof(header_t));
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


obj *allocate(size_t bytes, function1_t destructor){
  obj *objectPointer = malloc(bytes);
  header_t *header = malloc(sizeof(header_t));
  header->count = 0; // should count be = 1?
  header->destroyer = destructor;
  refmem_environment_insert(environment, objectPointer, header);
  return objectPointer;
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
