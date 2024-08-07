#ifndef __SIMPLE_JSON_H__
#define __SIMPLE_JSON_H__

#include "simple_json_list.h"
#include "simple_json_string.h"
#include "simple_json_object.h"
#include "simple_json_error.h"

/**
 * @brief frees a previously loaded json struct
 */
void sj_free(SJson *sjs);

/**
 * @brief make a duplicate of a json structure.
 * @note the duplicate needs to be sj_free()'d separately
 * @param json the json to be duplicated
 * @return NULL on error, or a Duplicate json
 */
SJson *sj_copy(SJson *json);

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
void sj_save(SJson *json,const char *filename);

/**
 * @brief make a new json value that is a string
 * @param str the string to base the json value on
 * @return NULL on error or a new json that holds a string value
 */
SJson *sj_new_str(const char *str);

/**
 * @brief get the JSON value as a string
 * @param json the json value to get the string from
 * @return NULL if the provided json was not a string or on error, the string otherwise
 * @note: the returned value should not be altered or freed
 */
const char *sj_get_string_value(SJson *json);

/**
 * @brief get the json value as an integer
 * Can be used to check for existence
 * @param json the json value to get from
 * @param i [optional] place to write the output to
 * @return 0 if the json did not contain an integer or if there was an error, 1 otherwise
 */
int sj_get_integer_value(SJson *json,int *i);

/**
 * @brief get the json value as an unsigned 8bit integer
 * Can be used to check for existence
 * @param json the json value to get from
 * @param i [optional] place to write the output to
 * @return 0 if the json did not contain an integer or if there was an error, 1 otherwise
 */
int sj_get_uint8_value(SJson *json,uint8_t *i);

/**
 * @brief get the json value as an signed 32bit integer
 * Can be used to check for existence
 * @param json the json value to get from
 * @param i [optional] place to write the output to
 * @return 0 if the json did not contain an integer or if there was an error, 1 otherwise
 */
int sj_get_int32_value(SJson *json,int32_t *i);

/**
 * @brief get the json value as an unsigned 32bit integer
 * Can be used to check for existence
 * @param json the json value to get from
 * @param i [optional] place to write the output to
 * @return 0 if the json did not contain an integer or if there was an error, 1 otherwise
 */
int sj_get_uint32_value(SJson *json,uint32_t *i);

/**
 * @brief get the json value as a float
 * Can be used to check for existence
 * @param json the json value to get from
 * @param f [optional] place to write the output to
 * @return 0 if the json did not contain a float or if there was an error, 1 otherwise
 */
int sj_get_float_value(SJson *json,float *f);

/**
 * @brief get the json value as a bool
 * Can be used to check for existence
 * @param json the json value to get from
 * @param b [optional] place to write the output to
 * @return 0 if the json did not contain a bool or if there was an error, 1 otherwise
 */
int sj_get_bool_value(SJson *json,short int *b);

/**
 * @brief make a new json value that is an integer
 * @param i the integer value
 * @return NULL on error or a new json that holds an integer value
 */
SJson *sj_new_int(int i);

/**
 * @brief make a new json value that is an unsigned 8bit integer
 * @param i the integer value
 * @return NULL on error or a new json that holds an integer value
 */
SJson *sj_new_uint8(uint8_t i);

/**
 * @brief make a new json value that is an unsigned 32bit integer
 * @param i the integer value
 * @return NULL on error or a new json that holds an integer value
 */
SJson *sj_new_uint32(uint32_t i);

/**
 * @brief make a new json value that is a signed 32bit integer
 * @param i the integer value
 * @return NULL on error or a new json that holds an integer value
 */
SJson *sj_new_int32(int32_t i);


/**
 * @brief make a new json value that is a float
 * @param f the float value
 * @return NULL on error or a new json that holds a floating point value
 */
SJson *sj_new_float(float f);

/**
 * @brief make a new json value that is a bool
 * @param b the boolean value (1 is true, 0 is false)
 * @return NULL on error or a new json that holds a bool value
 */
SJson *sj_new_bool(int b);

/**
 * @brief make a new json value that is NULL
 * @return NULL on error (Ironically) or a new json that holds a NULL value
 */
SJson *sj_null_new();

/**
 * @brief allocate a new empty json object
 * @return NULL on memory allocation error, an empty SJson object otherwise
 */
SJson *sj_object_new();

/**
 * @brief insert data into a json object
 * @param object the json object to insert into
 * @param key the string to identify the object value with
 * @param value the value of the key
 * @note: this is a no-op if object is not an actual JSON object
 */
void sj_object_insert(SJson *object,const char *key,SJson *value);

/**
 * @brief delete a value out of the object.
 * @note this will recursively free the value and its children
 * @param object the object to delete from
 * @param key the key to be deleted
 */
void sj_object_delete_key(SJson *object,const char *key);

/**
 * @brief get a list of the keys from the object
 * @param object the object to get keys from
 * @return NULL if not an object or an error, a list of const char * that must be freed otherwise
 * @note DO NOT edit the keys, do NOT free the keys.  ONLY free the list with sj_list_delete() when you are done
 */
SJList *sj_object_get_keys_list(SJson *object);

/**
 * @brief get the json value from an object given a key
 * @param object the json object to get
 * @param key the key to search by
 * @return NULL on error, or if no object provided or it is not of object or the key is not found, the value otherwise
 * @note: the json returned is OWNED by the parent object do not free it.
 */
SJson *sj_object_get_value(SJson *object,const char *key);

