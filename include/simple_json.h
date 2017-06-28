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
    SJString *(*get_string)(struct SJson_S *json);
    void  (*json_free)(struct SJson_S *json);
}SJson;

/**
 * @brief loads and parses a json file
 * @param filename the file to parse
 * @return NULL on error or an SJS pointer
 */
SJson *sj_load(const char *filename);

/**
 * @brief write a json value as a formatted json string to file
 * @param json the struct to convert and write
 * @param filename the file to overwrite
 */
void sj_save(SJson *json,char *filename);

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

/**
 * @brief make a jason value object out of a string
 * @param string the string to convert
 * @note the original string is put into the SJson object and is no longer owned by you do not free it!
 * @return NULL on error or the newly allocated and set SJson object
 */
SJson *sj_string_to_value(SJString *string);

/**
 * @brief make a new json value that is NULL
 * @return NULL on error (Ironically) or a new json that holds a NULL value
 */
SJson *sj_null_new();

/**
 * @brief get the contents of the string back formatted and escaped for json
 * @param string the json string to conver
 * @return NULL on error or the converted string
 */
SJString *sj_string_to_json_string(SJson *string);

/**
 * @brief convert the json value into a json string
 * @param json the value to convert
 * @return NULL on error or the json string
 */
SJString *sj_value_to_json_string(SJson *json);

#endif
