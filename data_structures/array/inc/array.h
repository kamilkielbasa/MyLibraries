#ifndef ARRAY_H
#define ARRAY_H


#include <stddef.h>
#include <common.h>


/*
    Create new array with @len of members with size @size_of. 
    Each of member will be set to 0.

    PARAMS:
    @IN len - array length.
    @IN size_of - size of each member.

    RETURN:
    %NULL if failure.
    %Pointer to the new array if success.
*/
void *array_create(const size_t len, const size_t size_of);


/*
    Destroy array.

    PARAMS:
    @IN array - pointer to array.

    RETURN:
    %This is a void function.
*/
void array_destroy(void *array);


/*
    Destroy array with entries.

    PARAMS:
    @IN array - pointer to array.
    @IN destroy_f - pointer to destructor function.
    @IN len - array length.
    @IN size_of - size of each member.

    RETURN:
    %This is a void function.
*/
void array_destroy_with_entries(void *array, const destructor_f destroy_f, const size_t len, const size_t size_of);


/*
    Copy array from @dst to @src.

    PARAMS:
    @IN dst - pointer to destination array.
    @IN src - pointer to source array.
    @IN len - array length.
    @IN size_of - size of each member.

    RETURN:
    %This is void function.
*/
void array_copy(void * __restrict__ dst, const void * __restrict__ const src, const size_t len, const size_t size_of);


/*
    Clone @array to another array. This function will allocate memory for new array.

    PARAMS:
    @IN array - pointer to array.
    @IN len - array length.
    @IN size_of - size of each member.

    RETURN:
    %NULL if failure.
    %Pointer to new cloned array if success.
*/
void *array_clone(const void * const array, const size_t len, const size_t size_of);


/*
    Move array from @src pointer to @dst pointer. (Can overlap).

    PARAMS:
    @IN dst - pointer to destination array.
    @IN src - pointer to source array.
    @IN len - array length.
    @IN size_of - size of each member.

    RETURN:
    %This is void function.
*/
void array_move(void * dst, void * src, const size_t len, const size_t size_of);


/*
    Memset to 0 whole (or @len) array.

    PARAMS:
    @IN array - pointer to array.
    @IN len - array length.
    @IN size_of - size of each member.

    RETURN:
    %This is void function.
*/
void array_zeros(void *array, const size_t len, const size_t size_of);


/*
    Insert data in first pos.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.
    @IN data - pointer to data.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int array_unsorted_insert_first(void * __restrict__ array, const size_t len, const size_t size_of, const void * __restrict__ const data);


/*
    Insert data in last pos.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.
    @IN data - pointer to data.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int array_unsorted_insert_last(void * __restrict__ array, const size_t len, const size_t size_of, const void * __restrict__ const data);


/*
    Insert data in @pos.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.
    @IN pos - position to insert.
    @IN data - pointer to data.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int array_unsorted_insert_pos(void * __restrict__ array, const size_t len, const size_t size_of, const size_t pos, const void * __restrict__ const data);


/*
    Delete first element from array.
    At first entry will be 2nd entry etc. Last will be empty.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int array_delete_first(void *array, const size_t len, const size_t size_of);


/*
    Delete last element from array.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int array_delete_last(void *array, const size_t len, const size_t size_of); 


/*
    Delete entry at @pos from @array.
    At @pos will be (@pos + 1)th entry etc.
    Last will be empty.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.
    @IN pos - position in array.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int array_delete_pos(void *array, const size_t len, const size_t size_of, const size_t pos);


/*
    Delete first element from array. (call destructor)
    At first entry will be 2nd entry etc. Last will be empty.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int array_delete_first_with_entry(void *array, const size_t len, const size_t size_of, const destructor_f destroy_f);


/*
    Delete last element from array. (call destructor)

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int array_delete_last_with_entry(void *array, const size_t len, const size_t size_of, const destructor_f destroy_f);


/*
    Delete entry at @pos from @array. (call destructor)
    At @pos will be (@pos + 1)th entry etc.
    Last will be empty.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.
    @IN pos - position in array.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int array_delete_pos_with_entry(void *array, const size_t len, const size_t size_of, const size_t pos, const destructor_f destroy_f);


#endif /* ARRAY_H */