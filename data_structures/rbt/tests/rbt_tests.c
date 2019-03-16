#include <rbt.h>
#include <common.h>
#include <ctest.h>
#include <stdint.h> /* int64_t */
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

typedef struct MyStruct
{
    int64_t key;
    int64_t a;
    int64_t b;
    int64_t c;
} MyStruct;


static MyStruct *my_struct_create(int64_t key)
{
    MyStruct *s = (MyStruct *)malloc(sizeof(MyStruct));
    s->key = key;
    s->a = 0;
    s->b = 0;
    s->c = 0;

    return s;
}


static void my_struct_destroy(void *s)
{
    MyStruct *ms = *(MyStruct **)s;
    FREE(ms);
}


static int my_struct_compare(const void *a, const void *b)
{
    assert(a != NULL && b != NULL);

    const MyStruct *s1 = *(const MyStruct **)a;
    const MyStruct *s2 = *(const MyStruct **)b;

    if (s1->key > s2->key) return 1;
    if (s1->key == s2->key) return 0;
	return -1;
}


static int my_compare_int64_t(const void *a, const void *b)
{
    assert(a != NULL && b != NULL);

    const int64_t *ia = (const int64_t *)a;
    const int64_t *ib = (const int64_t *)b;

    if (*ia > *ib) return 1;
    if (*ia == *ib) return 0;
    return -1;
}


static size_t mylog2(size_t val)
{
    if (val == 0) return SIZE_MAX;
    if (val == 1) return 0;

    size_t ret = 0;

    while (val > 1)
    {
        val >>= 1;
        ++ret;
    }

    return ret;
}


// need to be moved to common.h
bool correct_balanced_height(int height, size_t n);
bool correct_balanced_height(int height, size_t n)
{
    return height < (int)((mylog2(n) + 1) << 1);
}


bool correct_height(int height, size_t n);
bool correct_height(int height, size_t n)
{
    return correct_balanced_height(height, n);
}


static void test_rbt_create(void)
{
    Rbt *tree; 

    tree = rbt_create(sizeof(int), my_struct_compare, NULL, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);
    T_EXPECT(rbt_get_height(tree), 0);
    rbt_destroy(tree);

    tree = rbt_create(sizeof(long), my_struct_compare, NULL, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(long));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);
    T_EXPECT(rbt_get_height(tree), 0);
    rbt_destroy(tree);

    tree = rbt_create(sizeof(MyStruct), my_struct_compare, my_struct_destroy, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(MyStruct));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);
    T_EXPECT(rbt_get_height(tree), 0);
    rbt_destroy(tree);
}


static void test_rbt_insert_only_root(void)
{
    Rbt *tree; 

    MyStruct my_struct = { 2, 199, 200, 201 };
    MyStruct *arr;
    size_t size;

    tree = rbt_create(sizeof(MyStruct), my_struct_compare, NULL, NULL);
    T_EXPECT(rbt_insert(tree, &my_struct), 0);
    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(MyStruct));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)1);
    T_EXPECT(rbt_get_height(tree), 1);
    T_CHECK(tree->root->parent == tree->root->left_son);
    T_CHECK(tree->root->left_son == tree->root->right_son);

    T_EXPECT(rbt_to_array(tree, (void *)&arr, &size), 0);
    T_CHECK(size == (size_t)1);
    T_CHECK(arr->key == my_struct.key);
    T_CHECK(arr->a == my_struct.a);
    T_CHECK(arr->b == my_struct.b);
    T_CHECK(arr->c == my_struct.c);

    FREE(arr);
    rbt_destroy(tree);
}


static void test_rbt_insert(void)
{
    const size_t max_num_of_entries = 1000;

    for (size_t num_of_entries = 100; num_of_entries < max_num_of_entries; num_of_entries += 100)
    {
        Rbt *tree;

        int64_t *arr;
        size_t size = num_of_entries;

        int64_t *rarr;
        size_t rsize;

        arr = (int64_t *)malloc(sizeof(int64_t) * size);
        T_ERROR(arr == NULL);

        for (size_t i = 0; i < size; ++i)
            arr[i] = (int64_t)(i + 1);

        for (size_t i = 0; i < size; ++i)
        {
            size_t index = (size_t)rand() % (size - 1);
            SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
        }

        tree = rbt_create(sizeof(int64_t), my_compare_int64_t, NULL, NULL);
        T_ERROR(tree == NULL);
        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

        for (size_t i = 0; i < size; ++i)
            T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 0);

        qsort((void *)&arr[0], size, sizeof(int64_t), my_compare_int64_t);

        T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);
        T_CHECK(size == rsize);
        T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], size), 0);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
        T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

        FREE(arr);
        FREE(rarr);
        rbt_destroy(tree);
    }
}


