#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "simple_json.h"
#include "simple_json_object.h"
#include "simple_json_string.h"
#include "simple_json_error.h"

typedef struct
{
    char *buffer;
    char *position;
    char *end;
}jsParse;

char *get_next_unescaped_char(char *buffer, char target)
{
    if (!buffer)return NULL;
    while ((buffer[0] != '\0')&&(buffer[1] != '\0'))
    {
        if ((buffer[0] != '\\')&&(buffer[1] == target))
        {
            return ++buffer;
        }
        buffer++;
    }
    return NULL;
}

SJString *sj_parse_string(jsParse *parse)
{
    char *p,*p1;
    size_t str_length;
    SJString *string;
    int quoted = 0;
    if (!parse)return NULL;
    // validate we are an object

    string = sj_string_new();
    if (!string)
    {
        return NULL;
    }
    if (*parse->position == '"')
    {
        //chomp quote character
        quoted = 1;
        parse->position++;
    }
    if (quoted)
    {
        p = get_next_unescaped_char(parse->position, '"');
    }
    else
    {
        p = get_next_unescaped_char(parse->position, ',');
        p1 = get_next_unescaped_char(parse->position, ']');
        p = MIN(p,p1);
        p1 = get_next_unescaped_char(parse->position, '}');
        p = MIN(p,p1);
    }
    if (p == NULL)
    {
        sj_set_error("sj_parse_string: no end delimeter");
        sj_string_free(string);
        return NULL;
    }
    str_length = (p - quoted) - (parse->position);
    if (str_length <= 0)
    {
        sj_set_error("sj_parse_string: string is a zero or negative length");
        sj_string_free(string);
        return NULL;
    }
    if (quoted)parse->position++;
    sj_string_set_limit(string,parse->position,str_length);
    parse->position = p + 1;
    printf("parsed string: %s\n",string->text);
    return string;
}

SJson *sj_parse_value(jsParse *parse)
{
    return NULL;
}


SJson *sj_parse_object(jsParse *parse)
{
    SJson *json = NULL;
    SJson *value = NULL;
    char *p;
    SJString *key;
    if (!parse)return NULL;
    // validate we are an object
    if (*parse->position != '{')
    {
        sj_set_error("sj_parse_object: expected first character to be a {\n");
        return NULL;
    }

    // allocate working space
    json = sj_object_new();
    if (!json)return NULL;
    key = sj_string_new();
    if (!key)
    {
        sj_object_free(json);
        return NULL;
    }
    //chomp first character
    parse->position++;
    
    do
    {
        key = sj_parse_string(parse);
        p = get_next_unescaped_char(parse->position, ':');
        if (p == NULL)
        {
            sj_set_error("sj_parse_object: no colon (:) delimeter for object");
            sj_object_free(json);
            sj_string_free(key);
            return NULL;
        }
        printf("parsed object key: %s\n",key->text);
        parse->position = p+1;
        value = sj_parse_value(parse);
        ++parse->position;
        if (*parse->position == '}')
        {
            break;
        }
    }
    while(*parse->position == ',');
    sj_string_free(key);
    return json;
}

SJson *sj_parse_buffer(char *string,unsigned long length)
{
    SJson *json = NULL;
    jsParse parse;
    if (!string)
    {
        sj_set_error("sj_parse_string: no string provided");
        return NULL;
    }
    parse.buffer = string;
    parse.position = strchr(string, '{');
    parse.end = &string[length -1];
    json = sj_parse_object(&parse);
    return json;
}