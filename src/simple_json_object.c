#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_json_object.h"
#include "simple_json_list.h"
#include "simple_json_string.h"
#include "simple_json_error.h"

int sj_object_check(SJson *json)
{
    if (!json)return 0;
    if (json->sjtype != SJVT_Object)
    {
        sj_set_error("sj_object: expected type OBJECT");
        return 0;
    }
    return 1;
}

typedef struct
{
    SJString *key;
    SJson *value;
}SJPair;

void sj_pair_free(SJPair *pair)
{
    if (!pair)return;
    sj_string_free(pair->key);
    sj_free(pair->value);
    free(pair);
}

SJPair *sj_pair_new(char *key,SJson *value)
{
    SJPair *pair;
    pair = (SJPair *)malloc(sizeof(SJPair));
    if (!pair)
    {
        sj_set_error("failed to allocate data for new data pair in object");
        return NULL;
    }
    pair->key = sj_string_new_text(key);
    if (!pair->key)
    {
        sj_pair_free(pair);
        sj_set_error("failed to set key for object pair");
        return NULL;
    }
    pair->value = value;
    return pair;
}

void sj_object_insert(SJson *object,char *key,SJson *value)
{
    SJPair *pair;
    if (!sj_object_check(object))return;
    if (!key)return;
    if (!value)return;
    pair = sj_pair_new(key,value);
    if (!pair)return;
    sj_list_append(object->v.array,pair);
}

SJson *sj_object_new()
{
    SJson *object;
    object = sj_new();
    if (!object)return NULL;
    object->sjtype = SJVT_Object;
    object->json_free = sj_object_free;
    object->get_string = sj_object_get_string;
    return object;
}

void sj_object_free(SJson *object)
{
    int i,count;
    SJPair *pair;
    if (!sj_object_check(object))return;
    count = sj_list_get_count(object->v.array);
    for (i = 0; i < count; i++)
    {
        pair = sj_list_get_nth(object->v.array,i);
        if (!pair)continue;
        sj_pair_free(pair);
    }
    sj_list_delete(object->v.array);
    free(object);
}

SJson *sj_object_get_value(SJson *object,char *key)
{
    int i,count;
    SJPair *pair;
    if (!sj_object_check(object))return NULL;
    count = sj_list_get_count(object->v.array);
    for (i = 0; i < count; i++)
    {
        pair = sj_list_get_nth(object->v.array,i);
        if (!pair)continue;
        if (sj_string_cmp(pair->key,key) == 0)
        {
           return pair->value; 
        }
    }
    return NULL;
}

char *sj_object_get_value_as_string(SJson *object,char *key)
{
    SJson *value;
    value = sj_object_get_value(object,key);
    if (!value)return NULL;
    if (value->sjtype != SJVT_String)return NULL;
    return sj_string_get_text(value->v.string);
}

char *sj_object_get_string(SJson *object)
{
    return NULL;
}

/*eol@eof*/
