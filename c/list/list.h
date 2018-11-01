#ifndef __TA_GENERIC_LIST_H__
#define __TA_GENERIC_LIST_H__

#include <string.h>
#include <stdint.h>

#define SUCCESS 1
#define FAILURE -1

/**
 * @brief Define the virtual function table for the parent with regard to the child
 * @param type1 the parent type
 * @parem type2 the child type
 */
#define DEFINE_HLIST_OPS(type1, type2) \
  struct type1##_ops { \
    int (*create)(struct type2##_st **node, uint8_t *name, uint32_t nlen); \
    int (*add)(struct type1##_st *parent, struct type2##_st *child); \
    struct type2##_st *(*del)(struct type1##_st *parent, struct type2##_st *child); \
    struct type2##_st *(*get)(struct type1##_st *parent, uint8_t *name, uint32_t nlen); \
    void (*print)(struct type1##_st *parent); \
  };

/**
 * @brief Declare the functions for the parent with regard to the child
 * @param type1 the parent type
 * @param type2 the child type
 */
#define DECLARE_FUNC(type1, type2) \
  int type2##_create(struct type2##_st **node, uint8_t *name, uint32_t nlen); \
  int add_##type2##_to_##type1(struct type1##_st *parent, struct type2##_st *child); \
  struct type2##_st *del_##type2##_from_##type1(struct type1##_st *parent, \
      struct type2##_st *child); \
  struct type2##_st *get_##type2##_from_##type1##_by_name(struct type1##_st *parent,\
      uint8_t *name, uint32_t nlen); \
  void print_##type2##_in_##type1(struct type1##_st *head); \
  \
  static struct type1##_ops type1##_ops = { \
    .create = type2##_create, \
    .add = add_##type2##_to_##type1, \
    .del = del_##type2##_from_##type1, \
    .get = get_##type2##_from_##type1##_by_name, \
    .print = print_##type2##_in_##type1, \
  };

#define IMPLEMENT_CREATE_FUNC(type) \
  int type##_create(struct type##_st **node, uint8_t *name, uint32_t nlen) \
  { \
    (*node) = (struct type##_st *)malloc(sizeof(struct type##_st)); \
    if (!(*node)) return FAILURE; \
    (*node)->num = 0; \
    (*node)->name = name; \
    (*node)->nlen = nlen; \
    (*node)->prev = NULL; \
    (*node)->next = NULL; \
    (*node)->head = NULL; \
    return SUCCESS; \
  }

#define IMPLEMENT_ADD_FUNC(type1, type2) \
  int add_##type2##_to_##type1(struct type1##_st *parent, struct type2##_st *child) \
  { \
    struct type2##_st *curr;\
    \
    if (parent->num == 0) \
    { \
      parent->head = child; \
      child->prev = NULL; \
      child->next = NULL; \
    } \
    else \
    { \
      curr = parent->head; \
      while (curr->next) \
        curr = curr->next; \
      curr->next = child; \
      child->prev = curr; \
    } \
    \
    parent->num++; \
    \
    return SUCCESS; \
  };

#define IMPLEMENT_DEL_FUNC(type1, type2) \
  struct type2##_st *del_##type2##_from_##type1(struct type1##_st *parent, \
      struct type2##_st *child) \
  { \
    struct type2##_st *curr; \
    if (parent->num <= 0) \
      return NULL; \
    \
    curr = parent->head; \
    \
    do { \
      if (curr == child) \
        break; \
      curr = curr->next; \
    } while (curr); \
    \
    if (!curr) \
      return NULL; \
    \
    if (curr->prev) \
      curr->prev->next = child->next; \
    \
    if (curr->next) \
      curr->next->prev = child->prev; \
    \
    parent->num--;\
    return curr; \
  }

#define IMPLEMENT_GET_FUNC(type1, type2) \
  struct type2##_st *get_##type2##_from_##type1##_by_name(struct type1##_st *parent,\
      uint8_t *name, uint32_t nlen) \
  { \
    struct type2##_st *curr; \
    \
    if (parent->num == 0) \
      return NULL; \
    \
    curr = parent->head; \
    \
    while (curr) \
    { \
      if (!strncmp(curr->name, name, nlen)) \
        break; \
      curr = curr->next; \
    } \
    \
    return curr; \
  }

#define IMPLEMENT_PRINT_FUNC(type1, type2) \
  void print_##type2##_in_##type1(struct type1##_st *p) \
  { \
    int idx; \
    struct type2##_st *curr; \
    \
    idx = 0; \
    \
    if (p->num <= 0) \
      printf("[%s] Nothing is in the list\n", #type1);\
    else { \
      curr = p->head; \
      while(curr) \
      { \
        printf("[%s] index: %d, num of records: %d, name: %s, nlen: %d\n", #type1, idx++,\
            p->num, curr->name, curr->nlen); \
        curr = curr->next; \
      } \
    } \
  }

#define INIT_LIST_OPS(type1, type2) \
  DEFINE_HLIST_OPS(type1, type2) \
  DECLARE_FUNC(type1, type2) \
  IMPLEMENT_CREATE_FUNC(type2) \
  IMPLEMENT_ADD_FUNC(type1, type2) \
  IMPLEMENT_DEL_FUNC(type1, type2) \
  IMPLEMENT_GET_FUNC(type1, type2) \
  IMPLEMENT_PRINT_FUNC(type1, type2) \

#define INIT_LIST(type, var) \
  struct type##_st *var; \
  var = (struct type##_st *)malloc(sizeof(struct type##_st)); \
  var->name = NULL; \
  var->nlen = 0; \
  var->num = 0; \
  var->head = NULL; \
  BIND_FUNC(type, var)

#define BIND_FUNC(type, var) \
  var->ops = &(type##_ops);

#endif /* __TA_GENERIC_LIST_H__ */
