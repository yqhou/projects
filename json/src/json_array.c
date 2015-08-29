#include "json_util.h"
#include "json.h"
#include "json_string.h"

json_t *json_new_array( json_t *parent, char *name )
{
    json_t *arr;
    if( !parent || !name ) return NULL;
    arr = json_object_new( json_type_array );
    json_add_array( parent, name, arr );
    return arr;
}
int json_array_getlen( json_t *arr )
{
    int len = 0;
    json_t *ele;
    if( !arr ) return 0;
    ele = arr->child;
    for( ;ele; ele=ele->next ) len++; 
    return len;
}

json_t* json_array_get_item( json_t *arr, int idx )
{
    int len = 0;
    json_t *ele;
    if( !arr ) return NULL;
    if( idx < 0 ) return NULL;
    ele = arr->child;
    for( len=0; ele; ele=ele->next, len++ )
    {
        if( len == idx ) return ele;
    }
    return NULL;
}

json_t *json_array_get( json_t *arr, int idx )
{
    return json_array_get_item( arr, idx );
}
int json_array_insert_item_idx( json_t *arr, json_t *item, int idx )
{
    int i = 0;
    json_t *ele;
    if( !arr || !item) return -1;
    if( item->type != json_type_object && item->type != json_type_array )
        return -2;
    if( idx != 0 )
    {
        if( idx > 0 )
        {
            ele = json_array_get_item( arr, idx-1 );
            if( ! ele )
                ele = arr->last_child;
        }
        else if ( idx < 0 )
            ele = arr->last_child;
        /** insert item after ele**/
        /** ele->next =item, ele->next->prev = item,item->prev=ele,item->next=ele->next*/
        if( ele )
        {
            json_t *temp = ele->next;
            ele->next = item;
            item->prev = ele;
            item->next = temp;
            if( temp )
                temp->prev = item;
            else
                arr->last_child = item;
        }
        else
        {
            arr->child = item;
            arr->last_child = item;
        }
    }
    else
    {
        ele = arr->child;
        arr->child = item;
        item->next = ele;
        if( ele )
            ele->prev = item;
        else
            arr->last_child = item;
    }
    item->depth = arr->depth + 1;
    return 0;
}

int json_array_insert( json_t *arr, json_t *item )
{
    return json_array_insert_item_idx( arr, item, -1 );
}

int json_array_add_string( json_t *arr, int idx, char *key, char *val )
{
    json_t *ele;
    if( !arr ) return -1;
    if( idx < 0 ) return -2;
    if( key == NULL ) return -3;
    ele = json_array_get_item( arr, idx );
    if( ele )
    {
        return json_padd_string( ele, key, val );
    }
    else
    {
        json_t *tmp = json_object_new(json_type_object);
        tmp->depth = arr->depth + 1;
        json_padd_string( tmp, key, val );
        return json_array_insert( arr, tmp );
    }
}
int json_array_add_int( json_t *arr, int idx, char *key, int val )
{
    json_t *ele;
    if( !arr ) return -1;
    if( idx < 0 ) return -2;
    if( key == NULL ) return -3;
    ele = json_array_get_item( arr, idx );
    if( ele )
    {
        return json_add_int( ele, key, val );
    }
    else
    {
        json_t *tmp = json_object_new(json_type_object);
        tmp->depth = arr->depth + 1;
        json_add_int( tmp, key, val );
        return json_array_insert( arr, tmp );
    }
}

int json_array_add_double( json_t *arr, int idx, char *key, double val )
{
    json_t *ele;
    if( !arr ) return -1;
    if( idx < 0 ) return -2;
    if( key == NULL ) return -3;
    ele = json_array_get_item( arr, idx );
    if( ele )
    {
        return json_add_double( ele, key, val );
    }
    else
    {
        json_t *tmp = json_object_new(json_type_object);
        tmp->depth = arr->depth + 1;
        json_add_double( tmp, key, val );
        return json_array_insert( arr, tmp );
    }
}
int json_array_add_long( json_t *arr, int idx, char *key, long val )
{
    json_t *ele;
    if( !arr ) return -1;
    if( idx < 0 ) return -2;
    if( key == NULL ) return -3;
    ele = json_array_get_item( arr, idx );
    if( ele )
    {
        return json_add_long( ele, key, val );
    }
    else
    {
        json_t *tmp = json_object_new(json_type_object);
        tmp->depth = arr->depth + 1;
        json_add_long( tmp, key, val );
        return json_array_insert( arr, tmp );
    }
}
int json_array_add_boolean( json_t *arr, int idx, char *key, int val )
{
    json_t *ele;
    if( !arr ) return -1;
    if( idx < 0 ) return -2;
    if( key == NULL ) return -3;
    ele = json_array_get_item( arr, idx );
    if( ele )
    {
        return json_add_boolean( ele, key, val );
    }
    else
    {
        json_t *tmp = json_object_new(json_type_object);
        tmp->depth = arr->depth + 1;
        json_add_boolean( tmp, key, val );
        return json_array_insert( arr, tmp );
    }
}
int json_array_add_null( json_t *arr, int idx, char *key )
{
    json_t *ele;
    if( !arr ) return -1;
    if( idx < 0 ) return -2;
    if( key == NULL ) return -3;
    ele = json_array_get_item( arr, idx );
    if( ele )
    {
        return json_add_null( ele, key );
    }
    else
    {
        json_t *tmp = json_object_new(json_type_object);
        tmp->depth = arr->depth + 1;
        json_add_null( tmp, key );
        return json_array_insert( arr, tmp );
    }
}

char *json_array_get_string( json_t *arr, int idx, char *key )
{
    json_t *ele;
    if( !arr ) return NULL;
    if( idx < 0 ) return NULL;
    ele = json_array_get_item( arr, idx );
    if( !ele ) return NULL;
    return json_pget_string( ele, key );
}
int json_array_get_int( json_t *arr, int idx, char *key )
{
    json_t *ele;
    if( !arr ) return 0;
    if( idx < 0 ) return 0;
    ele = json_array_get_item( arr, idx );
    if( !ele ) return 0;
    return json_get_int( ele, (char*)key );
}
long json_array_get_long( json_t *arr, int idx, char *key )
{
    json_t *ele;
    if( !arr ) return 0;
    if( idx < 0 ) return 0;
    ele = json_array_get_item( arr, idx );
    if( !ele ) return 0;
    return json_get_long( ele, key );
}
double json_array_get_double( json_t *arr, int idx, char *key )
{
    json_t *ele;
    if( !arr ) return 0;
    if( idx < 0 ) return 0;
    ele = json_array_get_item( arr, idx );
    if( !ele ) return 0;
    return json_get_double( ele, key );
}
int json_array_get_boolean( json_t *arr, int idx, char *key )
{
    json_t *ele;
    if( !arr ) return 0;
    if( idx < 0 ) return 0;
    ele = json_array_get_item( arr, idx );
    if( !ele ) return 0;
    return json_get_boolean( ele, key );
}
