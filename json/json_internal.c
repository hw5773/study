#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json_internal.h"

struct json_object *init_json_object(void)
{
  struct json_object *jobj;
  jobj = (struct json_object *)malloc(sizeof(struct json_object));

  jobj->num = 0;
  jobj->next = NULL;
  jobj->head = NULL;
  jobj->tail = NULL;

  return jobj;
}

void free_json_object(struct json_object *jobj)
{
  struct kval *e, *f;

  e = jobj->head;
  f = NULL;

  do {
    f = e->next;
    free_kval_element(e);
    e = f;
  } while (e != NULL);

  jobj->num = 0;
  jobj->next = NULL;
  jobj->head = NULL;
  jobj->tail = NULL;
  jobj = NULL;
}

struct json_list *init_json_list(void)
{
  struct json_list *jlst;
  jlst = (struct json_list *)malloc(sizeof(struct json_list));

  jlst->num = 0;
  jlst->head = NULL;
  jlst->tail = NULL;

  return jlst;
}

void free_json_list(struct json_list *jlst)
{
  struct json_object *obj, *tmp;

  obj = jlst->head;
  tmp = NULL;

  do {
    tmp = obj->next;
    free_json_object(obj);
    obj = tmp;
  } while (obj != NULL);

  jlst->num = 0;
  jlst->head = NULL;
  jlst->tail = NULL;
}

void add_json_object_to_json_list(struct json_list *jlst, struct json_object *jobj)
{
  struct json_object *obj;

#ifdef DEBUG
  assert(jlst != NULL);
  assert(jobj != NULL);
#endif /* DEBUG */

  obj = jlst->tail;
  if (!obj)
  {
    jlst->head = jobj;
    jlst->tail = jobj;
  }
  else
  { 
    obj->next = jobj;
    jlst->tail = jobj;
  }
  jlst->num++;
}

struct kval *init_kval_element(void)
{
  struct kval *e;

  e = (struct kval *)malloc(sizeof(struct kval));
  e->key = NULL;
  e->klen = 0;
  e->type = JSON_TYPE_UNKNOWN;
  e->value = NULL;
  e->vlen = 0;
  e->next = NULL;

  return e;
}

struct kval *make_kval_element(char *ptr, int len)
{
  struct kval *e;
  char ch;
  char *c, *p, *q;
  int type;
  assert(ptr != NULL);
  assert(len > 0);

  e = init_kval_element();
  type = 0;


  // Key
  p = ptr;
  c = strstr(p, ":");
  q = c - 1;

#ifdef DEBUG
  assert(p < c);
  assert(c > 0);
#endif /* DEBUG */

  while (*p == ' ' || *p == '\"')
    p++;

  while (*q == ' ' || *q == '\"')
    q--;

  q += 1;

#ifdef DEBUG
  assert(p != q);
#endif /* DEBUG */

  e->key = p;
  e->klen = q - p;

  // Value
  p = c + 1;
  q = ptr + len - 1;
  

  while (*p == ' ')
    p++;

  while (*q == ' ')
    q--;

  switch (*p)
  {
    case '\"':
      if (*q != '\"') goto err;
      e->type = JSON_TYPE_STRING;
      break;

    case '[':
      if (*q != ']') goto err;
      e->type = JSON_TYPE_JSON_LIST;
      break;

    case 't':
      if (!check_boolean(p, q, TRUE)) goto err;
      e->type = JSON_TYPE_BOOLEAN_TRUE;
      break;

    case 'f':
      if (!check_boolean(p, q, FALSE)) goto err;
      e->type = JSON_TYPE_BOOLEAN_FALSE;
      break;

    case '{':
      if (*q != '}') goto err;
      e->type = JSON_TYPE_JSON_OBJECT;
      break;
    default:
      if (char_to_int(p, q) >= 0)
        e->type = JSON_TYPE_INTEGER;
      else
        e->type = JSON_TYPE_UNKNOWN;
  }

  q += 1;

#ifdef DEBUG
  assert(p != q);
#endif /* DEBUG */
  e->value = p;
  e->vlen = q - p;

#ifdef DEBUG
  print_kval_element(e);
#endif /* DEBUG */
  return e;
err:
  printf("Type error happened\n");
  if (e)
    free_kval_element(e);
  return NULL;
}

