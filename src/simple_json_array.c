#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_json_list.h"
#include "simple_json_array.h"
#include "simple_json_error.h"

int sj_array_check(SJson *json)
{
    if (!json)return 0;
    if (json->sjtype != SJVT_Array)
    {
        sj_set_error("sj_array: expected type ARRAY");
        return 0;
    }
    return 1;
}

SJson *sj_array_new()
{
    SJson *array;
    array = sj_new();
    if (!array)return NULL;
    array->sjtype = SJVT_Array;
    array->json_free = sj_array_free;
    array->get_string = sj_array_as_string;
    array->v.array = sj_list_new();
    return array;
}

void sj_array_free(SJson *array)
{
    int i,count;
    SJson *item;
    if (!sj_array_check(array))return;
    count = sj_list_get_count(array->v.array);
    for (i = 0; i < count; i++)
    {
        item = (SJson *)sj_list_get_nth(array->v.array,i);
        if (!item)continue;
        if (item->json_free)item->json_free(item);
    }
    sj_list_delete(array->v.array);
}

void sj_array_append(SJson *array,SJson *value)
{
    if (!sj_array_check(array))return;
    if (!value)return;
    sj_list_append(array->v.array,value);
}

SJson *sj_array_get_nth(SJson *array,int n)
{
    if (!sj_array_check(array))return NULL;
    return (SJson *)sj_list_get_nth(array->v.array,n);
}

SJString *sj_array_get_nth_as_string(SJson *array,int n)
{
    SJson *item;
    if (!sj_array_check(array))return NULL;
    item = (SJson *)sj_list_get_nth(array->v.array,n);
    if (!item)return NULL;
    if (item->sjtype != SJVT_String)return NULL;
    return item->v.string;
}

char *sj_array_as_string(SJson *array)
{
    if (!sj_array_check(array))return NULL;
    return NULL;
}

/*eol@eof*/
