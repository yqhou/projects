#ifndef __JSON_STRING_H__
#define __JSON_STRING_H__

#include "json_object.h"
#include "json_util.h"

typedef struct string_s
{
	char *buf;
	int size;
	int bpos;
}json_string_t;

#define string_append( s, b, i ) _string_append(s, b, i, VARNAME(s));

json_string_t *string_new(size_t size);
void string_free(json_string_t **s);
int _string_append(json_string_t *s, char *buf, int size, char *para_name);
int string_cmp(json_string_t *s, char *s1);
int string_ncmp(json_string_t *s, char *s1, size_t n);
int string_ncasecmp(json_string_t *s, char *s1, size_t n);
int string_casecmp(json_string_t *s, char *s1);
int string_sprintf(json_string_t *s, char *fmt, ...);
int string_reset(json_string_t *s);
int string_append_tab(json_string_t *s, int n);

#endif
