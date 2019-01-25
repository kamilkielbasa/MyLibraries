#ifndef _DARRAY_H_
#define _DARRAY_H_

/*
    Dynamic array implementation

    Author: Kamil Kiełbasa
    email: kamilkielbasa73@gmail.com

    LICENCE: GPL 3.0
*/

#include <stdbool.h> /* bool */
#include <stddef.h> /* size_t */
#include <sys/types.h> /* ssize_t */

typedef int (*compare_func)(const void *, const void *);

typedef enum DARRAY_TYPE
{
    DARRAY_SORTED = 0,      /* type of sorted array */
    DARRAY_UNSORTED         /* type of unsorted array */
} DARRAY_TYPE;

typedef struct Darray_iterator
{
    void *array;

    ssize_t index;          /* index in array */
    size_t size_of;         /* size of element */
    size_t array_length;    /* current allocated size of array */
} Darray_iterator;

typedef struct Darray 
{
	void *array;			/* main array */
    compare_func cmp_f;     /* pointer to compare function */

    DARRAY_TYPE type;       /* dynamic array type (sorted / unsorted) */
	size_t size_of;			/* size of element */
	size_t num_entries; 	/* number of entries in array */
	size_t size;			/* current allocated size of array */
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
Darray *darray_create(const DARRAY_TYPE type, const size_t size_of, const size_t size, compare_func cmp_f);

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
int darray_insert(Darray * restrict darray, const void * restrict entry);

/*
    Delete an entry at the end of the array.

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN val_out - pointer to deleted entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int darray_delete(Darray * restrict darray, void * restrict val_out);

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
int darray_insert_pos(Darray * restrict darray, const void * restrict entry, const size_t pos);

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
int darray_delete_pos(Darray * restrict darray, void * restrict val_out, const size_t pos);

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
int darray_get_data(const Darray * const restrict darray, void * restrict val_out, const size_t pos);

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
ssize_t darray_search_first(const Darray * const restrict darray, const void * const restrict key, void * restrict val_out);

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
ssize_t darray_search_last(const Darray * const restrict darray, const void * const restrict key, void * restrict val_out);

/*
    Find minimum value from darray.

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN val_out - pointer to value out.

    RETURN:
    %index in darray if succes and val_out != NULL.
    %-1 if failure and val_out == NULL.
*/
ssize_t darray_search_min(const Darray * const restrict darray, void * restrict val_out);

/*
    Find maximum value from darray.

    PARAMS:
    @IN darray - pointer to dynamic array.
    @IN val_out - pointer to value out.

    RETURN:
    %index in darray if succes and val_out != NULL.
    %-1 if failure and val_out == NULL.
*/
ssize_t darray_search_max(const Darray * const restrict darray, void * restrict val_out);

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
    Reverse sort darray.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int darray_reverse_sort(Darray *darray);

/*
    Check type of the darray.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %type of darray.
*/
DARRAY_TYPE darray_is_sorted(const Darray * const darray);

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
    Get number of entries.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %num of entries if success.
    %-1 if failure.
*/
ssize_t darray_get_num_entries(const Darray * const darray);

/*
    Get type of darray.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %num of entries if success.
    %-1 if failure.
*/
ssize_t darray_get_data_type(const Darray * const darray);

/*
    Get pointer to darray array.

    PARAMS:
    @IN darray - pointer to dynamic array.

    RETURN:
    %pointer to array if success.
    %NULL if darray wasn't create or failure.
*/
void *darray_get_array(const Darray * const darray);

#endif /* _DARRAY_H_ */