void free_kval_element(struct kval *e)
{
#ifdef DEBUG
  assert(e != NULL);
#endif /* DEBUG */

  e->key = NULL;
  e->klen = 0;
  e->type = JSON_TYPE_UNKNOWN;
  e->value = NULL;
  e->vlen = 0;
  e->next = NULL;

#ifdef DEBUG
  assert(e->key == NULL);
  assert(e->value == NULL);
  assert(e->next == NULL);
#endif /* DEBUG */
}

void print_kval_element(struct kval *e)
{
#ifdef DEBUG
  assert(e != NULL);
  assert(e->key != NULL);
  assert(e->klen > 0);
  assert(e->type > 0);
  assert(e->value != NULL);
  assert(e->vlen > 0);
#endif /* DEBUG */

  int len;
  char *buf;

  len = e->klen > e->vlen? e->klen : e->vlen;
  buf = (char *)malloc(len + 1);

  memset(buf, 0x0, len + 1);
  memcpy(buf, e->key, e->klen);
  printf("Key: %s\n", buf);

  switch(e->type)
  {
    case JSON_TYPE_BOOLEAN_TRUE:
    case JSON_TYPE_BOOLEAN_FALSE:
      printf("Type: boolean\n");
      break;
    case JSON_TYPE_INTEGER:
      printf("Type: integer\n");
      break;
    case JSON_TYPE_STRING:
      printf("Type: string\n");
      break;
    case JSON_TYPE_JSON_LIST:
      printf("Type: json list\n");
      break;
    case JSON_TYPE_JSON_OBJECT:
      printf("Type: json object\n");
      break;
    default:
      printf("Type: unknown\n");
  }

  memset(buf, 0x0, len + 1);
  memcpy(buf, e->value, e->vlen);
  printf("Value: %s\n", buf);
  printf("\n");
}

void add_kval_element_to_json_object(struct json_object *jobj, struct kval *elem)
{
  struct kval *e;

#ifdef DEBUG
  assert(jobj != NULL);
  assert(elem != NULL);
#endif /* DEBUG */

  if (jobj->tail)
  {
    e = jobj->tail;
    e->next = elem;
    jobj->tail = elem;
  }
  else
  {
    jobj->head = elem;
    jobj->tail = elem;
  }
  jobj->num++;
}

struct kval *get_kval_element_from_json_object(struct json_object *jobj, 
    char *key, int klen)
{
  struct kval *e, *ret;

#ifdef DEBUG
  assert(jobj != NULL);
  assert(key != NULL);
  assert(klen > 0);
#endif /* DEBUG */
  
  e = jobj->head;
  ret = NULL;

  while (e)
  {
    if (klen == e->klen)
    {
      if (!strncmp(key, e->key, klen))
      {
        ret = e;
        break;
      }
    }
    e = e->next;
  }

  return ret;
}

int check_boolean(char *p, char *q, int type)
{
  assert(type == TRUE || type == FALSE);

  switch (type)
  {
    case TRUE:
      if ((q - p + 1) != TRUE_LENGTH) goto err;
      if (p[0] != 't') goto err;
      if (p[1] != 'r') goto err;
      if (p[2] != 'u') goto err;
      if (p[3] != 'e') goto err;
      break;
    case FALSE:
      if ((q - p + 1) != FALSE_LENGTH) goto err;
      if (p[0] != 'f') goto err;
      if (p[1] != 'a') goto err;
      if (p[2] != 'l') goto err;
      if (p[3] != 's') goto err;
      if (p[4] != 'e') goto err;
      break;
    default:
      goto err;
  }
  return 1;
err:
  return 0;
}

int char_to_int(char *p, char *q)
{
  int i;
  int ret, len;
  char ch;

  ret = 0;
  len = q - p + 1;

  for (i=0; i<len; i++)
  {
    ch = p[i];
    switch(ch)
    {
      case '0':
        ret *= 10;
        continue;
      case '1':
        ret = ret * 10 + 1;
        continue;
      case '2':
        ret = ret * 10 + 2;
        continue;
      case '3':
        ret = ret * 10 + 3;
        continue;
      case '4':
        ret = ret * 10 + 4;
        continue;
      case '5':
        ret = ret * 10 + 5;
        continue;
      case '6':
        ret = ret * 10 + 6;
        continue;
      case '7':
        ret = ret * 10 + 7;
        continue;
      case '8':
        ret = ret * 10 + 8;
        continue;
      case '9':
        ret = ret * 10 + 9;
        continue;
      default:
        ret = -1;
    }
  }

  return ret;
}
