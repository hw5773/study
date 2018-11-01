#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "list.h"

struct child_st;
struct dummy_st;

struct parent_st
{
  uint8_t *name;
  uint32_t nlen;
  uint32_t num;

  struct parent_st *prev;
  struct parent_st *next;
  struct parent_ops *ops;

  struct child_st *head;
};

struct child_st
{
  uint8_t *name;
  uint32_t nlen;
  uint32_t num;

  struct child_st *prev;
  struct child_st *next;
  struct child_ops *ops;

  struct dummy_st *head;
};

struct dummy_st {};

INIT_LIST_OPS(parent, child);

int main(int argc, char *argv[])
{
  struct child_st *c1;
  struct child_st *c2;
  struct child_st *c3;
  struct child_st *tmp;
  uint8_t s1[] = "The first node";
  uint8_t s2[] = "The second node";
  uint8_t s3[] = "The third node";

  INIT_LIST(parent, p);

  p->ops->create(&c1, s1, strlen(s1));
  p->ops->add(p, c1);
  p->ops->print(p);

  p->ops->create(&c2, s2, strlen(s2));
  p->ops->add(p, c2);
  p->ops->print(p);

  p->ops->create(&c3, s3, strlen(s3));
  p->ops->add(p, c3);
  p->ops->print(p);

  tmp = p->ops->get(p, "The third node", 14);
  if (tmp)
    printf("Get Right? name (%d bytes): %s\n", tmp->nlen, tmp->name);
  else
    printf("Not found\n");

  p->ops->del(p, c2);
  p->ops->print(p);

  return 0;
}
