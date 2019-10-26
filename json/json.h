#ifndef __JSON_H__
#define __JSON_H__

#define JSON_TYPE_UNKNOWN         1
#define JSON_TYPE_BOOLEAN_TRUE    2
#define JSON_TYPE_BOOLEAN_FALSE   3
#define JSON_TYPE_INTEGER         4
#define JSON_TYPE_STRING          5
#define JSON_TYPE_JSON_LIST       6
#define JSON_TYPE_JSON_OBJECT     7

struct kval
{
  char *key;
  int klen;
  int type;
  char *value;
  int vlen;
  struct kval *next;
};

struct json_object
{
  int num;
  struct json_object *next;
  struct kval *head;
  struct kval *tail;
};

struct json_list
{
  int num;
  struct json_object *head;
  struct json_object *tail;
};


struct json_object *get_json_object_from_str(char *str, int total);

struct json_object *get_json_object_by_key(struct json_object *jobj, char *key);
int get_int_value_by_key(struct json_object *jobj, char *key);

/**
 * @brief Get the string value by a key
 * @param jobj JSON object to be retrieved
 * @param key key for retrieval
 * @param len the pointer of the integer to contain the length of the String value
 * @return the pointer of the string
 */
char *get_str_value_by_key(struct json_object *jobj, char *key, int *len);
int get_bool_value_by_key(struct json_object *jobj, char *key);

struct json_list *get_json_list_by_key(struct json_object *jobj, char *key);
int get_length_of_json_list(struct json_list *lst);
struct json_object *get_json_object_by_index(struct json_list *lst, int idx);

void print_json_object(struct json_object *jobj);
void print_json_list(struct json_list *jlst);

#endif /* __JSON_H__ */
