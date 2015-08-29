#include "json.h"
#include "json_string.h"
#include "util.h"


char debugfile[1024];


int remove_suffix(const char *file, char *out)
{
	char *p;
	p = (char*)strrchr(file, '.');
	if (p == NULL)
	{
		memcpy(out, file, strlen(file));
		out[strlen(file)] = 0;
		return 0;
	}
	memcpy(out, file, p - file);
	return 0;
}
int has_path(const char *file)
{
	char *p;
	p = (char*)strchr(file, '/');
	if (p == NULL)
	{
		p = (char*)strchr(file, '\\');
		if (p == NULL)
			return 0;
	}
	return 1;
}
void set_logfile(const char *file)
{
	char *path;
	char _path[1024] = {};
	char tmp[1024] = {};
	int v_haspath = has_path(file);

	if (!v_haspath)
	{
		if ((path = getenv("LOG")) == NULL && (path = getenv("log")) == NULL)
			memcpy(_path, " ", 1);
		else
		{
			memcpy(_path, path, strlen(path));
			if (_path[strlen(path) - 1] != '/' || _path[strlen(path) - 1] != '\\')
			{
				_path[strlen(path)] = '/';
				_path[strlen(path) + 1] = 0;
			}
		}
	}

	memset(debugfile, 0, sizeof(debugfile));
	remove_suffix(file, tmp);
	if (v_haspath)
		sprintf(debugfile, "%s.log", tmp);
	else
		sprintf(debugfile, "%s/%s.log", _path, tmp);
}

#ifdef WIN32
int msgbox(LPSTR title, int option, char *fmt, ...)
{
	va_list arg_ptr;
	char message[4096];

	int  length = 0, fmtlen = 0;
	fmtlen = strlen(fmt);
	length += fmtlen;
	va_start(arg_ptr, fmt);
	vsprintf(message, fmt, arg_ptr);
	va_end(arg_ptr);
	/*
	printlog( "%s, %s\n", message, title );
	*/
	//CopyMemory( lpmsg, message, strlen( message ) );
	MessageBox(NULL, TEXT(message), title, MB_OK);
	return 0;
}
#endif /*WIN32*/

char *GetTime(char *stime) {
	struct tm *p;
	struct timeb tb;
	int hour = 0;
	int min = 0;
	int sec = 0;

	ftime(&tb);
	p = localtime(&(tb.time));

	hour = p->tm_hour;
	min = p->tm_min;
	sec = p->tm_sec;


	sprintf(stime, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
		(1900 + p->tm_year),
		(1 + p->tm_mon),
		p->tm_mday,
		hour,
		min,
		sec,
		tb.millitm);

	return stime;
}


int userlog(int debuglev, const char *file, const char *function, int line, char *fmt, ...)
{

	va_list args;
	FILE    *fp;
	int i = 0;
	char tmp[1024];
	char alTime[25];
	char fname[512];

	if (!debuglev)
		return 0;

	memset(tmp, 0x00, sizeof(tmp));
	memset(fname, 0x00, sizeof(fname));
	memset(alTime, 0x00, sizeof(alTime));

	GetTime(alTime);

	//sprintf( fname, "%s\\%s", path, debugfile );

	if ((fp = fopen(debugfile, "a+")) == NULL) {
		//msgbox( "ERROR", MB_OK, " fopen(%s) failed. %s", debugfile, strerror(errno) );
#ifdef WIN32
		GetModuleFileName(NULL, fname, sizeof(fname)-1);
#else
        memcpy( fname, "json.log", sizeof(fname)-1 );
#endif
		set_logfile(fname);
		if ((fp = fopen(debugfile, "a+")) == NULL)
		{
#ifdef WIN32
			msgbox("ERROR", MB_OK, " fopen(%s) failed. %s", debugfile, strerror(errno));
#endif
			return -1;
		}
	}

	va_start(args, fmt);
#ifdef WIN32
	fprintf(fp, "[%s][%-10s-L%04d-%-15s-%05d] ", alTime + 2, file, line, function, _getpid());
#else
	fprintf(fp, "[%s][%-10s-L%04d-%-15s-%05d] ", alTime + 2, file, line, function, getpid());
#endif
	//vfprintf( fp, "%s->.%d: %s.\n", args );

	vfprintf(fp, fmt, args);
	fprintf(fp, "%s", NEWLINE);
	va_end(args);
	fclose(fp);
	return 0;
}

char *fmtstr( char *fmt, ... )
{
    static char msg[1024];
    va_list va_arg;
    va_start( va_arg, fmt );
    vsprintf( msg, fmt, va_arg );
    va_end( va_arg );
    return msg;
}