/**
 * @brief get the json value from an object given a key as a string
 * @param object the json object to get
 * @param key the key to search by
 * @return NULL on error, or the string
 * @note: the string returned is OWNED by the parent object do not free it.
 */
const char *sj_object_get_value_as_string(SJson *object,const char *key);
const char *sj_object_get_string(SJson *object,const char *key);

/**
 * @brief get the json value from an object given a key as a bool
 * @param object the json object to get
 * @param key the key to search by
 * @param output where the value is written to
 * @return 0 if not found or not an integer, 1 if all good
 */
int sj_object_get_value_as_bool(SJson *object,const char *key,short int *output);
int sj_object_get_bool(SJson *object,const char *key,short int *output);

/**
 * @brief get the json value from an object given a key as an integer
 * @param object the json object to get
 * @param key the key to search by
 * @param output where the value is written to
 * @return 0 if not found or not an integer, 1 if all good
 */
int sj_object_get_value_as_int(SJson *object,const char *key,int *output);
int sj_object_get_int(SJson *object,const char *key,int *output);

/**
 * @brief get the json value from an object given a key as an unsigned 8bit integer
 * @param object the json object to get
 * @param key the key to search by
 * @param output where the value is written to
 * @return 0 if not found or not an integer, 1 if all good
 */
int sj_object_get_value_as_uint8(SJson *object,const char *key,uint8_t *output);
int sj_object_get_uint8(SJson *object,const char *key,uint8_t *output);

/**
 * @brief get the json value from an object given a key as an unsigned 32bit integer
 * @param object the json object to get
 * @param key the key to search by
 * @param output where the value is written to
 * @return 0 if not found or not an integer, 1 if all good
 */
int sj_object_get_value_as_uint32(SJson *object,const char *key,uint32_t *output);
int sj_object_get_uint32(SJson *object,const char *key,uint32_t *output);

/**
 * @brief get the json value from an object given a key as a signed 32bit integer
 * @param object the json object to get
 * @param key the key to search by
 * @param output where the value is written to
 * @return 0 if not found or not an integer, 1 if all good
 */
int sj_object_get_value_as_int32(SJson *object,const char *key,int32_t *output);
int sj_object_get_int32(SJson *object,const char *key,int32_t *output);

/**
 * @brief get the json value from an object given a key as a float
 * @param object the json object to get
 * @param key the key to search by
 * @param output where the value is written to
 * @return 0 if not found or not an integer, 1 if all good
 */
int sj_object_get_value_as_float(SJson *object,const char *key,float *output);
int sj_object_get_float(SJson *object,const char *key,float *output);

/**
 * @brief get the json value from an object given a key as a float
 * @param object the json object to get
 * @param key the key to search by
 * @param output where the value is written to
 * @return 0 if not found or not an integer, 1 if all good
 */
int sj_object_get_value_as_double(SJson *object,const char *key,double *output);
int sj_object_get_double(SJson *object,const char *key,double *output);

/**
 * @brief allocate a new empty json array
 * @return NULL on memory allocation error, an empty SJson array otherwise
 */
SJson *sj_array_new();

/**
 * @brief append to a JSON array a new value
 * @param array the JSON array to append to
 * @param value the value to add to the string
 * @note: this is a no-op if array is not an actual JSON array
 */
void sj_array_append(SJson *array,SJson *value);

/**
 * @brief delete a given value from the array by index
 * @param array the array to delete from
 * @param n the index of the item to delete.
 * @note: the value will be sj_free'd
 */
void sj_array_delete_nth(SJson *array,int n);

/**
 * @brief delete a given value from the array by its json handle
 * @param array the array to delete from
 * @param item the item to delete.
 * @note: the value will be sj_free'd
 */
void sj_array_delete_item(SJson *array,SJson *item);

/**
 * @brief get the number of elements in the json array
 * @param array the json array
 * @return 0 if there is an error, the count otherwise
 */
int sj_array_get_count(SJson *array);
int sj_array_count(SJson *array);   //shorthand

/**
 * @brief retrieve the nth element in the json array
 * @param array the array to search through
 * @param n the index of the element to get
 * @return NULL on error (check sj_get_error()) or the SJson value otherwise
 */
SJson *sj_array_get_nth(SJson *array,int n);
SJson *sj_array_nth(SJson *array,int n);

/**
 * @brief print the contents of the json file to stdout
 * @param json the json struct to print
 */
void sj_echo(SJson *json);

/**
 * @brief check if the json is an array
 * @param json the json to check
 * @return 1 if it is, 0 if not
 */
int sj_is_array(SJson *json);

/**
 * @brief check if the json is an object
 * @param json the json to check
 * @return 1 if it is, 0 if not
 */
int sj_is_object(SJson *json);

/**
 * @brief check if the json is a string
 * @param json the json to check
 * @return 1 if it is, 0 if not
 */
int sj_is_string(SJson *json);

/**
 * @brief check if the json is a number
 * @param json the json to check
 * @return 1 if it is, 0 if not
 */
int sj_is_number(SJson *json);

/**
 * @brief check if the json is a bool
 * @param json the json to check
 * @return 1 if it is, 0 if not
 */
int sj_is_bool(SJson *json);

/**
 * @brief check if the json is NULL
 * @param json the json to check
 * @return 1 if it is, 0 if not
 */
int sj_is_null(SJson *json);

/**
 * @brief enable automatic debug output to stdout
 * @note it is disabled by default
 */
void sj_enable_debug();

/**
 * @brief disable automatic debug output to stdout
 * @note it is disabled by default
 */
void sj_disable_debug();
#endif
