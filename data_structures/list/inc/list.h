#ifndef LIST_H
#define LIST_H


/*
    Linked list implementation

    Author: Kamil Kiełbasa
    email: kamilkielbasa73@gmail.com

    LICENCE: GPL 3.0
*/


#include <stddef.h> /* size_t */
#include <sys/types.h> /* ssize_t */
#include <common.h> /* compare_f */


typedef struct List List;
typedef struct List_node List_node;


/*
    Create new instance of list.

    PARAMS:
    @IN size_of - size of element.
    @IN cmp_f - pointer to compare function.
    @IN destroy_f - pointer to destructor function.
    @IN print_f - pointer to print function.

    RETURN:
    %NULL if failure.
    %Pointer to list if success.
*/
List *list_create(const size_t size_of, const compare_f cmp_f, const destructor_f destroy_f);


/*
    Destroy list.

    PARAMS:
    @IN list - pointer to list.

    RETURN:
    %This is void function.
*/
void list_destroy(List *list);


/*
    Destroy list with all entires. (call destructor for each entires)

    PARAMS:
    @IN list - pointer to list.

    RETURN:
    %This is void function.
*/
void list_destroy_with_entries(List *list);


/*
    Insert element to sorted list.

    PARAMS:
    @IN list - pointer to list.
    @IN entry - pointer to entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list_insert(List * __restrict__ list, const void * __restrict__ const entry);


/*
    Delete the first entry which compare(list->entry, entry) == 0.

    PARAMS:
    @IN list - pointer to list.
    @IN entry - pointer to entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list_delete(List * __restrict__ list, const void * __restrict__ const entry);


/*
    Delete the first entry which compare(list->entry, entry) == 0. (call destructor)

    PARAMS:
    @IN list - pointer to list.
    @IN entry - pointer to entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list_delete_with_entry(List * __restrict__ list, const void * __restrict__ const entry);


/*
    Delete the all entries which compare(list->entry, entry) == 0.

    PARAMS:
    @IN list - pointer to list.
    @IN entry - pointer to entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list_delete_all(List * __restrict__ const list, const void * __restrict__ const entry);


/*
    Delete the all entries which compare(list->entry, entry) == 0. (call destructor)

    PARAMS:
    @IN list - pointer to list.
    @IN entry - pointer to entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list_delete_all_with_entry(List * __restrict__ const list, const void * __restrict__ const entry);


/*
    Search for entry which compare(list->entry, val) == 0.

    If you have your own struct, you have to use `fake struct` with correct key.

    For example:

    struct Entry {
        int64_t key;
        const char* name;
        const char* surname;
        const size_t age;
    };

    set only correct key.
    struct Entry fake_entry = { searched_key, NULL, NULL, 0 };
    struct Entry out_entry = {0};

    and search.
    list_search(list, (const void * const)&val, (void *)&entry);

    PARAMS:
    @IN list - pointer to list.
    @IN val - pointer to search value.
    @OUT entry - pointer to entry with found val if val was searched and entry != NULL.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list_search(const List * __restrict__ const list, const void * const val, void *entry);


/*
    Create array from list.

    PARAMS:
    @IN list - pointer to list.
    @OUT array - pointer to array.
    @OUT size - pointer to returned size of array.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int list_to_array(const List * __restrict__ const list, void * __restrict__ array, size_t * __restrict__ size);


/*
    Get number of entries in list.

    PARAMS:
    @IN list - pointer to list.

    RETURN:
    %-1 if failure
    %number of entries.
*/
ssize_t list_get_num_entries(const List * const list);


/*
    Get size of data list.

    PARAMS:
    @IN list - pointer to list.

    RETURN:
    %-1 if failure
    %number of entries.
*/
ssize_t list_get_data_size(const List * const list);


#endif /* LIST_H */
