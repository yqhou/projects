#include "json.h"
#include "json_string.h"
#include "json_util.h"

json_t *j_errmsg;

//static int (*to_string_fn[9])(json_t*, json_string_t *) = 
static json_object_to_string_fn *to_string_fn[9] = 
{
    json_object_object_to_string,
    json_object_null_to_string,
    json_object_boolean_to_string,
    json_object_string_to_string,
    json_object_int_to_string,
    json_object_long_to_string,
    json_object_double_to_string,
    json_object_object_to_string,
    json_object_array_to_string
};

void* json_alloc(size_t size)
{
    return malloc(size);
}

json_t *json_object_new( json_type_e type )
{
    json_t *n = (json_t*)malloc(sizeof(json_t));
    memset(n, 0, sizeof(json_t));
    n->type = type;
    n->depth = 0;
    n->last_child = NULL;
    n->to_string_fn = to_string_fn[type+1];
    return n;
}
json_t *json_new()
{
    return json_object_new(json_type_root);
}
void json_free_item(json_t *item)
{
    if (!item) return;
    if (item->name)
        string_free(&item->name);
    if (item->value._string)
        string_free(&(item->value._string));
    if (item->to_string)
        string_free(&(item->to_string));
    free(item);
}
void json_free(json_t **json)
{
    json_t *next;
    if (!(*json)) return;
    //LOG("type:=%d, name=%s", (*json)->type, (*json)->name ? (*json)->name->buf : NULL );
    switch ((*json)->type)
    {
        case json_type_root: case json_type_object: case json_type_array:
            json_free(&(*json)->child);
            //break;
        default:
            next = (*json)->next;
            json_free_item(*json);
            *json = NULL;
            json_free(&next);
            break;
    }
    if( j_errmsg ) json_free( &j_errmsg );
}
void json_set_name(json_t *json, const char *pair)
{
    if (!json->name)
        json->name = string_new(strlen(pair) + 1);
    string_append(json->name, (char*)pair, strlen(pair));
}
int json_object_int_to_string(json_t *json_int, json_string_t *sp )
{
    if (!json_int || !sp ) 
    {
        json_set_errmsg( -1, "para is null" );
        return -1;
    }
    if (json_int->type != json_type_int)
    {
        json_set_errmsg( -2, fmtstr("json type[%d] is not %d",json_int->type, json_type_int) );
        return -2;
    }
    string_append_tab(sp, json_int->depth);
    string_sprintf(sp, "\"%s\" : %d", json_int->name->buf, json_int->value._int);
    return 0;
}
json_t *json_new_int(const char *pair, int val)
{
    json_t *item = json_object_new( json_type_int );
    if (!item)
        return NULL;
    char temp[128];
    //item->type = json_type_int;
    json_set_name(item, pair);

    sprintf(temp, "%d", val);
    if (!item->value._string)
        item->value._string = string_new(strlen(temp) + 1);
    string_append(item->value._string, temp, strlen(temp));

    item->value._int = val;
    item->value._double = (double)val;
    item->value._long = val;
    item->value._bool = val;
    item->to_string_fn = &json_object_int_to_string;
    return item;
}
int json_object_long_to_string(json_t *json_long, json_string_t *sp )
{
    if (!json_long) return -1;
    if (json_long->type != json_type_long)
        return -2;
    if (!sp)
        return -1;
    string_sprintf(sp, "\"%s\" : %ld", json_long->name->buf, json_long->value._long);
    return 0;
}
json_t *json_new_long(const char *pair, long val)
{
    char temp[128];
    json_t *item = json_object_new( json_type_long );
    if (!item)
        return NULL;
    //item->type = json_type_long;
    json_set_name(item, pair);

    sprintf(temp, "%ld", val);
    if (!item->value._string)
        item->value._string = string_new(strlen(temp)+1);
    string_append(item->value._string, temp, strlen(temp));

    item->value._int = val;
    item->value._double = (double)val;
    item->value._long = val;
    item->value._bool = val;
    item->to_string_fn = &json_object_long_to_string;
    return item;
}
int json_object_double_to_string(json_t *jso, json_string_t *sp )
{
    if (!jso) return -1;
    if (jso->type != json_type_double)
        return -2;
    if (!sp)
        return -1;
    string_sprintf(sp, "\"%s\" : %lf", jso->name->buf, jso->value._double);
    return 0;
}
json_t *json_new_double(const char *pair, double val)
{
    char temp[128];
    json_t *item = json_object_new( json_type_double );
    if (!item)
        return NULL;
    //item->type = json_type_double;
    json_set_name(item, pair);

    sprintf(temp, "%lf", val);
    if (!item->value._string)
        item->value._string = string_new(strlen(temp) + 1);
    string_append(item->value._string, temp, strlen(temp));

    item->value._int = (int)val;
    item->value._double = val;
    item->value._long = (long)val;
    item->value._bool = (int)val;
    item->to_string_fn = &json_object_double_to_string;
    return item;
}
int json_object_boolean_to_string(json_t *jso, json_string_t *sp )
{
    if (!jso) return -1;
    if (jso->type != json_type_boolean)
        return -2;
    if (!sp)
        return -1;
    string_sprintf(sp, "\"%s\" : %s", jso->name->buf, jso->value._bool ? "true" : "false");
    return 0;
}
json_t *json_new_boolean(const char *pair, int val)
{
    char temp[128];
    json_t *item = json_object_new( json_type_boolean );
    if (!item)
        return NULL;
    //item->type = json_type_boolean;
    json_set_name(item, pair);

    sprintf(temp, "%s", val ? "true" : "false" );
    if (!item->value._string)
        item->value._string = string_new(strlen(temp) + 1);
    string_append(item->value._string, temp, strlen(temp));

    item->value._int = (int)val;
    item->value._double = (double)val;
    item->value._long = (long)val;
    item->value._bool = (int)val;
    item->to_string_fn = &json_object_boolean_to_string;
    return item;
}
int json_object_string_to_string(json_t *jso, json_string_t *sp )
{
    if (!jso) return -1;
    if (jso->type != json_type_string)
        return -2;
    if (!(sp))
        return -1;
    string_append_tab(sp, jso->depth);
    string_sprintf(sp, "\"%s\" : \"%s\"", jso->name?jso->name->buf:NULL, jso->value._string?jso->value._string->buf : NULL);
    //LOG("xxxx:[%s]=[%s]", jso->name->buf, (sp)->buf);
    return 0;
}
json_t *json_new_string(const char *pair, char *val)
{
    json_t *item = json_object_new( json_type_string );
    if (!item)
        return NULL;
    //item->type = json_type_string;
    json_set_name(item, pair);

    if (!item->value._string)
        item->value._string = string_new(strlen(val) + 1);
    string_sprintf(item->value._string, "%s", val);

    item->value._int = atoi(val);
    item->value._double = atof(val);
    item->value._long = atol(val);
    item->value._bool = atoi(val);
    item->to_string_fn = &json_object_string_to_string;
    return item;
}
int json_object_null_to_string(json_t *jso, json_string_t *sp)
{
    if (!jso) return -1;
    if (jso->type != json_type_null)
        return -2;
    if (!sp)
        return -1;
    string_sprintf(sp, "\"%s\" : \"%s\"", jso->name->buf, jso->value._string);
    return 0;
}
json_t *json_new_null(const char *pair)
{
    json_t *item = json_object_new( json_type_null );
    if (!item)
        return NULL;
    //item->type = json_type_null;
    json_set_name(item, pair);

    if (!item->value._string)
        item->value._string = string_new(6 + 1);
    string_append(item->value._string, "(null)", 6);

    item->value._int = 0;
    item->value._double = 0.0;
    item->value._long = 0;
    item->value._bool = 0;
    item->to_string_fn = &json_object_null_to_string;
    return item;
}
json_t *json_get_last_child(json_t *jso)
{
    if (!jso) return NULL;
    return jso->last_child;
}
json_t* json_search_key(json_t *jso, const char *key)
{
    json_t *item;
    if (!jso) return NULL;
    for ( item=jso->child; item; item = item->next)
    {
        if (!string_cmp(item->name, (char*)key))
            return item;
    }
    return NULL;
}
int json_delete_item(json_t *jso, const char *key )
{
    json_t *item = json_search_key(jso, key);
    if (item)
    {
        json_t *pprev, *ppnext;
        pprev = item->prev, ppnext = item->next;
        if (pprev)
            pprev->next = ppnext;
        if (ppnext)
            ppnext->prev = pprev;
        json_free(&item);
    }
    return 0;
}
int json_link_insert_item(json_t *jso, json_t *ele)
{
    json_t *tail = NULL;
    if (!jso) return -1;
    tail = json_get_last_child(jso);
    //LOG("tail.addr=[%ld]", tail);
    if (tail)
    {
        tail->next = ele;
        ele->prev = tail;
        ele->next = NULL;
        ele->parent = jso;
    }
    else if (!jso->child)
    {
        jso->child = ele;
        ele->parent = jso;
        ele->prev = NULL;
        ele->next = NULL;
    }
    else
        return -2;
    jso->last_child = ele;
    ele->depth = jso->depth + 1;
    return 0;
}
int json_object_object_to_string(json_t *jso, json_string_t *sp )
{
    json_t *j_tmp;
    if (!jso) return -1;
    if (!sp) return -2;
    string_append_tab(sp, jso->depth);
    if (jso->name)
    {
        string_append(sp, "\"", 1);
        string_append(sp, jso->name->buf, jso->name->bpos);
        string_append(sp, "\" : {\n", 6);
    }
    else
        string_append(sp, "{\n", 2);
    //LOG("xxxxx\n[%s]", (sp)->buf);
    j_tmp = jso->child;
    while (j_tmp)
    {
        //LOG("j_tmp.type=%d, j_tmp.name=%s", j_tmp->type, j_tmp->name ? j_tmp->name->buf : NULL);
        //LOG("j_tmp.to_string_fn:%d", j_tmp->to_string_fn);
        if (j_tmp->to_string_fn)
        {
            if (!j_tmp->to_string) j_tmp->to_string = string_new(64);
            j_tmp->to_string_fn(j_tmp, j_tmp->to_string);
        }
        //LOG("j_tmp.to_string[%s]", j_tmp->to_string ? j_tmp->to_string->buf : NULL);
        if ( j_tmp->to_string )
            string_append(sp, j_tmp->to_string->buf, j_tmp->to_string->bpos);
        if (j_tmp->next) string_append(sp, ",\n", 2);
        //LOG("j_tmp.name=%s...[%s]", j_tmp->name ? j_tmp->name->buf : NULL, (*sp)->buf);
        j_tmp = j_tmp->next;
    }
    //LOG("jso->depth:%d....name=%s...[\n%s]", jso->depth, jso->name ? jso->name->buf : NULL, (sp)->buf);
    string_append(sp, "\n", 1);
    string_append_tab(sp, jso->depth);
    //LOG("11111[%s]", (sp)->buf);
    string_append(sp, "}", 1);
    return 0;
}
int json_add_object(json_t *jso, const char *key, json_t *ele)
{
    json_t *item = json_object_new( json_type_object );
    if (!item)
        return -1;
    //item->type = json_type_object;
    json_delete_item(jso, key);

    json_set_name(item, key);
    item->child = ele;
    if (ele)
        ele->parent = item;
    item->depth = jso->depth + 1;
    if (ele)
        ele->depth = item->depth + 1;
    /** item->to_string = xxxx */
    if (json_link_insert_item(jso, item))
    {
        json_free_item(item);
        return -2;
    }
    item->to_string_fn = &json_object_object_to_string;
    return 0;
}
int json_object_array_to_string(json_t *jso, json_string_t *sp )
{
    json_t *tmp;
    //LOG( "into %s", __FUNCTION__ );
    if( ! jso ) return -1;
    if( ! sp ) return -2;
    if( jso->name )
    {
        string_append_tab( sp, jso->depth );
        string_append( sp, "\"", 1 );
        string_append( sp, jso->name->buf, jso->name->bpos );
        string_append( sp, "\" : [\n", 6 );
    }
    /*
       else
       string_append( sp, "[\n", 2 );
     */
    tmp = jso->child;
    //if( tmp )
    //    tmp = tmp->child;
    while( tmp )
    {
        //LOG( "tmp.type=[%d],[%s],func=[%d],", tmp->type, json_get_string(tmp,"array-item"),tmp->to_string_fn );
        if( tmp->to_string_fn )
        {
            if( !tmp->to_string ) tmp->to_string = string_new( 64 );
            tmp->to_string_fn( tmp, tmp->to_string );
        }
        if( tmp->to_string )
        {
            string_append( sp, tmp->to_string->buf, tmp->to_string->bpos );
        }
        if( tmp->next )
            string_append( sp, ",\n", 2 );
        tmp = tmp->next;
    }
    if( jso->name )
    {
        string_append( sp, "\n", 1 );
        string_append_tab( sp, jso->depth );
        string_append( sp, "]", 1 );
    }
    //LOG( "-------[%s]-----------", sp->buf );
    return 0;
}

