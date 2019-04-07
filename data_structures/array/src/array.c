#include <array.h>
#include <stdlib.h>
#include <string.h>


/*
    Insert @data in @pos in @array.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.
    @IN pos - position to insert.
    @IN data - pointer to data to insert.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
static int __array_insert_pos(void * __restrict__ array, const size_t len, const size_t size_of, const size_t pos, const void * __restrict__ const data);


/*
    Delete @data in @pos in @array. 
    If @destroy_f is not null, call destructor before deletion.

    PARAMS:
    @IN array - pointer to array.
    @IN len - length of array.
    @IN size_of - size of each member.
    @IN pos - position to insert.
    @IN data - pointer to data to insert.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
static int __array_delete_pos(void * __restrict__ array, const size_t len, const size_t size_of, const size_t pos, const destructor_f destroy_f);


static int __array_insert_pos(void * __restrict__ array, const size_t len, const size_t size_of, const size_t pos, const void * __restrict__ const data)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (pos > len)
        ERROR("pos > len\n", -1);

    if (data == NULL)
        ERROR("data == NULL\n", -1);

    BYTE *arr = (BYTE *)array;

    void *dst =  (void *)(arr + ((pos + 1) * size_of));
    const void *src = (const void *)(arr + (pos * size_of));
    const size_t to_move = (len - pos - 1) * size_of;

    if (memmove(dst, src, to_move) == NULL)
        ERROR("memmove error\n", -1);

    __ASSIGN__(arr[pos * size_of], *(BYTE *)data, size_of);

    return 0;
}


static int __array_delete_pos(void * __restrict__ array, const size_t len, const size_t size_of, const size_t pos, const destructor_f destroy_f)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (pos > len)
        ERROR("pos > len\n", -1);

    BYTE *arr = (BYTE *)array;

    if (destroy_f != NULL)
        destroy_f((void *)&(arr[pos * size_of]));

    void *dst = (void *)(arr + (pos * size_of));
    const void *src = (const void *)(arr + ((pos + 1) * size_of));
    const size_t to_move = (len - pos - 1) * size_of;

    if (memmove(dst, src, to_move) == NULL)
        ERROR("memmove error\n", -1);

    return 0;
}


void *array_create(const size_t len, const size_t size_of)
{
    /* preconditions */
    if (len == 0)
        ERROR("len == 0\n", NULL);

    if (size_of == 0)
        ERROR("size_of == 0\n", NULL);

    void *arr = calloc(len, size_of);

    /* assert */
    if (arr == NULL)
        ERROR("calloc error\n", NULL);

    return arr;
}


void array_destroy(void *array)
{
    /* preconditions */
    if (array == NULL)  
        VERROR("array == NULL\n");

    FREE(array);
}


void array_destroy_with_entries(void *array, const destructor_f destroy_f, const size_t len, const size_t size_of)
{
    /* preconditions */
    if (array == NULL)  
        VERROR("array == NULL\n");

    if (destroy_f == NULL)
        VERROR("destroy_f == NULL\n");

    if (len == 0)
        VERROR("len == 0\n");

    if (size_of == 0)
        VERROR("size_of == 0\n");

    BYTE* arr = (BYTE *)array;

    for (size_t offset = 0; offset < len * size_of; offset += size_of)
        destroy_f((void *)(arr + offset));

    FREE(array);
}


void array_copy(void * __restrict__ dst, const void * __restrict__ const src, const size_t len, const size_t size_of)
{
    /* preconditions */
    if (dst == NULL)  
        VERROR("dst == NULL\n");

    if (src == NULL)
        VERROR("src == NULL\n");

    if (len == 0)
        VERROR("len == 0\n");

    if (size_of == 0)
        VERROR("size_of == 0\n");

    (void)memcpy(dst, src, len * size_of);
}

void *array_clone(const void * const array, const size_t len, const size_t size_of)
{
    /* preconditions */
    if (array == NULL)  
        ERROR("array == NULL\n", NULL);

    if (len == 0)
        ERROR("len == 0\n", NULL);

    if (size_of == 0)
        ERROR("size_of == 0\n", NULL);

    void *arr = array_create(len, size_of);

    /* assert */
    if (arr == NULL)
        ERROR("array_create error\n", NULL);

    array_copy(arr, array, len, size_of);

    return arr;
}


