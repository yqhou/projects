#include "json.h"
#include "json_string.h"

json_string_t *string_new(size_t size)
{
	json_string_t *str = (json_string_t*)malloc(sizeof(json_string_t));
	if (str)
	{
		str->size = size;
		str->bpos = 0;
		str->buf = (char*)malloc(size);
		if (!str->buf)
		{
			free(str);
			return NULL;
		}
		memset(str->buf, 0, size);
	}
	return str;
}
void string_free(json_string_t **s)
{
	if (!(*s))
		return;
	free((*s)->buf);
	(*s)->size = 0;
	(*s)->bpos = 0;
	free(*s);
	*s = NULL;
}
int _string_append(json_string_t *s, char *buf, int size, char *para_name)
{
	char *t;
	//LOG("%s__%s:src_buf=[[%s]], append=[%s],size=%d",__FUNCTION__, para_name, s->buf, buf, size);
	if (s->size - s->bpos <= size )
	{
		int new_size = s->size * 2 >(s->bpos + size + 8) ? s->size * 2 : (s->bpos + size + 8);
		//LOG("s->size=[%d],s->bpos=[%d],size=[%d],new_size=[%d]", s->size, s->bpos, size, new_size);
		t = (char*)realloc(s->buf, new_size);
		if (!t)
			return -1;
		s->buf = t;
		s->size = new_size;
	}
	memcpy(s->buf + s->bpos, buf, size);
	s->bpos += size;
	s->buf[s->bpos] = 0;
	//LOG("%s__%s::[%s]", __FUNCTION__, para_name,s->buf);
	return size;
}
int string_cmp(json_string_t *s, char *s1)
{
	return strcmp(s->buf, s1);
}
int string_ncmp(json_string_t *s, char *s1, size_t n)
{
	return strncmp(s->buf, s1, n);
}
int string_ncasecmp(json_string_t *s, char *s1, size_t n)
{
#ifdef WIN32
	return _strnicmp(s->buf, s1, n);
#else
	return strncasecmp(s->buf, s1, n);
#endif
}
int string_casecmp(json_string_t *s, char *s1)
{
#ifdef WIN32
	return _stricmp(s->buf, s1);
#else
	return strcasecmp(s->buf, s1);
#endif
}


#if !HAVE_VSNPRINTF && defined(WIN32)
# define vsnprintf _vsnprintf
#elif !HAVE_VSNPRINTF /* !HAVE_VSNPRINTF */
# error Need vsnprintf!
#endif /* !HAVE_VSNPRINTF && defined(WIN32) */

#if !HAVE_VASPRINTF
/* CAW: compliant version of vasprintf */
static int vasprintf(char **buf, const char *fmt, va_list ap)
{
#ifndef WIN32
	static char _T_emptybuffer = '\0';
#endif /* !defined(WIN32) */
	int chars;
	char *b;

	if (!buf) { return -1; }

#ifdef WIN32
	chars = _vscprintf(fmt, ap) + 1;
#else /* !defined(WIN32) */
	/* CAW: RAWR! We have to hope to god here that vsnprintf doesn't overwrite
	our buffer like on some 64bit sun systems.... but hey, its time to move on */
	chars = vsnprintf(&_T_emptybuffer, 0, fmt, ap) + 1;
	if (chars < 0) { chars *= -1; } /* CAW: old glibc versions have this problem */
#endif /* defined(WIN32) */

	b = (char*)malloc(sizeof(char)*chars);
	if (!b) { return -1; }

	if ((chars = vsprintf(b, fmt, ap)) < 0)
	{
		free(b);
	}
	else {
		*buf = b;
	}

	return chars;
}
#endif /* !HAVE_VASPRINTF */

int string_sprintf(json_string_t *s, char *fmt, ...)
{
	va_list ap;
	char *t;
	int size;
	char buf[128];

	/* user stack buffer first */
	va_start(ap, fmt);
	size = vsnprintf(buf, 128, fmt, ap);
	va_end(ap);
	/* if string is greater than stack buffer, then use dynamic string
	with vasprintf.  Note: some implementation of vsnprintf return -1
	if output is truncated whereas some return the number of bytes that
	would have been written - this code handles both cases. */
	if (size == -1 || size > 127) {
		va_start(ap, fmt);
		if ((size = vasprintf(&t, fmt, ap)) == -1) { va_end(ap); return -1; }
		va_end(ap);
		string_append(s, t, size);
		free(t);
		return size;
	}
	else {
		string_append(s, buf, size);
		return size;
	}
}

int string_reset(json_string_t *s)
{
	s->bpos = 0;
	memset(s->buf, 0, s->size);
	return 0;
}


int string_append_tab(json_string_t *s, int n)
{
	int i;
    if (!s) return -1;
    for (i = 0; i < n; i++)
    {
        //string_append(s, "\t", 1);
        string_append( s, "    ", 4 );
    }
    return 0;
}
