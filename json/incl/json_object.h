#ifndef __JSON_OBJECT_H__
#define __JSON_OBJECT_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "json_string.h"
#include "json_util.h"

#define DEFAULT_STRING_SIZE 1024
#define WIN 1



typedef struct json_value_s
{
	json_string_t *_string;
	int _int;
	double _double;
	long _long;
	int  _bool;
} json_value_t;

typedef enum json_type_e
{
	json_type_root = -1,
	json_type_null,
	json_type_boolean,
	json_type_string,
	json_type_int,
	json_type_long,
	json_type_double,
	json_type_object,
	json_type_array
} json_type_e;


typedef struct json_s json_t;
typedef int (json_object_to_string_fn)(json_t *, json_string_t *);

struct json_s
{
	json_string_t *name;
	json_type_e type;
	json_value_t value;
	json_string_t *to_string;
	json_object_to_string_fn *to_string_fn;
	struct json_s *next, *prev, *child, *parent, *last_child;
	int depth;
};

extern json_t *j_errmsg;

void* json_alloc(size_t size);
json_t *json_object_new( json_type_e type );
json_t *json_new();
void json_free_item(json_t *item);
void json_free(json_t **json);
void json_set_name(json_t *json, const char *pair);
int json_object_int_to_string(json_t *json_int, json_string_t *sp );
json_t *json_new_int(const char *pair, int val);
int json_object_long_to_string(json_t *json_long, json_string_t *sp );
json_t *json_new_long(const char *pair, long val);
int json_object_double_to_string(json_t *jso, json_string_t *sp );
json_t *json_new_double(const char *pair, double val);
int json_object_boolean_to_string(json_t *jso, json_string_t *sp );
json_t *json_new_boolean(const char *pair, int val);
int json_object_string_to_string(json_t *jso, json_string_t *sp );
json_t *json_new_string(const char *pair, char *val);
int json_object_null_to_string(json_t *jso, json_string_t *sp);
json_t *json_new_null(const char *pair);
json_t *json_get_last_child(json_t *jso);
json_t* json_search_key(json_t *jso, const char *key);
int json_delete_item(json_t *jso, const char *key );
int json_link_insert_item(json_t *jso, json_t *ele);
int json_object_object_to_string(json_t *jso, json_string_t *sp );
int json_add_object(json_t *jso, const char *key, json_t *ele);
int json_object_array_to_string(json_t *jso, json_string_t *sp );
int json_add_array(json_t *jso, const char *key, json_t *arr);
int json_add_string(json_t *jso, const char *key, char *val);
int json_add_int(json_t *jso, const char *key, int val);
int json_add_long(json_t *jso, const char *key, int val);
int json_add_double(json_t *jso, const char *key, double val);
int json_add_boolean(json_t *jso, const char *key, int val);
int json_add_null(json_t *jso, const char *key);
char *json_get_string(json_t *jso, char *key);
int json_get_int(json_t *jso, char *key);
long json_get_long(json_t *jso, char *key);
double json_get_double(json_t *jso, char *key);
int json_get_boolean(json_t *jso, char *key);
json_t *json_get_object(json_t *jso, char *key);
int json_split_path(json_t *jso, char *path);
int json_padd_string(json_t *jso, char *path, char *val);
char* json_pget_string(json_t *jso, char *path );
char* json_to_string(json_t *jso);

void json_set_errmsg( int rc, char *msg );
char *json_get_errmsg();

#endif

