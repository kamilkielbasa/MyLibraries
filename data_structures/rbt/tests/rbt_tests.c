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

static int my_struct_compare(const void *a, const void *b)
{
    assert(a != NULL && b != NULL);
    const MyStruct *ia = (const MyStruct *)a;
    const MyStruct *ib = (const MyStruct *)b;

    if (ia->key > ib->key) return 1;
    if (ia->key == ib->key) return 0;
	return -1;
}

static int my_compare_int(const void *a, const void *b)
{
    assert(a != NULL && b != NULL);

    const int *ia = (const int *)a;
    const int *ib = (const int *)b;

    if (*ia > *ib) return 1;
    if (*ia == *ib) return 0;
    return -1;
}

static void my_struct_destroy(void *s)
{
    MyStruct *ms = *(MyStruct **)s;
    FREE(ms);
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

    tree = rbt_create(sizeof(int), my_struct_compare, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);
    T_EXPECT(rbt_get_height(tree), 0);
    rbt_destroy(tree);

    tree = rbt_create(sizeof(long), my_struct_compare, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(long));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);
    T_EXPECT(rbt_get_height(tree), 0);
    rbt_destroy(tree);

    tree = rbt_create(sizeof(MyStruct), my_struct_compare, my_struct_destroy);
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

    tree = rbt_create(sizeof(MyStruct), my_struct_compare, NULL);
    T_EXPECT(rbt_insert(tree, &my_struct), 0);
    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(MyStruct));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)1);
    T_EXPECT(rbt_get_height(tree), 1);
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
    Rbt *tree;

    int *arr;
    size_t size = 10;

    int *rarr;
    size_t rsize;

    arr = (int *)malloc(sizeof(int) * size);
    T_ERROR(arr == NULL);

    for (size_t i = 0; i < size; ++i)
        arr[i] = (int)(i + 1);

    for (size_t i = 0; i < size; ++i)
    {
        size_t index = (size_t)rand() % (size - 1);
        SWAP(*(uint8_t *)&arr[index], *(uint8_t *)&arr[size - i - 1], sizeof(int));
    }

    tree = rbt_create(sizeof(int), my_compare_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 0);

    qsort((void *)&arr[0], size, sizeof(int), my_compare_int);

    T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);
    T_CHECK(size == rsize);
    T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], size), 0);

    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
    T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

    FREE(arr);
    FREE(rarr);
    // rbt_destroy(tree);
    rbt_rek_destroy(tree);
}

static void test_rbt_insert_the_same(void)
{
    Rbt *tree;

    int *arr;
    size_t size = 30;

    int *rarr;
    size_t rsize;

    arr = (int *)malloc(sizeof(int) * size);
    T_ERROR(arr == NULL);

    for (size_t i = 0; i < size; ++i)
        arr[i] = (int)(i + 1);

    for (size_t i = 0; i < size; ++i)
    {
        size_t index = (size_t)rand() % (size - 1);
        SWAP(*(uint8_t *)&arr[index], *(uint8_t *)&arr[size - i - 1], sizeof(int));
    }

    tree = rbt_create(sizeof(int), my_compare_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)0);

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 0);

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(rbt_insert(tree, (void *)&arr[i]), 1);

    qsort((void *)&arr[0], size, sizeof(int), my_compare_int);

    T_EXPECT(rbt_to_array(tree, (void *)&rarr, &rsize), 0);
    T_CHECK(size == rsize);
    T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], size), 0);

    T_EXPECT(rbt_get_data_size(tree), (ssize_t)sizeof(int));
    T_EXPECT(rbt_get_num_entries(tree), (ssize_t)size);
    T_EXPECT(correct_height(rbt_get_height(tree), (size_t)rbt_get_num_entries(tree)), (bool)true);

    FREE(arr);
    FREE(rarr);
    // rbt_destroy(tree);
    rbt_rek_destroy(tree);
}

int main(void)
{
	TEST_INIT("TESTING RED BLACK TREE");
    TEST(test_rbt_create());
    TEST(test_rbt_insert_only_root());
    TEST(test_rbt_insert());
    TEST(test_rbt_insert_the_same());
	TEST_SUMMARY();

	return 0;
}