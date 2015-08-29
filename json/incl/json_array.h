#ifndef __JSON_ARRAY_H__
#define __JSON_ARRAY_H__
#include "json_object.h"

typedef struct json_s json_array_t;
typedef json_array_t jarray_t;
typedef jarray_t array_t;

array_t *json_new_array( json_t *parent, char *name );
int json_array_getlen( json_t *arr );
json_t* json_array_get_item( json_t *arr, int idx );
json_t *json_array_get( json_t *arr, int idx );
int json_array_insert_item_idx( json_t *arr, json_t *item, int idx );
int json_array_insert( json_t *arr, json_t *item );
int json_array_add_string( json_t *arr, int idx, char *key, char *val );
int json_array_add_int( json_t *arr, int idx, char *key, int val );
int json_array_add_double( json_t *arr, int idx, char *key, double val );
int json_array_add_long( json_t *arr, int idx, char *key, long val );
int json_array_add_boolean( json_t *arr, int idx, char *key, int val );
int json_array_add_null( json_t *arr, int idx, char *key );
char *json_array_get_string( json_t *arr, int idx, char *key );
int json_array_get_int( json_t *arr, int idx, char *key );
long json_array_get_long( json_t *arr, int idx, char *key );
double json_array_get_double( json_t *arr, int idx, char *key );
int json_array_get_boolean( json_t *arr, int idx, char *key );
#endif
