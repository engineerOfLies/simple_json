#ifndef __SIMPLE_JSON_OBJECT_H__
#define __SIMPLE_JSON_OBJECT_H__


/**
 * @brief allocate a new empty json object
 * @return NULL on memory allocation error, an empty SJson otherwise
 */
SJson *sj_object_new();

/**
 * @brief free a previously allocated json object
 * @param object the object to free
 */
void sj_object_free(SJson *object);

/**
 * @brief insert data into a json object
 * @param object the json object to insert into
 * @param key the string to identify the object value with
 * @param value the value of the key
 */
void sj_object_insert(SJson *object,char *key,SJson *value);

/**
 * @brief get the json object back as a formatted json string
 * @param object the object to convert
 * @return an allocated character array populated with the json string, or NULL on error
 */
char *sj_object_get_string(SJson *object);

#endif
