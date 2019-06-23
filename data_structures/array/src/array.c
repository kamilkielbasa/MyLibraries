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


int array_sorted_insert(void * __restrict__ array, const size_t len, const size_t size_of, const compare_f cmp_f, const void * __restrict__ const data)
{
    if (len == 1)
        return __array_insert_pos(array, len, size_of, 0, data);

    ssize_t index = array_upper_bound(array, len - 1, size_of, cmp_f, data);

    if (index == -1)
        ERROR("array_upper_bound error\n", -1);

    return __array_insert_pos(array, len, size_of, (size_t)index, data);
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


ssize_t array_lower_bound(const void * const array, const size_t len, const size_t size_of, const compare_f cmp_f, const void * const data)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    if (data == NULL)
        ERROR("data == NULL\n", -1);

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


ssize_t array_upper_bound(const void * const array, const size_t len, const size_t size_of, const compare_f cmp_f, const void * const data)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    if (data == NULL)
        ERROR("data == NULL\n", -1);

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


int array_sort(void * __restrict__ array, const size_t len, const size_t size_of, const compare_f cmp_f)
{
    if (array == NULL)      
        ERROR("array == NULL\n", -1);

    if (len == 0)   
        ERROR("len == 0\n", -1);

    if (size_of == 0)   
        ERROR("size_of == 0\n", -1);

    if (cmp_f == NULL)  
        ERROR("cmp_f == NULL\n", -1);

    qsort(array, len, size_of, cmp_f);

    return 0;
}


ssize_t array_min(const void * __restrict__ const array, const size_t len, const size_t size_of, const compare_f cmp_f, void * __restrict__ min)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    BYTE *_min = (BYTE *)array_create(1UL, size_of);

    if (_min == NULL)
        ERROR("malloc error\n", -1);

    BYTE *arr = (BYTE *)array; 

    __ASSIGN__(_min[0], arr[0], size_of);

    const size_t offset_max = size_of * len;
    size_t pos = 0;

    for (size_t offset = size_of; offset < offset_max; offset += size_of)
    {
        if (cmp_f((const void *)&_min[0], (const void *)&arr[offset]) > 0)
        {
            __ASSIGN__(_min[0], arr[offset], size_of);
            pos = offset;
        }
    }

    if (min != NULL)
        __ASSIGN__(*(BYTE *)min, arr[pos], size_of);

    array_destroy((void *)_min);

    return (ssize_t)pos / (ssize_t)size_of;
}


ssize_t array_max(const void * __restrict__ const array, const size_t len, const size_t size_of, const compare_f cmp_f, void * __restrict__ max)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    BYTE *_max = (BYTE *)array_create(1UL, size_of);

    if (_max == NULL)
        ERROR("malloc error\n", -1);

    BYTE *arr = (BYTE *)array; 

    __ASSIGN__(_max[0], arr[0], size_of);

    const size_t offset_max = size_of * len;
    size_t pos = 0;

    for (size_t offset = size_of; offset < offset_max; offset += size_of)
    {
        if (cmp_f((const void *)&_max[0], (const void *)&arr[offset]) < 0)
        {
            __ASSIGN__(_max[0], arr[offset], size_of);
            pos = offset;
        }
    }

    if (max != NULL)
        __ASSIGN__(*(BYTE *)max, arr[pos], size_of);

    array_destroy((void *)_max);

    return (ssize_t)pos / (ssize_t)size_of;
}


ssize_t array_unsorted_find_first(const void * __restrict__ const array, const size_t len, const size_t size_of, const compare_f cmp_f, const void * __restrict__ const key, void * __restrict__ val)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    if (key == NULL)
        ERROR("key == NULL\n", -1);

    BYTE *arr = (BYTE *)array;

    const size_t offset_max = len * size_of;
    ssize_t index = -1;

    for (size_t offset = 0; offset < offset_max; offset += size_of)
    {
        if (cmp_f((const void *)(arr + offset), key) == 0)
        {
            index = (ssize_t)(offset / size_of);
            break;
        }
    }

    if (index == -1)
        return -1;

    if (val != NULL)
        __ASSIGN__(*(BYTE *)val, arr[(size_t)index * size_of], size_of);

    return index;
}


ssize_t array_unsorted_find_last(const void * __restrict__ const array, const size_t len, const size_t size_of, const compare_f cmp_f, const void * __restrict__ const key, void * __restrict__ val)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    if (key == NULL)
        ERROR("key == NULL\n", -1);

    BYTE *arr = (BYTE *)array;

    const ssize_t offset_max = (const ssize_t)((len - 1) * size_of);
    ssize_t index = -1;

    for (ssize_t offset = offset_max; offset >= 0; offset -= (ssize_t)size_of)
    {
        if (cmp_f((const void *)(arr + offset), key) == 0)
        {
            index = offset / (ssize_t)size_of;
            break;
        }
    }

    if (index == -1)
        return -1;

    if (val != NULL)
        __ASSIGN__(*(BYTE *)val, arr[(size_t)index * size_of], size_of);

    return index;
}


ssize_t array_sorted_find_first(const void * __restrict__ const array, const size_t len, const size_t size_of, const compare_f cmp_f, const void * __restrict__ const key, void * __restrict__ val)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    if (key == NULL)
        ERROR("key == NULL\n", -1);

    BYTE* arr = (BYTE *)array;

    size_t left_offset = 0;
    size_t right_offset = (len - 1) * size_of;

    while (left_offset < right_offset)
    {
        size_t middle_offset = ((left_offset / size_of + right_offset / size_of) >> 1) * size_of;

        if (cmp_f((const void *)(arr + middle_offset), key) < 0)
            left_offset = middle_offset + size_of;
        else
            right_offset = middle_offset;
    }

    if (cmp_f((const void *)(arr + left_offset), key) == 0)
    {
        if (val != NULL)
            __ASSIGN__(*(BYTE *)val, arr[left_offset], size_of);

        return (ssize_t)(left_offset / size_of);
    }

    return -1;
}


ssize_t array_sorted_find_last(const void * __restrict__ const array, const size_t len, const size_t size_of, const compare_f cmp_f, const void * __restrict__ const key, void * __restrict__ val)
{
    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", -1);

    if (key == NULL)
        ERROR("key == NULL\n", -1);

    BYTE* arr = (BYTE *)array;

    size_t left_offset = 0;
    size_t right_offset = (len - 1) * size_of;

    while (left_offset < right_offset)
    {
        size_t middle_offset = ((left_offset / size_of + right_offset / size_of + 1) >> 1) * size_of;

        if (cmp_f((const void *)(arr + middle_offset), key) > 0)
            right_offset = middle_offset - size_of;
        else
            left_offset = middle_offset;
    }

    if (cmp_f((const void *)(arr + left_offset), key) == 0)
    {
        if (val != NULL)
            __ASSIGN__(*(BYTE *)val, arr[left_offset], size_of);

        return (ssize_t)(left_offset / size_of);
    }

    return -1;
}

