#include <list.h>
#include <ctest.h>
#include <stdint.h>
#include <assert.h>


#define ARRAY_TEST_SIZE 100


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

    if (s == NULL)
        return NULL;

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


static int my_compare_double(const void *a, const void *b)
{
    assert(a != NULL && b != NULL);

    const double *ia = (const double *)a;
    const double *ib = (const double *)b;

    if (*ia > *ib) return 1;
    if (*ia == *ib) return 0;
    return -1;
}


static void test_list_create_destroy_empty(void)
{
    List *list = NULL;

    list = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(int64_t));
    list_destroy(list);

    list = list_create(sizeof(double), my_compare_double, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(double));
    list_destroy(list);

    list = list_create(0, NULL, NULL);
    T_ASSERT(list, NULL);
    
    list = list_create(0, my_compare_int64_t, NULL);
    T_ASSERT(list, NULL);

    list = list_create(sizeof(double), NULL, NULL);
    T_ASSERT(list, NULL);
}


static void test_list_insert(void)
{
    List *list = NULL;
    int64_t val = 0;
    size_t size = 0;
    int64_t* rarr = NULL;
    size_t rsize = 0;

    /* scenario when we insert only first element */
    list = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(int64_t));

    size = 1;
    val = 13;

    T_EXPECT(list_insert(list, (const void * const)&val), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)size);
    T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list));
    T_EXPECT(memcmp((const void *)rarr, (const void *)&val, rsize), 0);

    FREE(rarr);
    list_destroy(list);

    /* scenario when we insert only at the end of list */
    list = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(int64_t));

    size = ARRAY_TEST_SIZE;
    int64_t arr[ARRAY_TEST_SIZE] = {0};

    for (size_t i = 0; i < size; ++i)
        arr[i] = (int64_t)(i + 1);

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (const void * const)&arr[i]), 0);

    T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)size);
    T_ASSERT(size, rsize);
    T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], rsize), 0);

    FREE(rarr);
    list_destroy(list);

    /* scenario when we insert in the middle */
    list = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(int64_t));

    size = ARRAY_TEST_SIZE;

    for (size_t i = 0; i < size; ++i)
        arr[i] = (int64_t)(i + 1);

    for (size_t i = 0; i < size; ++i)
    {
        size_t index = (size_t)rand() % (size - 1);
        SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
    }

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (const void * const)&arr[i]), 0);

    T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)size);
    T_ASSERT(size, rsize);

    for (size_t i = 0; i < size; ++i)
        arr[i] = (int64_t)(i + 1);

    T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr[0], rsize), 0);

    FREE(rarr);
    list_destroy(list);
}


