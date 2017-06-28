#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "simple_json.h"
#include "simple_json_string.h"
#include "simple_json_error.h"

SJString *sj_string_new()
{
    SJString *string;
    string = (SJString *)malloc(sizeof(SJString));
    if (!string)
    {
        sj_set_error("failed to allocate space for a string");
        return NULL;
    }
    memset(string,0,sizeof(SJString));
    return string;
}

/**
 * @brief make a new string with the characters from array s
 * @param s the character array to populate the string with
 * @return NULL on error or a new string that must be freed with sj_string_free
 */
SJString *sj_string_new_text(char *s)
{
    SJString *string;
    unsigned int l;
    if (!s)
    {
        sj_set_error("sj_string_new_text: no string provided");
        return sj_string_new();
    }
    string = sj_string_new();
    if (!string)return NULL;
    l = strlen(s)+1;
    string->text = (char *)malloc(sizeof(char)*l);
    if (!string->text)
    {
        sj_string_free(string);
        sj_set_error("failed to allocate space for string!");
        return NULL;
    }
    strncpy(string->text,s,l);
    string->size = l;
    return string;
}

void sj_string_free(SJString *string)
{
    if (!string)return;
    if (string->text != NULL)
    {
        free(string->text);
    }
    free(string);
}

int sj_string_cmp(SJString *string,char *s)
{
    if (!string)
    {
        sj_set_error("sj_string_cmp: no string provided");
        return -1;
    }
    if (!s)
    {
        sj_set_error("sj_string_cmp: no character array provided");
        return 1;
    }
    return strncmp(string->text,s,string->size);
}

void sj_string_set(SJString *string,char *s)
{
    unsigned int l;
    if (!string)
    {
        sj_set_error("sj_string_set: no string provided");
        return;
    }
    if (!s)
    {
        sj_set_error("sj_string_set: no character array provided");
        return;
    }
    l = strlen(s);
    if (l >= string->size)
    {
        if (string->text)free(string->text);
        string->text = (char*)malloc(sizeof(char)*l);
        if (!string->text)
        {
            sj_set_error("sj_string_set: failed to allocate space for resized string");
            return;
        }
        string->size = l;
    }
    strncpy(string->text,s,string->size);
}

void sj_string_set_limit(SJString *string,char *s,unsigned long l)
{
    if (!string)
    {
        sj_set_error("sj_string_set: no string provided");
        return;
    }
    if (!s)
    {
        sj_set_error("sj_string_set: no character array provided");
        return;
    }
    if (l >= string->size)
    {
        if (string->text)free(string->text);
        string->text = (char*)malloc(sizeof(char)*l);
        if (!string->text)
        {
            sj_set_error("sj_string_set: failed to allocate space for resized string");
            return;
        }
        string->size = l;
    }
    strncpy(string->text,s,l);
    string->text[l] = '\0';
}

void sj_string_value_free(SJson *json)
{
    if (!json)return;
    sj_string_free(json->v.string);
    free(json);
}

char *sj_string_value_get_string(SJson *json)
{
    if (!json)return NULL;
    return json->v.string->text;
}

SJson *sj_string_to_value(SJString *string)
{
    SJson *json;
    if (!string)return NULL;
    json = sj_new();
    if (!json)return NULL;
    json->v.string = string;
    json->sjtype = SJVT_String;
    json->json_free = sj_string_value_free;
    json->get_string = sj_string_to_json_string;
    return json;
}

char *sj_string_get_text(SJString *string)
{
    if (!string)return NULL;
    return string->text;
}

void sj_string_concat(SJString *string1,SJString *string2)
{
    char *newtext;
    size_t size;
    if ((!string1) || (!string2))return;// no op
    size = string1->size + string2->size;
    if (size <= 0)return;
    newtext=(char *)malloc(sizeof(char)*size);
    if (!newtext)
    {
        sj_set_error("sj_string_concat:failed to allocate space for new string\n");
        return;
    }
    memset(newtext,0,sizeof(char)*size);
    snprintf(newtext,size,"%s%s",string1->text,string2->text);
    if (string1->text)
    {
        free(string1->text);
    }
    string1->text = newtext;
    string1->size = size;
}

void sj_string_append(SJString *string,char *buffer)
{
    char *newtext;
    size_t size;
    if ((!string) || (!buffer))return;// no op
    size = string->size + strlen(buffer);
    if (size <= 0)return;
    newtext=(char *)malloc(sizeof(char)*size);
    if (!newtext)
    {
        sj_set_error("sj_string_append:failed to allocate space for new string\n");
        return;
    }
    memset(newtext,0,sizeof(char)*size);
    snprintf(newtext,size,"%s%s",string->text,buffer);
    if (string->text)
    {
        free(string->text);
    }
    string->text = newtext;
    string->size = size;
}

SJString *sj_string_to_json_string(SJson *string)
{
    SJString *json;
    if (!string)return NULL;
    if (string->sjtype != SJVT_String)
    {
        sj_set_error("sj_string_to_json_string: input string not a string type");
        return NULL;
    }
    json = sj_string_new_text("\"");
    if (!json)return NULL;
    sj_string_concat(json,string->v.string);
    sj_string_append(json,"\"");
    return json;
}

/*eol@eof*/