static void test_rbt_insert_the_same(void)
{
    const size_t max_num_of_entries = 1000;

    for (size_t num_of_entries = 100; num_of_entries < max_num_of_entries; num_of_entries += 100)
    {
        Rbt *tree;

        int64_t *arr;
        size_t size = num_of_entries;

        int64_t *rarr;
        size_t rsize;

        arr = (int64_t *)malloc(sizeof(int64_t) * size);
        T_ERROR(arr == NULL);

        for (size_t i = 0; i < size; ++i)
            arr[i] = (int64_t)(i + 1);

        for (size_t i = 0; i < size; ++i)
        {
            size_t index = (size_t)rand() % (size - 1);
            SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
        }

        tree = rbt_create(sizeof(int64_t), my_compare_int64_t, NULL, NULL);
        T_ERROR(tree == NULL);
        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

        for (size_t i = 0; i < size; ++i)
            T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 0);

        for (size_t i = 0; i < size; ++i)
            T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 1);

        qsort((void *)&arr[0], size, sizeof(int64_t), my_compare_int64_t);

        T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);
        T_CHECK(size == rsize);
        T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], size), 0);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
        T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

        FREE(arr);
        FREE(rarr);
        rbt_destroy(tree);
    }
}


static void test_rbt_destroy_with_entries(void)
{
    const size_t max_num_of_entries = 1000;

    for (size_t num_of_entries = 100; num_of_entries < max_num_of_entries; num_of_entries += 100)
    {
        Rbt *tree;
        MyStruct *s;
        
        int64_t *arr;
        size_t size = 100;

        arr = (int64_t *)malloc(sizeof(int64_t) * size);
        T_ERROR(arr == NULL);

        for (size_t i = 0; i < size; ++i)
            arr[i] = (int64_t)(i + 1);

        for (size_t i = 0; i < size; ++i)
        {
            size_t index = (size_t)rand() % (size - 1);
            SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
        }

        tree = rbt_create(sizeof(MyStruct), my_struct_compare, my_struct_destroy, NULL);
        T_ERROR(tree == NULL);
        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(MyStruct));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

        for (size_t i = 0; i < size; ++i)
        {
            s = my_struct_create(arr[i]);
            T_EXPECT(rbt_insert(tree, (void *)&s), 0);
        }

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(MyStruct));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
        T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

        FREE(arr);
        rbt_destroy_with_entries(tree);
    }
}


static void test_rbt_min_max(void)
{
    const size_t max_num_of_entries = 1000;

    for (size_t num_of_entries = 100; num_of_entries < max_num_of_entries; num_of_entries += 100)
    {
        Rbt *tree;

        int64_t *arr;
        size_t size = num_of_entries;

        int64_t *rarr;
        size_t rsize;

        int64_t val;
        int64_t min;
        int64_t max;

        arr = (int64_t *)malloc(sizeof(int64_t) * size);
        T_ERROR(arr == NULL);

        for (size_t i = 0; i < size; ++i)
            arr[i] = (int64_t)(i + 1);

        for (size_t i = 0; i < size; ++i)
        {
            size_t index = (size_t)rand() % (size - 1);
            SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
        }

        tree = rbt_create(sizeof(int64_t), my_compare_int64_t, NULL, NULL);
        T_ERROR(tree == NULL);
        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

        for (size_t i = 0; i < size; ++i)
            T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 0);

        qsort((void *)&arr[0], size, sizeof(int64_t), my_compare_int64_t);

        min = arr[0];
        max = arr[size - 1];

        T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);
        T_CHECK(size == rsize);
        T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], size), 0);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
        T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

        T_EXPECT(rbt_min(tree, (void *)&val), 0);
        T_CHECK(min == val);

        T_EXPECT(rbt_max(tree, (void *)&val), 0);
        T_CHECK(max == val);

        FREE(arr);
        FREE(rarr);
        rbt_destroy(tree);
    }
}


static void test_rbt_search(void)
{
    const size_t max_num_of_entries = 1000;

    for (size_t num_of_entries = 100; num_of_entries < max_num_of_entries; num_of_entries += 100)
    {
        Rbt *tree;

        int64_t *arr;
        size_t size = num_of_entries;

        int64_t *rarr;
        size_t rsize;

        int64_t val;

        arr = (int64_t *)malloc(sizeof(int64_t) * size);
        T_ERROR(arr == NULL);

        for (size_t i = 0; i < size; ++i)
            arr[i] = (int64_t)(i + 1);

        for (size_t i = 0; i < size; ++i)
        {
            size_t index = (size_t)rand() % (size - 1);
            SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
        }

        tree = rbt_create(sizeof(int64_t), my_compare_int64_t, NULL, NULL);
        T_ERROR(tree == NULL);
        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

        for (size_t i = 0; i < size; ++i)
            T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 0);

        qsort((void *)&arr[0], size, sizeof(int64_t), my_compare_int64_t);

        T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);
        T_CHECK(size == rsize);
        T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], size), 0);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
        T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

        for (size_t i = 0; i < size; ++i)
        {
            T_CHECK(rbt_search(tree, (void *)&arr[i], (void *)&val) == 0);
            T_ASSERT(val, arr[i]);
        }

        for (size_t i = 0; i < size; ++i)
        {
            val = arr[i] + (int64_t)size;
            T_CHECK(rbt_search(tree, (void *)&val, (void *)&val) != 0);
        }

        FREE(arr);
        FREE(rarr);
        rbt_destroy(tree);
    }
}