int json_add_array(json_t *jso, const char *key, json_t *arr)
{
    json_t *item = json_object_new( json_type_array );
    if (!item)
        return -1;
    json_delete_item(jso, key);

    json_set_name(item, key);
    item->child = arr;
    if( arr )
        arr->parent = item;
    item->depth = jso->depth + 1;
    if( arr )
        arr->depth = item->depth + 1;
    if (json_link_insert_item(jso, item))
    {
        json_free_item(item);
        return -2;
    }
    item->to_string_fn = &json_object_array_to_string;
    return 0;
}
int json_add_string(json_t *jso, const char *key, char *val)
{
    json_t *json_str;
    int rc;
    json_delete_item(jso, key);
    if (!(json_str = json_new_string(key, val)))
        return -1;
    //LOG("key=[%s],val=[%s]", json_str->name->buf, json_str->value._string->buf);
    rc = json_link_insert_item(jso, json_str);
    //LOG("rc = %d", rc);
    if ( rc )
    {
        json_free_item(json_str);
        return -2;
    }
    return 0;
}
int json_add_int(json_t *jso, const char *key, int val)
{
    json_t *json_int;
    json_delete_item(jso, key);
    if (!(json_int = json_new_int(key, val)))
        return -1;
    if (json_link_insert_item(jso, json_int))
    {
        json_free_item(json_int);
        return -2;
    }
    return 0;
}
int json_add_long(json_t *jso, const char *key, int val)
{
    json_t *json_long;
    json_delete_item(jso, key);
    if (!(json_long = json_new_long(key, val)))
        return -1;
    if (json_link_insert_item(jso, json_long))
    {
        json_free_item(json_long);
        return -2;
    }
    return 0;
}
int json_add_double(json_t *jso, const char *key, double val)
{
    json_t *json_double;
    json_delete_item(jso, key);
    if (!(json_double = json_new_double(key, val)))
        return -1;
    if (json_link_insert_item(jso, json_double))
    {
        json_free_item(json_double);
        return -2;
    }
    return 0;
}
int json_add_boolean(json_t *jso, const char *key, int val)
{
    json_t *json_boolean;
    json_delete_item(jso, key);
    if (!(json_boolean = json_new_boolean(key, val)))
        return -1;
    if (json_link_insert_item(jso, json_boolean))
    {
        json_free_item(json_boolean);
        return -2;
    }
    return 0;
}
int json_add_null(json_t *jso, const char *key)
{
    json_t *json_null;
    json_delete_item(jso, key);
    if (!(json_null = json_new_null(key)))
        return -1;
    if (json_link_insert_item(jso, json_null))
    {
        json_free_item(json_null);
        return -2;
    }
    return 0;
}