void array_move(void * dst, void * src, const size_t len, const size_t size_of)
{
    /* preconditions */
    if (dst == NULL)  
        VERROR("dst == NULL\n");

    if (src == NULL)
        VERROR("src == NULL\n");

    if (len == 0)
        VERROR("len == 0\n");

    if (size_of == 0)
        VERROR("size_of == 0\n");

    (void)memmove(dst, src, len * size_of);
}


void array_zeros(void *array, const size_t len, const size_t size_of)
{
    /* preconditions */
    if (array == NULL)  
        VERROR("array == NULL\n");

    if (len == 0)
        VERROR("len == 0\n");

    if (size_of == 0)
        VERROR("size_of == 0\n");

    (void)memset(array, 0, len * size_of);
}


int array_unsorted_insert_first(void * __restrict__ array, const size_t len, const size_t size_of, const void * __restrict__ const data)
{
    return __array_insert_pos(array, len, size_of, 0, data);
}


int array_unsorted_insert_last(void * __restrict__ array, const size_t len, const size_t size_of, const void * __restrict__ const data)
{
    return __array_insert_pos(array, len, size_of, len - 1, data);
}


int array_unsorted_insert_pos(void * __restrict__ array, const size_t len, const size_t size_of, const size_t pos, const void * __restrict__ const data)
{
    return __array_insert_pos(array, len, size_of, pos, data);
}


int array_delete_first(void *array, const size_t len, const size_t size_of)
{
    return __array_delete_pos(array, len, size_of, 0, NULL);
}


int array_delete_last(void *array, const size_t len, const size_t size_of)
{
    return __array_delete_pos(array, len, size_of, len - 1, NULL);
}


int array_delete_pos(void *array, const size_t len, const size_t size_of, const size_t pos)
{
    return __array_delete_pos(array, len, size_of, pos, NULL);
}


int array_delete_first_with_entry(void *array, const size_t len, const size_t size_of, const destructor_f destroy_f)
{
    return __array_delete_pos(array, len, size_of, 0, destroy_f);
}


int array_delete_last_with_entry(void *array, const size_t len, const size_t size_of, const destructor_f destroy_f)
{
    return __array_delete_pos(array, len, size_of, len - 1, destroy_f);
}


int array_delete_pos_with_entry(void *array, const size_t len, const size_t size_of, const size_t pos, const destructor_f destroy_f)
{
    return __array_delete_pos(array, len, size_of, pos, destroy_f);
}


ssize_t array_lower_bound(const void * const array, const size_t len, const size_t size_of, const void * const data, const compare_f cmp_f)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (data == NULL)
        ERROR("data == NULL\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    const BYTE * const arr = (const BYTE * const)array;

    size_t offset_left = 0;
    size_t offset_right = len * size_of;
    
    while (offset_left < offset_right)
    {
        size_t offset_middle = ((offset_left / size_of + offset_right / size_of) / 2) * size_of;

        if (cmp_f(data, &arr[offset_middle]) <= 0)
            offset_right = offset_middle;
        else
            offset_left = offset_middle + size_of;
    }

    return (ssize_t)(offset_left / size_of);
}


ssize_t array_upper_bound(const void * const array, const size_t len, const size_t size_of, const void * const data, const compare_f cmp_f)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (data == NULL)
        ERROR("data == NULL\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    const BYTE * const arr = (const BYTE * const)array;

    size_t offset_left = 0;
    size_t offset_right = len * size_of;
    
    while (offset_left < offset_right)
    {
        size_t offset_middle = ((offset_left / size_of + offset_right / size_of) / 2) * size_of;

        if (cmp_f(data, &arr[offset_middle]) >= 0)
            offset_left = offset_middle + size_of;
        else
            offset_right = offset_middle;
    }

    return (ssize_t)(offset_left / size_of); 
}