static void test_rbt_key_exist(void)
{
    const size_t max_num_of_entries = 1000;

    for (size_t num_of_entries = 100; num_of_entries < max_num_of_entries; num_of_entries += 100)
    {
        Rbt *tree;

        int64_t *arr;
        size_t size = num_of_entries;

        int64_t *rarr;
        size_t rsize;

        int64_t val;

        arr = (int64_t *)malloc(sizeof(int64_t) * size);
        T_ERROR(arr == NULL);

        for (size_t i = 0; i < size; ++i)
            arr[i] = (int64_t)(i + 1);

        for (size_t i = 0; i < size; ++i)
        {
            size_t index = (size_t)rand() % (size - 1);
            SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
        }

        tree = rbt_create(sizeof(int64_t), my_compare_int64_t, NULL, NULL);
        T_ERROR(tree == NULL);
        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

        for (size_t i = 0; i < size; ++i)
            T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 0);

        qsort((void *)&arr[0], size, sizeof(int64_t), my_compare_int64_t);

        T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);
        T_CHECK(size == rsize);
        T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], size), 0);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
        T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

        for (size_t i = 0; i < size; ++i)
            T_EXPECT(rbt_key_exist(tree, (void *)&arr[i]), (bool)true);

        for (size_t i = 0; i < size; ++i)
        {
            val = arr[i] + (int64_t)size;
            T_EXPECT(rbt_key_exist(tree, (void *)&val), (bool)false);
        }

        FREE(arr);
        FREE(rarr);
        rbt_destroy(tree);
    }
}


static void test_rbt_delete(void)
{
    const size_t max_num_of_entries = 1000;

    for (size_t num_of_entries = 100; num_of_entries < max_num_of_entries; num_of_entries += 100)
    {
        Rbt *tree;

        int64_t *arr;
        size_t size = num_of_entries;

        int64_t *rarr;
        size_t rsize;

        arr = (int64_t *)malloc(sizeof(int64_t) * size);
        T_ERROR(arr == NULL);

        for (size_t i = 0; i < size; ++i)
            arr[i] = (int64_t)(i + 1);

        for (size_t i = 0; i < size; ++i)
        {
            size_t index = (size_t)rand() % (size - 1);
            SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
        }

        tree = rbt_create(sizeof(int64_t), my_compare_int64_t, NULL, NULL);
        T_ERROR(tree == NULL);
        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

        for (size_t i = 0; i < size; ++i)
            T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 0);

        qsort((void *)&arr[0], size, sizeof(int64_t), my_compare_int64_t);

        T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);
        T_CHECK(size == rsize);
        T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], size), 0);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
        T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

        for (size_t i = 0; i < size >> 2; ++i)
        {
            T_EXPECT(rbt_delete(tree, (void *)&arr[i]), 0);
            T_EXPECT(rbt_delete(tree, (void *)&arr[i + (size >> 1) + (size >> 2)]), 0);            
        }

        FREE(rarr);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)(size >> 1));

        T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);

        T_CHECK((size >> 1) == rsize);
        T_EXPECT(memcmp((const void *)&arr[0 + (size >> 2)], (const void *)&rarr[0], size >> 1), 0);

        FREE(rarr);
        FREE(arr);
        rbt_destroy(tree);
    }
}


