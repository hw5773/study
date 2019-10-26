#ifndef __JSON_INTERNAL_H__
#define __JSON_INTERNAL_H__

#define TRUE 1
#define TRUE_LENGTH 4
#define FALSE 0
#define FALSE_LENGTH 5

#include "json.h"

/* JSON list related functions */

/**
 * @brief initialize a JSON list
 * @return initialized JSON object
 */
struct json_list *init_json_list(void);

/**
 * @brief free a JSON list
 * @param jlst a JSON list to be freed
 */
void free_json_list(struct json_list *jlst);


/* JSON object related functions */

/**
 * @brief initialize a JSON object
 * @return initialized JSON object structure
 */
struct json_object *init_json_object(void);

/**
 * @brief free a JSON object
 * @param jobj a JSON object to be freed
 */
void free_json_object(struct json_object *jobj);

/**
 * @brief add a JSON object to the JSON list
 * @param jlst a JSON list
 * @param jobj a JSON object to be added
 */
void add_json_object_to_json_list(struct json_list *jlst, struct json_object *jobj);


/* Key-value element related functions */

/**
 * @brief initialize the key-value element
 * @return initialized the key-value element
 */
struct kval *init_kval_element(void);

/**
 * @brief make the key-value element
 * @param ptr the pointer of the starting point
 * @param len the length of the string element
 * @return the structure of the key-value element
 */
struct kval *make_kval_element(char *ptr, int len);

/**
 * @brief free the key-value element
 * @param elem the structure to be freed
 */
void free_kval_element(struct kval *elem);

/**
 * @brief print the key-value element
 * @param elem the element to be printed
 */
void print_kval_element(struct kval *elem);


/* JSON object and Key-value element related functions */

/**
 * @brief add the element into the JSON object
 * @param jobj the JSON object
 * @param elem the key-value element to be inserted
 */
void add_kval_element_to_json_object(struct json_object *jobj, struct kval *elem);

/**
 * @brief get the key-value element from the JSON object
 * @param jobj the JSON object
 * @param key the key of the element to be retrieved
 * @param klen the length of the key
 * @return the element found
 */
struct kval *get_kval_element_from_json_object(struct json_object *jobj, 
    char *key, int klen);

/**
 * @brief Check whether the value is boolean-type
 * @return boolean (1) / not boolean (0)
 */
int check_boolean(char *p, char *q, int type);

/**
 * @brief Change the charactor to the integer
 * @return the resultant integer value
 */
int char_to_int(char *p, char *q);

#endif /* __JSON_INTERNAL_H__ */
