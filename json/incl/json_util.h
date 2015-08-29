#ifndef __UTIL_H__
#define __UTIL_H__

#include <time.h>
#include <sys/timeb.h>
#ifdef WIN32
#include <Windows.h>
#include <process.h>
#endif
#include "json_object.h"
#include "json_string.h"


#define HAVE_VSNPRINTF 1


#define NEWLINE "\n"

#define LOG( ...) userlog(1,__FILE__,__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGL(l, ...) userlog((l),__FILE__,__FUNCTION__, __LINE__, __VA_ARGS__)

#define VARNAME( var )  (#var)

extern char debugfile[1024];

int userlog(int debuglev, const char *file, const char *function, int line, char *fmt, ...);
//int json_split_path(json_t *jso, char *path);
int trim( char *s);
char *fmtstr( char *fmt, ... );

#endif
