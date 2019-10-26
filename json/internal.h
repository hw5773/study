#ifndef __INTERNAL_H__
#define __INTERNAL_H__

struct json_object *init_json_object(void);
struct kval *init_key_value_st(void);

struct kval *make_element
struct json_object *add_element_to_object(struct json_object *jobj, struct kval *e);


#endif /* __INTERNAL_H__ */
