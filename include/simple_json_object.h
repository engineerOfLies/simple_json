#ifndef __SIMPLE_JSON_OBJECT_H__
#define __SIMPLE_JSON_OBJECT_H__

#include "simple_json_value.h"

/**
 * @brief free a previously allocated json object
 * @param object the object to free
 */
void sj_object_free(SJson *object);

void sj_object_insert(SJson *object,const char *key,SJson *value);

/**
 * @brief get the json object back as a formatted json string
 * @param object the object to convert
 * @return NULL on error, or a string encapsulated ith {}
 */
SJString *sj_object_to_json_string(SJson *object, int pretty);

/**
 * @brief appends spaces for get_string(), nothing if 0
 * @param object the string to append to
 * @param int amount of tabs to append offset by 1
 */
void sj_pretty_append_spaces(SJString *object, int pretty);
#endif
