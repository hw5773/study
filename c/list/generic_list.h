#ifndef __GENERIC_LIST_H__
#define __GENERIC_LIST_H__

#define LIST 1

struct clist
{
  int num;
  uint8_t *name;
  uint32_t nlen;
  struct list *prev;
  struct list *next;
  struct list_ops *ops;
};

struct clist_ops = 
{
  int (*init)(struct list **head);
  int (*create)(struct list **node, uint8_t *name);
  int (*add)(struct list *first, struct list *second);
  int (*del)(struct list *first, struct list *second);
  struct list *(*get)(struct list *first, uint8_t *name);
  void (*print)(struct list *head);
}

int add(void *first, void *second, int type);

#endif /* __GENERIC_LIST_H__ */