char *json_get_string(json_t *jso, char *key)
{
    json_t *tmp = json_search_key(jso, key);
    //LOG("jso.addr=[%ld],tmp.addr=[%ld],key=[%s],tmp.name=[%s]", jso, tmp,key,tmp->name->buf);
    if (tmp)
    {
        if (tmp->value._string)
            return tmp->value._string->buf;
    }
    return NULL;
}
int json_get_int(json_t *jso, char *key)
{
    json_t *tmp = json_search_key(jso, key);
    if (tmp)
    {
        return tmp->value._int;
    }
    return -9999;
}
long json_get_long(json_t *jso, char *key)
{
    json_t *tmp = json_search_key(jso, key);
    if (tmp)
    {
        return tmp->value._long;
    }
    return -9999;
}
double json_get_double(json_t *jso, char *key)
{
    json_t *tmp = json_search_key(jso, key);
    if (tmp)
    {
        return tmp->value._double;
    }
    return -9999.0;
}
int json_get_boolean(json_t *jso, char *key)
{
    json_t *tmp = json_search_key(jso, key);
    if (tmp)
    {
        return tmp->value._int;
    }
    return -9999.0;
}
json_t *json_get_object(json_t *jso, char *key)
{
    return json_search_key(jso, key);
}
int json_split_path(json_t *jso, char *path)
{
    char *p;
    int sub_cnt = 0;
    json_string_t *sbuf = string_new(64);
    json_string_t *tbuf = string_new(64);
    if (!jso) return -1;
    if (!path) return -2;
    for (p = path; *p; p++)
    {
        if (*p != '/')
        {
            string_append(sbuf, p, 1);
        }
        else if ( sbuf->bpos )
        {
            sub_cnt++;
            string_sprintf(tbuf, "%d", sub_cnt);
            json_add_string(jso, tbuf->buf, sbuf->buf);
            string_reset(tbuf);
            string_reset(sbuf);
        }
    }
    if (sbuf->bpos != 0)
    {
        sub_cnt++;
        string_sprintf(tbuf, "%d", sub_cnt);
        json_add_string(jso, tbuf->buf, sbuf->buf);
        string_reset(tbuf);
        string_reset(sbuf);
    }
    string_free(&tbuf);
    string_free(&sbuf);
    json_add_int(jso, "path_cnt", sub_cnt);
    return 0;
}

