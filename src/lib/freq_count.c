#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(* (char * const *) p1, * (char * const *) p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
  elem_t result;
  if (!ioopm_hash_table_lookup(ht, ioopm_charp_elem(word), &result)) {
    result = ioopm_int_elem(0);
    word = strdup(word);
  }

  result.i += 1;
  ioopm_hash_table_insert(ht, ioopm_charp_elem(word), result);
}

void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");

  while (true) 
    {
      char *buf = NULL;
      size_t len = 0;
      getline(&buf, &len, f);

      if (feof(f))
        {
          free(buf);
          break;
        }
      
      for (char *word = strtok(buf, Delimiters);
           word != NULL && *word != '\0';
           word = strtok(NULL, Delimiters)
           )
        {
          process_word(word, ht);
        }

      free(buf);
    }
  
  fclose(f);
}

struct append_args {
  char **arr;
  int idx;
};

void append_val_to_array(elem_t key, elem_t *value, struct append_args *extra)
{
  extra->arr[extra->idx] = value->charp;
  extra->idx += 1;
}

void free_val(elem_t key, elem_t *value, void *extra)
{
  free(value->voidp);
}

static bool string_eq(elem_t a, elem_t b)
{
  return strcmp(a.charp, b.charp) == 0;
}

static bool int_eq(elem_t a, elem_t b)
{
  return a.i == b.i;
}

int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_string_knr_hash, string_eq, int_eq);
  
  if (argc > 1)
    {
      for (int i = 1; i < argc; ++i)
          process_file(argv[i], ht);

      int ht_size = ioopm_hash_table_size(ht);
      ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);
      
      char **keys = calloc(ht_size, sizeof(char*));

      struct append_args extra = {keys, 0};
      ioopm_linked_list_apply_to_all(keys_list, (ioopm_apply_function) append_val_to_array, &extra);

      sort_keys(keys, ht_size);
      for (int i = 0; i < ht_size; ++i) {
	elem_t result;
	ioopm_hash_table_lookup(ht, ioopm_charp_elem(keys[i]), &result);
	printf("%s: %d\n", keys[i], result.i);
      }

      ioopm_linked_list_apply_to_all(keys_list, free_val, NULL);
      ioopm_linked_list_destroy(keys_list);
      ioopm_hash_table_destroy(ht);
      free(keys);
    }
  else
    {
      puts("Usage: freq_count file1 ... filen");
    }
}
