#include "json.h"
#include <errno.h>

#define BEFORE_QUOTE 1
#define AFTER_QUOTE_BEFORE_NAME 2
#define IN_NAME 3
#define AFTER_NAME_BEFORE_COLON 4
#define AFTER_COLON_BEFORE_QUOTE 5 /* : */
#define AFTER_QUOTE_BEFORE_VAL 6
#define IN_VAL 7
#define AFTER_QUOTE_BEFORE_COMMA 8
#define NEW_OBJECT 0
#define 

int set_flag( int flag1, char c ) /** " : , **/
{
    int flag;
    switch( flag1 )
    {
        case NEW_OBJECT: case BEFORE_QUOTE:
            flag = AFTER_QUOTE_BEFORE_NAME;
            if( c != '\"' )
                flag = -1;
            break;
        case AFTER_QUOTE_BEFORE_NAME: /** name is null **/
            break;
        case IN_NAME: /** name is end */
            flag = AFTER_NAME_BEFORE_COLON;
            break;
        case AFTER_NAME_BEFORE_COLON: /*** "" ***/
            return -5;
        case AFTER_COLON_BEFORE_QUOTE:
            break;
        case AFTER_QUOTE_BEFORE_VAL:
            break;
        case IN_VAL:
            break;
        case AFTER-QUOTE_BEFORE_COMMA:
            break;
        default:
    }
    return flag;
}

int json_load_from_buffer( json_t *jso, char *buf )
{
    char ch;
    int flag = NEW_OBJECT; /** 0-out,**/
    int escape_flag = 0;
    int cur = 0;
    if( !jso ) 
    {
        json_set_errmsg( -9999, "parameter jso is null" );
        return -9999;
    }
    ch = *buf;
    cur ++;
    if( ch != '{' )
    {
        json_set_errmsg( -2, fmtstr("file format error,position %d", cur));
        return -2;
    }
    json_string_t *str_name, *str_val;
    while( (ch=*(buf++)) != 0  )
    {
        cur++;
        switch( ch )
        {
            case '\"':
                {
                }
                escape_flag = 0;
                break;
            case ':':
                escape_flag = 0;
                break;
            case '{':
                {
                    escape_flag = 0;
                    flag = NEW_OBJECT;
                    if( !str_name )
                    {
                        json_set_errmsg( -3, fmtstr("format error, position %d", cur) );
                        return -3;
                    }
                    json_t *j = json_new_object( json_type_object );
                    int n = json_load_from_buffer( j, buf-1 );
                    cur += (n-1); 
                    json_add_object( jso, str_name->buf, j );
                }
                break;
            case '}':
                escape_flag = 0;
                break;
            case '\n': case '\r':
                escape_flag = 0;
                break;
            case '[':
                escape_flag = 0;
                break;
            case ']':
                escape_flag = 0;
                break;
            case '\\':
                escape_flag = 1;
                break;
            case ',':
                break;
            default:
                escape_flag = 0;
                break;
        }
    }
    return cur;
}