int json_padd_string(json_t *jso, char *path, char *val)
{
    int cnt = 0, i;
    json_string_t *sbuf = string_new(64), *tbuf = string_new(64);
    json_t *chd, *j_tmp, *j_path;
    j_path = json_new();
    json_split_path(j_path, path);
    cnt = json_get_int(j_path, "path_cnt");
    if (cnt < 0)
        return cnt;
    j_tmp = jso;
    for (i = 1; i < cnt; i++)
    {
        string_reset(tbuf);
        string_reset(sbuf);
        string_sprintf(tbuf, "%d", i);
        string_sprintf(sbuf, "%s", json_get_string(j_path, tbuf->buf));
        chd = json_search_key(j_tmp, sbuf->buf);
        if (!chd)
        {
            json_add_object(j_tmp, sbuf->buf, NULL);
        }
        j_tmp = json_search_key(j_tmp, sbuf->buf);
    }
    string_reset(tbuf);
    string_reset(sbuf);
    string_sprintf(tbuf, "%d", cnt);
    string_sprintf(sbuf, "%s", json_get_string(j_path, tbuf->buf));
    json_add_string(j_tmp, sbuf->buf, val);
    string_free(&tbuf);
    string_free(&sbuf);
    json_free(&j_path);
    return 0;
}

