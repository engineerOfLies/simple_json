#include "simple_json_error.h"
#include "simple_json_string.h"

static SJString error = {0};

char *sj_get_error()
{
    return error.text;
}

void sj_set_error(char *er)
{
    sj_string_set(&error,er);
}

/*eol@eof*/