static void test_list_delete(void)
{
    List *list = NULL;
    int64_t del = 0;
    int64_t val = 0;
    size_t size = 0;
    int64_t* rarr = NULL;
    size_t rsize = 0;

    /* scenario when we insert only first element and we delete it */
    list = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(int64_t));

    size = 1;
    val = 13;
    del = 13;

    T_EXPECT(list_insert(list, (const void * const)&val), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)size);
    T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list));
    T_EXPECT(memcmp((const void *)rarr, (const void *)&val, rsize), 0);

    size = 0;

    T_EXPECT(list_delete(list, (const void * const)&del), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)size);

    FREE(rarr);
    list_destroy(list);

    /* scenario when we insert many entries and we delete always from head */
    list = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(int64_t));

    size = ARRAY_TEST_SIZE;
    int64_t arr[ARRAY_TEST_SIZE] = {0};
    int64_t del_arr[ARRAY_TEST_SIZE] = {0};

    for (size_t i = 0; i < size; ++i)
    {
        arr[i] = (int64_t)(i + 1);
        del_arr[i] = arr[i];
    }

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (const void * const)&arr[i]), 0);

    T_EXPECT(list_get_num_entries(list), (ssize_t)size);
    T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list));
    T_EXPECT(memcmp((const void *)rarr, (const void *)&arr[0], rsize), 0);
    FREE(rarr);

    for (size_t i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete(list, (const void * const)&del_arr[i]), 0);
        T_EXPECT(list_get_num_entries(list), (ssize_t)(size - i - 1));
        T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
        T_ASSERT(rsize, (size_t)list_get_num_entries(list));
        T_EXPECT(memcmp((const void *)rarr, (const void *)&arr[i + 1], rsize), 0);
        FREE(rarr);
    }

    T_EXPECT(list_delete(list, (const void * const)&del_arr[size - 1]), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);

    list_destroy(list);

    /* scenario when we insert many entries and we delete always from tail */
    list = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(int64_t));

    size = ARRAY_TEST_SIZE;
    int64_t arr2[ARRAY_TEST_SIZE] = {0};
    int64_t del_arr2[ARRAY_TEST_SIZE] = {0};

    for (size_t i = 0; i < size; ++i)
    {
        arr2[i] = (int64_t)(i + 1);
        del_arr2[i] = arr2[i];
    }

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (const void * const)&arr2[i]), 0);

    T_EXPECT(list_get_num_entries(list), (ssize_t)size);
    T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list));
    T_EXPECT(memcmp((const void *)&rarr[0], (const void *)&arr2[0], rsize), 0);
    FREE(rarr);

    for (size_t i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete(list, (const void * const)&del_arr2[size - i - 1]), 0);
        T_EXPECT(list_get_num_entries(list), (ssize_t)(size - i - 1));
        T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
        T_ASSERT(rsize, (size_t)list_get_num_entries(list));
        T_EXPECT(memcmp((const void *)&rarr[0], (const void *)&arr2[0], rsize), 0);
        FREE(rarr);
    }

    T_EXPECT(list_delete(list, (const void * const)&del_arr2[0]), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);

    list_destroy(list);

    /* scenario when we insert many entries and we delete usually from middle */
    list = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(int64_t));

    size = ARRAY_TEST_SIZE;
    int64_t arr3[ARRAY_TEST_SIZE] = {0};
    int64_t del_arr3[ARRAY_TEST_SIZE] = {0};

    for (size_t i = 0; i < size; ++i)
    {
        arr3[i] = (int64_t)(i + 1);
        del_arr3[i] = arr3[i];
    }

    for (size_t i = 0; i < size; ++i)
    {
        size_t index = (size_t)rand() % (size - 1);
        SWAP(*(BYTE *)&del_arr3[index], *(BYTE *)&del_arr3[size - i - 1], sizeof(int64_t));
    }

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (const void * const)&arr2[i]), 0);

    T_EXPECT(list_get_num_entries(list), (ssize_t)size);
    T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list));
    T_EXPECT(memcmp((const void *)&rarr[0], (const void *)&arr2[0], rsize), 0);
    FREE(rarr);

    for (size_t i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete(list, (const void * const)&del_arr3[i]), 0);
        T_EXPECT(list_get_num_entries(list), (ssize_t)(size - i - 1));
    }

    T_EXPECT(list_delete(list, (const void * const)&del_arr3[size - 1]), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);

    list_destroy(list);

    /* scenario when all entries is hardcoded */
    list = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(int64_t));

    size = 10;

    int64_t arr4[] = { 7, 2, 1, 1, 4, 3, 3, 5, 9, 0 };
    int64_t expt_arr4[] = { 0, 1, 1, 2, 3, 3, 4, 5, 7, 9 };

    int64_t del_arr4[] = { 7, 0, 9, 1, 2, 1, 4, 3, 3, 5 };
    int64_t expt_del_arr4[ARRAY_SIZE(arr4)][ARRAY_SIZE(arr4)] =
    {
        { 0, 1, 1, 2, 3, 3, 4, 5, 9 },
        { 1, 1, 2, 3, 3, 4, 5, 9 },
        { 1, 1, 2, 3, 3, 4, 5 },
        { 1, 2, 3, 3, 4, 5 },
        { 1, 3, 3, 4, 5 },
        { 3, 3, 4, 5 }, 
        { 3, 3, 5 },
        { 3, 5 },
        { 5, },
        { 0 }
    };

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (const void * const)&arr4[i]), 0);

    T_EXPECT(list_get_num_entries(list), (ssize_t)size);
    T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list));
    T_EXPECT(memcmp((const void *)&rarr[0], (const void *)&expt_arr4[0], rsize), 0);
    FREE(rarr);

    for (size_t i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete(list, (const void * const)&del_arr4[i]), 0);
        T_EXPECT(list_get_num_entries(list), (ssize_t)(size - i - 1));
        T_EXPECT(list_to_array(list, &rarr, &rsize), 0);
        T_ASSERT(rsize, (size_t)list_get_num_entries(list));
        T_EXPECT(memcmp((const void *)&rarr[0], (const void *)expt_del_arr4[i], rsize), 0);
        FREE(rarr);
    }

    T_EXPECT(list_delete(list, (const void * const)&del_arr4[size - 1]), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);

    list_destroy(list);
}


