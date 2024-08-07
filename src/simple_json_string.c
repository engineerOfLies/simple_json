#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_json_string.h"
#include "simple_json_error.h"

#if defined(WIN32)
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif

extern int __SJ_DEBUG;


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
 * @param numeric if the value is a number or bool, set this to true
 * @return NULL on error or a new string that must be freed with sj_string_free
 */
SJString *sj_string_new_text(const char *s,int numeric)
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
    if (string->text == NULL)
    {
        sj_string_free(string);
        sj_set_error("failed to allocate space for string!");
        return NULL;
    }
    memset(string->text,0,sizeof(char)*l);
    strncpy(string->text,s,l);
    string->length = strlen(s);
    string->size = l;
    string->numeric = numeric;
    return string;
}

SJString *sj_string_new_integer(int i)
{
    static char buffer[128];
    sprintf(buffer,"%i",i);
    return sj_string_new_text(buffer,1);
}

SJString *sj_string_new_uint8(uint8_t i)
{
    static char buffer[128];
    sprintf(buffer,"%u",i);
    return sj_string_new_text(buffer,1);
}

SJString *sj_string_new_uint32(uint32_t i)
{
    static char buffer[128];
    sprintf(buffer,"%u",i);
    return sj_string_new_text(buffer,1);
}

SJString *sj_string_new_int32(int32_t i)
{
    static char buffer[128];
    sprintf(buffer,"%i",i);
    return sj_string_new_text(buffer,1);
}


SJString *sj_string_new_float(float f)
{
    static char buffer[128];
    if (f == 0)f = 0;//to prevent negative zero from being written
    sprintf(buffer,"%f",f);
    return sj_string_new_text(buffer,1);
}

SJString *sj_string_new_bool(int i)
{
    if (i)return sj_string_new_text("true",1);
    return sj_string_new_text("false",1);
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

int sj_string_cmp(SJString *string,const char *s)
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
    if (strlen(string->text) != strlen(s))return -1;
    return strncmp(string->text,s,string->length);
}

void sj_string_set(SJString *string,const char *s)
{
    int l;
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
    l = strlen(s)+1;
    if (l >= string->size)
    {
        if (string->text)free(string->text);
        string->text = (char*)malloc(sizeof(char)*l);
        if (!string->text)
        {
            sj_set_error("sj_string_set: failed to allocate space for resized string");
            return;
        }
        memset(string->text,0,sizeof(char)*l);
        string->size = l;
        string->length = strlen(s);
    }
    strncpy(string->text,s,string->length);
}

void sj_string_set_limit(SJString *string,const char *s,long l)
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
    if (l+1 >= string->size)
    {
        if (string->text != NULL)free(string->text);
        string->text = (char*)malloc(sizeof(char)*(l+1));
        if (!string->text)
        {
            sj_set_error("sj_string_set: failed to allocate space for resized string");
            return;
        }
        memset(string->text,0,sizeof(char)*(l+1));
        string->length = strlen(s);
        string->size = (l+1);
    }
    strncpy(string->text,s,l);
}

void sj_string_value_free(SJson *json)
{
    if (!json)return;
    sj_string_free(json->v.string);
    free(json);
}

const char *sj_string_value_get_string(SJson *json)
{
    if (!json)return NULL;
    return json->v.string->text;
}

SJson *sj_string_copy(SJson *json)
{
    SJson *copy;
    if (!json)return NULL;
    if (json->sjtype != SJVT_String)return NULL;
    copy = sj_new_str(json->v.string->text);
    if (!copy)return NULL;
    copy->v.string->numeric = json->v.string->numeric;
    return copy;
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
    json->copy = sj_string_copy;
    return json;
}

const char *sj_string_get_text(SJString *string)
{
    if (!string)return NULL;
    return (const char *)string->text;
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

void sj_string_append(SJString *string,const char *buffer)
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
    //need to skip if numeric
    json = sj_string_new_text("",0);
    if (!string->v.string->numeric)sj_string_append(json,"\"");
    if (!json)return NULL;
    sj_string_concat(json,string->v.string);
    if (!string->v.string->numeric)sj_string_append(json,"\"");
    return json;

}

int sj_string_as_uint32(SJString *string,uint32_t *output)
{
    uint32_t value;
    if (!string)return 0;
    if (!string->text)return 0;
    value = strtoul(string->text,NULL,10);
    if (!value)// if we have a zero, make sure the string itself is not just zero
    {
        if (string->text[0] != '0')return 0;
    }
    if (output)
    {
        *output = (uint32_t)value;
    }
    return 1;
}

int sj_string_as_int32(SJString *string,int32_t *output)
{
    int32_t value;
    if (!string)return 0;
    if (!string->text)return 0;
    value = atol(string->text);
    if (!value)// if we have a zero, make sure the string itself is not just zero
    {
        if (string->text[0] != '0')return 0;
    }
    if (output)
    {
        *output = (int32_t)value;
    }
    return 1;
}


int sj_string_as_uint8(SJString *string,uint8_t *output)
{
    long value;
    if (!string)return 0;
    if (!string->text)return 0;
    value = atol(string->text);
    if (!value)// if we have a zero, make sure the string itself is not just zero
    {
        if (string->text[0] != '0')return 0;
    }
    if (output)
    {
        *output = (uint8_t)value;
    }
    return 1;
}

int sj_string_as_integer(SJString *string,int *output)
{
    int value;
    if (!string)return 0;
    if (!string->text)return 0;
    value = atoi(string->text);
    if (!value)// if we have a zero, make sure the string itself is not just zero
    {
        if (string->text[0] != '0')return 0;
    }
    if (output)
    {
        *output = value;
    }
    return 1;
}

int sj_string_as_double(SJString *string,double *output)
{
    const char *str = NULL;
    int negative = 0;
    double value;
    if (!string)return 0;
    if (!string->text)return 0;
    str = string->text;
    if (str[0] == '-')
    {
        negative = 1;
        str++;//skip it
    }
    value = atof(str);
    if (value == 0.0)// if we have a zero, make sure the string itself is not just zero
    {
        if (str[0] != '0')return 0;
    }
    if (output)
    {
        if ((value)&&(negative))value *= -1.0;
        *output = value;
    }
    return 1;
}

int sj_string_as_float(SJString *string,float *output)
{
    const char *str = NULL;
    int negative = 0;
    double value;
    if (!string)return 0;
    if (!string->text)return 0;
    str = string->text;
    if (str[0] == '-')
    {
        negative = 1;
        str++;//skip it
    }
    value = atof(str);
    if (value == 0.0)// if we have a zero, make sure the string itself is not just zero
    {
        if (str[0] != '0')return 0;
    }
    if (output)
    {
        if ((value)&&(negative))value *= -1.0;
        *output = (float)value;
    }
    return 1;
}

int sj_string_as_bool(SJString *string,short int *output)
{
    if (!string)return 0;
    if (!string->text)return 0;
    if ((strcmp(string->text,"true")==0) ||
        (strcmp(string->text,"TRUE")==0) ||
        (strcmp(string->text,"True")==0) ||
        (strcmp(string->text,"1\0")==0))
    {
        if (output)*output = 1;
        return 1;
    }
    if ((strcmp(string->text,"false")==0) ||
        (strcmp(string->text,"FALSE")==0) ||
        (strcmp(string->text,"False")==0) ||
        (strcmp(string->text,"0\0")==0))
    {
        if (output)*output = 0;
        return 1;
    }
    return 0;
}

/*eol@eof*/
