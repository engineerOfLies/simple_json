#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_json.h"
#include "simple_json_error.h"


SJson *simple_json_load(const char *filename)
{
    //TODO
    return NULL;
}

SJson *simple_json_new()
{
    SJson *json;
    json = (SJson *)malloc(sizeof(SJson));
    if (!json)
    {
        sj_set_error("simple_json_new: failed to allocate space for new json object");
        return NULL;
    }
    memset(json,0,sizeof(SJson));
    return json;
}

void simple_json_free(SJson *json)
{
    if (!json)return;
    
    if (json->json_free)json->json_free(json);
    
    free(json);
}















































































/*eol@eof*/
