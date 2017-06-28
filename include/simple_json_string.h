#ifndef __SIMPLE_JSON_STRING_H__
#define __SIMPLE_JSON_STRING_H__

typedef struct
{
    char *text;
    int size;
}SJString;

/**
 * @brief make a new empty string
 * @return NULL on error or a new string that must be freed with sj_string_free
 */
SJString *sj_string_new();

/**
 * @brief make a new string with the characters from array s
 * @param s the character array to populate the string with
 * @return NULL on error or a new string that must be freed with sj_string_free
 */
SJString *sj_string_new_text(char *s);

/**
 * @brief free an SJString
 * @param string the string to free
 */
void sj_string_free(SJString *string);

/**
 * @brief performs a strcmp on the string with the provided character array
 * @param string the SJString to check
 * @param s the character array to compare with
 * @return <0 id string has a lower value than s, >0 if s has a lower value than string, 0 if they are equivalant
 */
int sj_string_cmp(SJString *string,char *s);

/**
 * @brief set the value of string to s
 * @param string the string to change
 * @param s the character array to change it to
 */
void sj_string_set(SJString *string,char *s);

/**
 * @brief set the value of string to s but only length of s
 * @param string the string to change
 * @param s the character array to change it to
 * @param length the limit on characters to copy from s
 */
void sj_string_set_limit(SJString *string,char *s,unsigned long length);

/**
 * @brief get the text back from the string
 * @param string the string to get
 * @return NULL on error or the character array containing the string text
 */
char *sj_string_get_text(SJString *string);

#endif
