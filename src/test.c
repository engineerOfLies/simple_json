#include "simple_json.h"
#include <stdio.h>

int main(int argc, char *argv[])
{

    SJson *json;
    
    printf("Loading Json file %s\n",argv[1]);
    
    json = sj_load(argv[1]);
    printf("json loaded:\n");
    
    sj_echo(json);
    
    printf("freeing json structure\n");
    sj_free(json);

    printf("complete\n");

    return 0;
}

