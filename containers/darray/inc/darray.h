#ifndef DARRAY_H
#define DARRAY_H


/*
    Dynamic array implementation

    Author: Kamil Kiełbasa
    email: kamilkielbasa73@gmail.com

    LICENCE: GPL 3.0
*/


#include <stddef.h> /* size_t */
#include <sys/types.h> /* ssize_t */
#include <common.h> /* compare_f, destroy_f */


typedef enum DARRAY_TYPE
{
    DARRAY_SORTED = 0,      /* type of sorted array */
    DARRAY_UNSORTED         /* type of unsorted array */
} DARRAY_TYPE;


typedef struct Darray 
{
    void *array;	          /* main array */
    compare_f cmp_f;          /* pointer to compare function */
    destructor_f destroy_f;   /* pointer to destroy function */

    DARRAY_TYPE type;         /* dynamic array type (sorted / unsorted) */
    size_t size_of;	          /* size of element */
    size_t num_entries;       /* number of entries in array */
    size_t size;	          /* current allocated size of array */
} Darray;


/*
    Create new instance of dynamic array.

    PARAMS:
    @IN type - type of darray.
    @IN size_of - size of element.
    @IN size - beggining size of darray.
    @IN cmp_f - pointer to compare function.

    RETURN:
    %NULL if failure.
    %Pointer to dynamic array if success.
*/
Darray *darray_create(const DARRAY_TYPE type, const size_t size_of, const size_t size, const compare_f cmp_f, const destructor_f destroy_f);


/*
    Deallocate dynamic array with all entries.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %This is void function.
*/
void darray_destroy(Darray *darray);


/*
    Insert an entry at the end of the array.

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN entry - entry to inserted.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int darray_insert(Darray * __restrict__ darray, const void * __restrict__ const entry);


/*
    Delete an entry at the end of the array.

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN val_out - pointer to deleted entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int darray_delete(Darray * __restrict__ darray, void * __restrict__ val_out);


/*
    Insert an entry to array in selected pos (array[pos]).

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN entry - pointer to entry.
    @IN pos - index where insert.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int darray_insert_pos(Darray * __restrict__ darray, const void * __restrict__ const entry, const size_t pos);


/*
    Delete an element from array selected as pos (array[pos]).

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN outdoor - pointer to deleted entry.
    @IN pos - index where insert.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int darray_delete_pos(Darray * __restrict__ darray, void * __restrict__ val_out, const size_t pos);


/*
    Get data from array[pos].

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN outdoor - pointer to deleted entry.
    @IN pos - index where getting data.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int darray_get_data(const Darray * __restrict__ const darray, void * __restrict__ val_out, const size_t pos);


/*
    Find first key from darray using compare function.

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN key - pointer to search key.
    @IN val_out - pointer for value with found key.

    RETURN:
    %index in darray if success.
    %-1 if failure.
*/
ssize_t darray_search_first(const Darray * __restrict__ const darray, const void * __restrict__ key, void * __restrict__ val_out);


/*
    Find last key from darray using compare function.

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN key - pointer to search key.
    @IN val_out - pointer for value with found key.

    RETURN:
    %index in darray if success.
    %-1 if failure.
*/
ssize_t darray_search_last(const Darray * __restrict__ const darray, const void * __restrict__ const key, void * __restrict__ val_out);


/*
    Find minimum value from darray.

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN val_out - pointer to value out.

    RETURN:
    %index in darray if succes and val_out != NULL.
    %-1 if failure and val_out == NULL.
*/
ssize_t darray_search_min(const Darray * __restrict__ const darray, void * __restrict__ val_out);


/*
    Find maximum value from darray.

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN val_out - pointer to value out.

    RETURN:
    %index in darray if succes and val_out != NULL.
    %-1 if failure and val_out == NULL.
*/
ssize_t darray_search_max(const Darray * __restrict__ const darray, void * __restrict__ val_out);


/*
    Sorts darray.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int darray_sort(Darray *darray);


/*
    Get size of array.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %num of entries if success.
    %negative value if failure.
*/
ssize_t darray_get_size(const Darray * const darray);


/*
    Get size of each data.

    PARAMS:
    @IN darray - pointer to darray.

    RETURN:
    %number of entries if success.
    %-1 if failure.
*/
ssize_t darray_get_data_size(const Darray * const darray);


/*
    Get number of entries.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %num of entries if success.
    %-1 if failure.
*/
ssize_t darray_get_num_entries(const Darray * const darray);


/*
    Get pointer to darray array.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %pointer to array if success.
    %NULL if darray wasn't create or failure.
*/
void *darray_get_array(const Darray * const darray);


#endif /* DARRAY_H */
