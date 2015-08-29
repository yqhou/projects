#include "json.h"

int main()
{
	int i = 0;
    int rc;
	while (i++ < 1)
	{
		json_t *jso = json_new();
		json_add_string(jso, "string1", "string1_val");
		json_add_string(jso, "string2", "string2_val");
		json_add_int(jso, "integer1", 12345);
		printf("%s\n", json_get_string(jso, "string1"));
		printf("%s\n%d\n", json_get_string(jso, "string2"),
			json_get_int(jso, "integer1"));

		json_padd_string(jso, "sub-object/sub-string", "sub-string-val");
		json_padd_string(jso, "object1/sub-object/sub-string", "sub-string-val");
		char *path = "object1/sub-object1/sub-sub-object1//sub-sub-string1";
		json_padd_string(jso, path, "sub-sub-string1-val ");
		printf("sub-sub-string1=[%s]\n", json_pget_string(jso, path));
		json_add_string(jso, "string3", "string3_val");
		json_padd_string(jso, "string4", "string4_val");
//		LOG("\n%s", json_to_string(jso));

        array_t *arr = json_new_array( jso, "array0" );
        rc = json_array_add_string( arr, 0, "array-item", "array-item0-val" );
        rc = json_array_add_int( arr, 0, "array-item-int", 1234 );
        rc = json_array_add_string( arr, 1, "array-item", "array-item1-val" );
        rc = json_array_add_int( arr, 1, "array-item-int", 2234 );
        rc = json_array_add_string( arr, 2, "array-item", "array-item2-val" );
        rc = json_array_add_int( arr, 2, "array-item-int", 3234 );
        /*
        LOG( "rc = [%d][%s][%s][%s][%d]", rc, json_array_get_string( arr, 0, "array-item"),
            json_array_get_string( arr, 1, "array-item" ),
            json_array_get_string( arr, 2, "array-item" ),
            json_array_get_int( arr, 2, "array-item-int") );
            */

        //printf( "====[%d]===[%s]-=-----\n", json_array_getlen(arr),json_array_get_string( arr, 0, "array-item" ) );
        json_t *tmp1 = json_array_get_item( arr, 1 );
        //printf( "-----------\n%s\n--------\n", json_to_string(tmp1) );
		//printf("\n%s\n", json_to_string(jso));
        LOG( "\n%s\n", json_to_string(jso) );
		json_free(&jso);
	}
	//system("pause");
}
