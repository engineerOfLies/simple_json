#ifndef __SIMPLE_JSON_ERROR_H__
#define __SIMPLE_JSON_ERROR_H__

/**
 * @brief get the last reported error for simple json
 * @return a character array with the error message
*/
char *sj_get_error();

/**
 * @brief used internally to se the error message
 * @param error the error message to set
 */
void sj_set_error(char *error);

#endif
