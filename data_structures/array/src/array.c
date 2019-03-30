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
    const void *src = (void *)(arr + (pos * size_of));
    size_t to_move = (len - pos - 1) * size_of;

    if (memmove(dst, src, to_move) == NULL)
        ERROR("memmove error\n", -1);

    __ASSIGN__(arr[pos * size_of], *(BYTE *)data, size_of);

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