#include "json.h"

void json_set_errmsg( int rc, char *msg )
{
    if( !j_errmsg ) j_errmsg = json_new();
    json_add_int( j_errmsg, "rc", rc );
    json_add_string( j_errmsg, "msg", msg );
}

char *json_get_errmsg()
{
    return json_get_string( j_errmsg, "msg" );
}
