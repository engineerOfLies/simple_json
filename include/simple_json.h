#ifndef __SIMPLE_JSON_H__
#define __SIMPLE_JSON_H__

#include "simple_json_list.h"
#include "simple_json_string.h"

typedef enum
{
    SJVT_NULL,
    SJVT_Object,
    SJVT_Array,
    SJVT_String
}SJValueTypes;

typedef struct SJson_S
{
    SJValueTypes sjtype;
    union
    {
        SJList *array;  /**<an array or values or an array of pairs*/
        SJString *string;
    }v;
    char *(*get_string)(struct SJson_S *json);
    void  (*json_free)(struct SJson_S *json);
}SJson;

/**
 * @brief loads and parses a json file
 * @param filename the file to parse
 * @return NULL on error or an SJS pointer
 */
SJson *simple_json_load(const char *filename);

/**
 * @brief allocate a new empty json object
 * @return NULL on error or a new json object
 */
SJson *simple_json_new();

/**
 * @brief frees a previously loaded json struct
 */
void simple_json_free(SJson *sjs);
#endif
