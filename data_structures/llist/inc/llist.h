#ifndef LLIST_H
#define LLIST_H

/*
    Doubly linked list implementation

    Author: Kamil Kiełbasa
    email: kamilkielbasa73@gmail.com

    LICENCE: GPL 3.0
*/

#include <stddef.h> /* size_t */
#include <sys/types.h> /* ssize_t */
#include <common.h> /* compare_f */


typedef struct List1d_node
{
    struct List1d_node *next;   /* pointer to next node */
    size_t size_of;             /* size of node */
    
    char data[];                /* placeholder for data */
} List1d_node;


typedef struct List1d
{
    List1d_node *head;          /* pointer to head */
    List1d_node *tail;          /* pointer to tail */

    compare_f cmp_f;            /* compare function */

    size_t size;                /* num of nodes in list */
    size_t size_of;             /* size of element */
} List1d;


/*
    Create new instance of list.

    PARAMS:
    @IN size_of - size of element.
    @IN cmp_f - pointer to compare function.

    RETURN:
    %NULL if failure.
    %Pointer to list if success.
*/
List1d *list1d_create(const size_t size_of, compare_f cmp_f);


/*
    Destroy existed instance of list.

    PARAMS:
    @IN list - pointer to list.

    RETURN:
    %This is void function.
*/
void list1d_destroy(List1d *list);


/*
    Create new instance of list.

    PARAMS:
    @IN list - pointer to list.
    @IN entry - pointer to entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list1d_insert(List1d * __restrict__ const list, const void * __restrict__ const entry);


/*
    Delete the first found entry which compare(list->entry, entry) == 0.

    PARAMS:
    @IN list - pointer to list.
    @IN entry - pointer to entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list1d_delete(List1d * __restrict__ const list, const void * __restrict__ const entry);


/*
    Delete the all entries which compare(list->entry, entry) == 0.

    PARAMS:
    @IN list - pointer to list.
    @IN entry - pointer to entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list1d_delete_all(List1d * __restrict__ const list, const void * __restrict__ const entry);


/*
    Search for entry which compare(list->entry, val) == 0.

    PARAMS:
    @IN list - pointer to list.
    @IN val - pointer to search value.
    @OUT entry - pointer to entry with found val if val was searched and entry != NULL.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list1d_search(const List1d * __restrict__ const list, const void *val, void *entry);


/*
    Delete the all entries which compare(list->entry, entry) == 0.

    PARAMS:
    @IN list - pointer to list.
    @OUT array - pointer to array.
    @OUT size - pointer to returned size of array.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list1d_to_array(const List1d * __restrict__ const list, void * __restrict__ array, size_t * __restrict__ size);


/*
    Get number of entries in list.

    PARAMS:
    @IN list - pointer to list.

    RETURN:
    %-1 if failure
    %number of entries.
*/
ssize_t list1d_get_num_entries(const List1d * const list);


/*
    Get size of data list.

    PARAMS:
    @IN list - pointer to list.

    RETURN:
    %-1 if failure
    %number of entries.
*/
ssize_t list1d_get_data_size(const List1d * const list);


#endif /* LLIST_H */
