#ifndef __SIMPLE_JSON_LIST_H__
#define __SIMPLE_JSON_LIST_H__

typedef struct
{
    void *data;
}SJListElementData;

typedef struct
{
    SJListElementData *elements;
    unsigned int size;
    unsigned int count;
}SJList;

SJList *sj_list_new();

/**
 * @brief allocate a new empty list of size 'count'
 * @param count how many elements you wish to support in this list.
 * @return NULL on memory error or a new empty list
 */
SJList *sj_list_new_size(unsigned int count);

/**
 * @brief deletes a list that has been previously allocated
 * @param list the list to delete;
 */
void sj_list_delete(SJList *list);

/**
 * @brief get the data stored at the nth element
 * @param list the list to pull data from
 * @param n which element to look out
 * @return NULL on error (such as if n > the element count) or the address of the data otherwise
 */
void *sj_list_get_nth(SJList *list,unsigned int n);

/**
 * @brief add an element to the end of the list
 * @param list the list to add to
 * @param data the data to assign to the new element
 */
void sj_list_append(SJList *list,void *data);

/**
 * @brief instert a new element at the position provided
 * @param list the list to insert into
 * @param data the data to assin to the new element
 * @return the address of the list.  In the event that the list needed to expand this will be different from the list provided
 */
SJList *sj_list_insert(SJList *list,void *data,unsigned int n);

/**
 * @brief delete the element at the nth position in the list
 * @param list the list to delete out of
 * @param n the element to delete.  This is no-op if the nth element is beyond the scope of the list (event is logged)
 * @return the list provided
 */
SJList *sj_list_delete_nth(SJList *list,unsigned int n);

/**
 * @brief delete the first element in the list pointing to the address of data
 * @note does not delete the data itself
 * @param list the list to delete the element from
 * @param data used to match against which element to delete
 * @return 0 on complete, error otherwise
 */
int sj_list_delete_data(SJList *list,void *data);

/**
 * @brief get the number of tracked elements in the list
 * @param list the list the check
 * @return the count in the list.  Will be zero if list was NULL
 */
unsigned int sj_list_get_count(SJList *list);

#endif
