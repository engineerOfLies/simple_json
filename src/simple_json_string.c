#include <string.h>
#include <stdlib.h>
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

char *sj_string_get_text(SJString *string)
{
    if (!string)return NULL;
    return string->text;
}

/*eol@eof*/
