#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_json_object.h"
#include "simple_json_list.h"
#include "simple_json_string.h"
#include "simple_json_error.h"

extern int __SJ_DEBUG;

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

/**
 * @brief an object is a list of key/value pairs
 */
typedef struct
{
    SJString *key;  /**<the identifying key*/
    SJson *value;   /**<the value the key references*/
}SJPair;

void sj_pair_free(SJPair *pair)
{
    if (!pair)return;
    sj_string_free(pair->key);
    sj_free(pair->value);
    free(pair);
}

SJPair *sj_pair_new(const char *key,SJson *value)
{
    SJPair *pair;
    pair = (SJPair *)malloc(sizeof(SJPair));
    if (!pair)
    {
        sj_set_error("failed to allocate data for new data pair in object");
        return NULL;
    }
    pair->key = sj_string_new_text(key,0);
    if (!pair->key)
    {
        sj_pair_free(pair);
        sj_set_error("failed to set key for object pair");
        return NULL;
    }
    pair->value = value;
    return pair;
}

void sj_object_delete_key(SJson *object,const char *key)
{
    int i,count;
    SJPair *pair;
    if (!sj_object_check(object))return;
    count = sj_list_get_count(object->v.array);
    for (i = 0; i < count; i++)
    {
        pair = sj_list_get_nth(object->v.array,i);
        if (!pair)continue;
        if (sj_string_cmp(pair->key,key) == 0)
        {
            sj_pair_free(pair);
            sj_list_delete_nth(object->v.array,i);
            break;
        }
    }
    return;
}


void sj_object_insert(SJson *object,const char *key,SJson *value)
{
    SJPair *pair;
    if (!sj_object_check(object))return;
    if (!key)return;
    if (!value)return;
    pair = sj_pair_new(key,value);
    if (!pair)return;
    object->v.array = sj_list_append(object->v.array,pair);
}

SJson *sj_object_copy(SJson *json)
{
    SJPair *pair;
    SJson *object;
    int i,count;
    if (!json)return NULL;
    object = sj_object_new();
    if (!object)return NULL;
    count = sj_list_get_count(json->v.array);
    for (i = 0; i < count; i++)
    {
        pair = sj_list_get_nth(json->v.array,i);
        if (!pair)continue;
        sj_object_insert(object,pair->key->text,sj_copy(pair->value));
    }
    return object;
}

SJList *sj_object_get_keys_list(SJson *object)
{
    SJPair *pair;
    int i,c;
    SJList *list;
    if (!object)return NULL;
    if (object->sjtype != SJVT_Object)return NULL;
    list = sj_list_new();
    c = sj_list_get_count(object->v.array);
    for (i = 0; i < c; i++)
    {
        pair = sj_list_get_nth(object->v.array,i);
        if (!pair)continue;
        list = sj_list_append(list,(void *)sj_string_get_text(pair->key));
    }
    return list;
}

SJson *sj_object_new()
{
    SJson *object;
    object = sj_new();
    if (!object)return NULL;
    object->sjtype = SJVT_Object;
    object->v.array = sj_list_new();
    object->json_free = sj_object_free;
    object->get_string = sj_object_to_json_string;
    object->copy = sj_object_copy;
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

SJson *sj_object_get_value(SJson *object,const char *key)
{
    int i,count;
    SJPair *pair;
    if (!sj_object_check(object))return NULL;
    count = sj_list_get_count(object->v.array);
    for (i = 0; i < count; i++)
    {
        pair = sj_list_get_nth(object->v.array,i);
        if (!pair)continue;
        if ((sj_string_cmp(pair->key,key) == 0)&&(strlen(pair->key->text) == strlen(key)))
        {
           return pair->value; 
        }
    }
    return NULL;
}

int sj_object_get_value_as_bool(SJson *object,const char *key,short int *output)
{
    SJson *value;
    value = sj_object_get_value(object,key);
    if (!value)return 0;
    if (value->sjtype != SJVT_String)return 0;
    return sj_string_as_bool(value->v.string,output);
}


int sj_object_get_value_as_int(SJson *object,const char *key,int *output)
{
    SJson *value;
    value = sj_object_get_value(object,key);
    if (!value)return 0;
    if (value->sjtype != SJVT_String)return 0;
    return sj_string_as_integer(value->v.string,output);
}

int sj_object_get_value_as_uint8(SJson *object,const char *key,uint8_t *output)
{
    SJson *value;
    value = sj_object_get_value(object,key);
    if (!value)return 0;
    if (value->sjtype != SJVT_String)return 0;
    return sj_string_as_uint8(value->v.string,output);
}

int sj_object_get_value_as_uint32(SJson *object,const char *key,uint32_t *output)
{
    SJson *value;
    value = sj_object_get_value(object,key);
    if (!value)return 0;
    if (value->sjtype != SJVT_String)return 0;
    return sj_string_as_uint32(value->v.string,output);
}

int sj_object_get_value_as_int32(SJson *object,const char *key,int32_t *output)
{
    SJson *value;
    value = sj_object_get_value(object,key);
    if (!value)return 0;
    if (value->sjtype != SJVT_String)return 0;
    return sj_string_as_int32(value->v.string,output);
}

int sj_object_get_value_as_float(SJson *object,const char *key,float *output)
{
    SJson *value;
    value = sj_object_get_value(object,key);
    if (!value)return 0;
    if (value->sjtype != SJVT_String)return 0;
    return sj_string_as_float(value->v.string,output);
}


const char *sj_object_get_value_as_string(SJson *object,const char *key)
{
    SJson *value;
    value = sj_object_get_value(object,key);
    if (!value)return NULL;
    if (value->sjtype != SJVT_String)return NULL;
    return sj_string_get_text(value->v.string);
}

SJString *sj_object_to_json_string(SJson *object)
{
    SJString *string;
    SJString *valuestring;
    SJPair *pair;
    int i, count;
    string = sj_string_new_text("{",0);
    //for each
    count = sj_list_get_count(object->v.array);
    for (i = 0; i < count; i++)
    {
        pair = sj_list_get_nth(object->v.array,i);
        if (!pair)continue;
        sj_string_append(string,"\"");
        sj_string_concat(string,pair->key);
        sj_string_append(string,"\":");
        valuestring = sj_value_to_json_string(pair->value);
        sj_string_concat(string,valuestring);
        sj_string_free(valuestring);
        if (i +1 < count)sj_string_append(string,",");
    }
    sj_string_append(string,"}");
    return string;
}

/*eol@eof*/
