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
SJson *sj_load(const char *filename);

/**
 * @brief allocate a new empty json object
 * @return NULL on error or a new json object
 */
SJson *sj_new();

/**
 * @brief frees a previously loaded json struct
 */
void sj_free(SJson *sjs);

/**
 * @brief convert a json structure into a string
 * @note the returned string needs to be freed
 * @param json the json structure to be converted
 * @return NULL on error or an allocated character array containing the json string
 */
char *sj_to_string(SJson *json);

/**
 * @brief print the contents of the json file to stdout
 * @param json the json struct to print
 */
void sj_echo(SJson *json);
#endif