char* json_pget_string(json_t *jso, char *path )
{
    int cnt = 0, i;
    json_string_t *sbuf = string_new(64), *tbuf = string_new(64);
    json_t *j_tmp, *j_path;
    char *result;
    j_path = json_new();
    json_split_path(j_path, path);
    cnt = json_get_int(j_path, "path_cnt");
    if (cnt < 0)
        return NULL;
    j_tmp = jso;
    for (i = 1; i < cnt; i++)
    {
        string_reset(tbuf);
        string_reset(sbuf);
        string_sprintf(tbuf, "%d", i);
        string_sprintf(sbuf, "%s", json_get_string(j_path, tbuf->buf));
        j_tmp = json_search_key(j_tmp, sbuf->buf);
        if (!j_tmp)
            return NULL;
    }
    string_reset(tbuf);
    string_reset(sbuf);
    string_sprintf(tbuf, "%d", cnt);
    string_sprintf(sbuf, "%s", json_get_string(j_path, tbuf->buf));
    result = json_get_string(j_tmp, sbuf->buf);
    //LOG(" %s depth: %d", sbuf->buf, j_tmp->depth);
    string_free(&tbuf);
    string_free(&sbuf);
    json_free(&j_path);
    return result;
}


char* json_to_string(json_t *jso)
{
    if (!jso) return NULL;
    if (!jso->to_string) jso->to_string = string_new(64);
    json_object_object_to_string(jso, (jso->to_string));
    if ( jso->to_string )
        return jso->to_string->buf;
    else 
        return NULL;
}