static void test_rbt_delete_the_same(void)
{
    const size_t max_num_of_entries = 1000;

    for (size_t num_of_entries = 100; num_of_entries < max_num_of_entries; num_of_entries += 100)
    {
        Rbt *tree;

        int64_t *arr;
        size_t size = num_of_entries;

        int64_t *rarr;
        size_t rsize;

        arr = (int64_t *)malloc(sizeof(int64_t) * size);
        T_ERROR(arr == NULL);

        for (size_t i = 0; i < size; ++i)
            arr[i] = (int64_t)(i + 1);

        for (size_t i = 0; i < size; ++i)
        {
            size_t index = (size_t)rand() % (size - 1);
            SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
        }

        tree = rbt_create(sizeof(int64_t), my_compare_int64_t, NULL, NULL);
        T_ERROR(tree == NULL);
        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

        for (size_t i = 0; i < size; ++i)
            T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 0);

        qsort((void *)&arr[0], size, sizeof(int64_t), my_compare_int64_t);

        T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);
        T_CHECK(size == rsize);
        T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], size), 0);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
        T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

        for (size_t i = 0; i < size >> 2; ++i)
        {
            T_EXPECT(rbt_delete(tree, (void *)&arr[i]), 0);
            T_EXPECT(rbt_delete(tree, (void *)&arr[i + (size >> 1) + (size >> 2)]), 0);            
        }

        for (size_t i = 0; i < size >> 2; ++i)
        {
            T_EXPECT(rbt_delete(tree, (void *)&arr[i]), -1);
            T_EXPECT(rbt_delete(tree, (void *)&arr[i + (size >> 1) + (size >> 2)]), -1);            
        }

        FREE(rarr);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int64_t));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)(size >> 1));

        T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);

        T_CHECK((size >> 1) == rsize);
        T_EXPECT(memcmp((const void *)&arr[0 + (size >> 2)], (const void *)&rarr[0], size >> 1), 0);

        FREE(rarr);
        FREE(arr);
        rbt_destroy(tree);
    }
}


static void test_rbt_delete_with_entries(void)
{
    const size_t max_num_of_entries = 1000;

    for (size_t num_of_entries = 100; num_of_entries < max_num_of_entries; num_of_entries += 100)
    {
        Rbt *tree;

        int64_t *arr;
        size_t size;

        MyStruct *ms;
        MyStruct fake_struct;
        MyStruct *fake_ptr = &fake_struct;

        MyStruct **rms;
        int64_t *keys;
        size_t rsize;

        size = num_of_entries;

        arr = (int64_t *)malloc(sizeof(int64_t) * size);
        T_ERROR(arr == NULL);

        for (size_t i = 0; i < size; ++i)
            arr[i] = (int64_t)(i + 1);

        for (size_t i = 0; i < size; ++i)
        {
            size_t index = (size_t)rand() % (size - 1);
            SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
        }

        tree = rbt_create(sizeof(MyStruct *), my_struct_compare, my_struct_destroy, NULL);
        T_ERROR(tree == NULL);
        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(MyStruct *));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

        for (size_t i = 0; i < size; ++i)
        {
            ms = my_struct_create(arr[i]);
            T_EXPECT(rbt_insert(tree, (void *)&ms), 0);
        }

        qsort((void *)&arr[0], size, sizeof(int64_t), my_compare_int64_t);

        T_EXPECT(rbt_to_array(tree, (void *)&rms, &rsize), 0);
        T_CHECK(size == rsize);

        keys = (int64_t *)malloc(sizeof(int64_t) * rsize);
        T_ERROR(keys == NULL);

        for (size_t i = 0; i < rsize; ++i)
            keys[i] = rms[i]->key;

        T_EXPECT(memcmp((const void *)&arr[0], (const void *)&keys[0], size), 0);

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(MyStruct *));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
        T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

        FREE(rms);
        FREE(keys);

        for (size_t i = 0; i < size >> 2; ++i)
        {
            fake_ptr->key = arr[i];
            T_EXPECT(rbt_delete_with_entry(tree, (void *)&fake_ptr), 0);

            fake_ptr->key = arr[i + (size >> 1) + (size >> 2)];
            T_EXPECT(rbt_delete_with_entry(tree, (void *)&fake_ptr), 0);
        }

        T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(MyStruct *));
        T_EXPECT(rbt_get_num_entries(tree), (ssize_t)(size >> 1));

        T_EXPECT(rbt_to_array(tree, (void *)&rms, &rsize), 0);
        T_CHECK(size >> 1 == rsize);

        keys = (int64_t *)malloc(sizeof(int64_t) * rsize);
        T_ERROR(keys == NULL);

        for (size_t i = 0; i < rsize; ++i)
            keys[i] = rms[i]->key;

        T_EXPECT(memcmp((const void *)&arr[0 + (size >> 2)], (const void *)&keys[0], size >> 1), 0);

        FREE(rms);
        FREE(keys);
        FREE(arr);

        rbt_destroy_with_entries(tree);
    }
}


int main(void)
{
    TEST_INIT("TESTING RED BLACK TREE");
    TEST(test_rbt_create());
    TEST(test_rbt_insert_only_root());
    TEST(test_rbt_insert());
    TEST(test_rbt_insert_the_same());
    TEST(test_rbt_destroy_with_entries());
    TEST(test_rbt_min_max());
    TEST(test_rbt_search());
    TEST(test_rbt_key_exist());
    TEST(test_rbt_delete());
    TEST(test_rbt_delete_the_same());
    TEST(test_rbt_delete_with_entries());
    TEST_SUMMARY();

	return 0;
}