static void test_list_delete_with_entry(void)
{
    int64_t arr[] = { 7, 2, 1, 1, 4, 3, 3, 5, 9, 0 };
    int64_t expt_arr[] = { 0, 1, 1, 2, 3, 3, 4, 5, 7, 9 };

    int64_t del_arr[] = { 7, 0, 9, 1, 2, 1, 4, 3, 3, 5 };
    int64_t expt_del_arr[ARRAY_SIZE(arr)][ARRAY_SIZE(arr)] =
    {
        { 0, 1, 1, 2, 3, 3, 4, 5, 9 },
        { 1, 1, 2, 3, 3, 4, 5, 9 },
        { 1, 1, 2, 3, 3, 4, 5 },
        { 1, 2, 3, 3, 4, 5 },
        { 1, 3, 3, 4, 5 },
        { 3, 3, 4, 5 }, 
        { 3, 3, 5 },
        { 3, 5 },
        { 5, },
        { 0 }
    };

    int64_t *keys = NULL;
    MyStruct *my_struct = NULL;
    MyStruct fake_struct = {0};
    MyStruct *fake_ptr = &fake_struct;
    MyStruct **rfake_ptr = NULL;

    const size_t size = ARRAY_SIZE(arr);
    size_t rsize = 0;

    List *list = list_create(sizeof(MyStruct *), my_struct_compare, my_struct_destroy);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), (ssize_t)0);
    T_EXPECT(list_get_data_size(list), (ssize_t)sizeof(MyStruct *));

    for (size_t i = 0; i < size; ++i)
    {
        my_struct = my_struct_create(arr[i]);
        T_ERROR(my_struct == NULL);
        T_EXPECT(list_insert(list, (void *)&my_struct), 0);
    }

    T_EXPECT(list_get_num_entries(list), (ssize_t)size);
    T_EXPECT(list_to_array(list, (void *)&rfake_ptr, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list));
    T_ASSERT(rsize, size);

    keys = (int64_t *)malloc(sizeof(int64_t) * rsize);
    T_ERROR(keys == NULL);
    
    for (size_t i = 0; i < size; ++i)   
        keys[i] = rfake_ptr[i]->key;

    T_EXPECT(memcmp((const void *)&keys[0], (const void *)&expt_arr[0], rsize), 0);

    FREE(rfake_ptr);
    FREE(keys);

    for (size_t i = 0; i < size - 1; ++i)
    {
        fake_ptr->key = del_arr[i];

        T_EXPECT(list_delete_with_entry(list, (const void * const)&fake_ptr), 0);
        T_EXPECT(list_get_num_entries(list), (ssize_t)(size - i - 1));
        T_EXPECT(list_to_array(list, &rfake_ptr, &rsize), 0);
        T_ASSERT(rsize, (size_t)list_get_num_entries(list));
        T_ASSERT(rsize, (size - i - 1));

        keys = (int64_t *)malloc(sizeof(int64_t) * rsize);
        T_ERROR(keys == NULL);
    
        for (size_t j = 0; j < rsize; ++j)   
            keys[j] = rfake_ptr[j]->key;

        T_EXPECT(memcmp((const void *)&keys[0], (const void *)expt_del_arr[i], rsize), 0);

        FREE(rfake_ptr);
        FREE(keys);
    }

    fake_ptr->key = del_arr[size - 1];
    T_EXPECT(list_delete_with_entry(list, (const void * const)&fake_ptr), 0);
    T_EXPECT(list_get_num_entries(list), (ssize_t)(0));

    list_destroy_with_entries(list);
}


int main(void)
{
    TEST_INIT("TESTING LINKED LIST");
    TEST(test_list_create_destroy_empty());
    TEST(test_list_insert());
    TEST(test_list_delete());
    TEST(test_list_delete_with_entry());
    TEST_SUMMARY();

    return 0;
}