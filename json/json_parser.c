#include "json.h"
#include "json_internal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct json_object *get_json_object_from_str(char *str, int total)
{
  int idx, len, level;
  int kv; // if 0, key; otherwise, value
  int lst; // if 0 "not in list"; otherwise, "in list"
  char ch;
  char *cptr, *nptr;
  struct json_object *jobj;
  struct kval *e;
  char buf[1024];

  level = 0;
  lst = 0;
  kv = 0;

  idx = 0;
  cptr = str;

  jobj = init_json_object();

  while (idx < total)
  {
    ch = str[idx];

    switch(ch)
    {
      case '{':
        level++;
        if (level == 1)
          cptr = str + idx + 1;
        break;
      case '}':
        level--;
        if (level < 0) goto err;
        break;
      case ',':
        if (level == 1 && lst == 0)
        {
          nptr = str + idx;
          e = make_kval_element(cptr, nptr - cptr);
          cptr = nptr + 1;
          add_kval_element_to_json_object(jobj, e);
        }
        break;
      case '[':
        lst++;
        break;
      case ']':
        lst--;
        break;
    }
    idx++;
  }

  nptr = str + idx - 1;
  e = make_kval_element(cptr, nptr - cptr);
  add_kval_element_to_json_object(jobj, e);

  return jobj;
err:
  return NULL;
}

struct json_object *get_json_object_by_key(struct json_object *jobj, char *key)
{
  int klen;
  struct kval *e;
  char *str;
#ifdef DEBUG
  assert(jobj != NULL);
  assert(key != NULL);
#endif /* DEBUG */

  klen = strlen(key);
#ifdef DEBUG
  assert(klen > 0);
#endif /* DEBUG */
  e = get_kval_element_from_json_object(jobj, key, klen);
  if (!e) goto err;
  
  return get_json_object_from_str(e->value, e->vlen);

err:
  return NULL;
}

int get_int_value_by_key(struct json_object *jobj, char *key)
{
  struct kval *e;
#ifdef DEBUG
  assert(jobj != NULL);
  assert(key != NULL);
#endif /* DEBUG */
  e = get_kval_element_from_json_object(jobj, key, strlen(key));

#ifdef DEBUG
  assert(e != NULL);
  assert(e->type == JSON_TYPE_INTEGER);
#endif /* DEBUG */

  return char_to_int(e->value, e->value + e->vlen - 1);
}

char *get_str_value_by_key(struct json_object *jobj, char *key, int *len)
{
  struct kval *e;
#ifdef DEBUG
  assert(jobj != NULL);
  assert(key != NULL);
#endif /* DEBUG */
  e = get_kval_element_from_json_object(jobj, key, strlen(key));

#ifdef DEBUG
  assert(e != NULL);
  assert(e->type == JSON_TYPE_STRING);
#endif /* DEBUG */

  if (!e) goto err;
  if (e->type != JSON_TYPE_STRING) goto err;
  *len = e->vlen;
  return e->value;
err:
  return NULL;
}

int get_bool_value_by_key(struct json_object *jobj, char *key)
{
}

struct json_list *get_json_list_by_key(struct json_object *jobj, char *key)
{
  struct json_object *obj;
  struct json_list *ret;
  struct kval *e;
  char ch;
  char *str, *cptr, *nptr;
  int total, idx, lst, level;
#ifdef DEBUG
  char buf[2048];
#endif /* DEBUG */
  e = get_kval_element_from_json_object(jobj, key, strlen(key));
#ifdef DEBUG
  assert(e->type == JSON_TYPE_JSON_LIST);
  print_kval_element(e);
#endif /* DEBUG */

  if (!e) goto err;
  if (e->type != JSON_TYPE_JSON_LIST) goto err;

  idx = 0; level = 0; lst = 0;
  str = e->value;
  cptr = str;
  total = e->vlen;
  ret = init_json_list();

  while (idx < total)
  {
    ch = str[idx];

    switch(ch)
    {
      case '{':
        level++;
        if (level == 1)
          cptr = str + idx;
        break;
      case '}':
        level--;
        if (level < 0) goto err;
        if (level == 0 && lst == 1)
        {
          nptr = str + idx + 1;
#ifdef DEBUG
          memset(buf, 0x0, 2048);
          memcpy(buf, cptr, nptr - cptr);
          printf("String: %s\n", buf);
#endif /* DEBUG */
          obj = get_json_object_from_str(cptr, nptr - cptr);
          add_json_object_to_json_list(ret, obj);
        }
        break;
      case '[':
        lst++;
        break;
      case ']':
        lst--;
        if (lst < 0) goto err;
        break;
    }
    idx++;
  }

  return ret;
err:
  e = NULL;
  ret = NULL;
  return NULL;
}

int get_length_of_json_list(struct json_list *jlst)
{
  return jlst->num;
}

struct json_object *get_json_object_by_index(struct json_list *jlst, int idx)
{
  int i;
  struct json_object *obj;
#ifdef DEBUG
  assert(jlst != NULL);
  assert(idx < jlst->num);
#endif /* DEBUG */
  if (idx >= jlst->num) goto err;

  obj = jlst->head;
  for (i=0; i<idx; i++)
    obj = obj->next;

  return obj;
err:
  return NULL;
}

void print_json_object(struct json_object *jobj)
{
  struct kval *e;
  assert(jobj != NULL);
  e = jobj->head;
  
  printf("Number of elements: %d\n\n", jobj->num);
  
  while (e)
  {
    print_kval_element(e);
    e = e->next;
  }
}

void print_json_list(struct json_list *jlst)
{
  int i;
  struct json_object *obj;
  obj = jlst->head;

  printf("Number of JSON objects: %d\n\n", jlst->num);
  for (i=0; i<jlst->num; i++)
  {
    print_json_object(obj);
    obj = obj->next;
  }
}
