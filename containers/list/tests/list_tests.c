#include <list.h>
#include <ctest.h>
#include <stdint.h>
#include <assert.h>


typedef struct MyStruct
{
    int64_t key;
    int64_t a;
    int64_t b;
    int64_t c;
} MyStruct;


/* functions needed for testing */
static ___inline___ MyStruct* my_struct_create(const int64_t key);
static void my_struct_destroy(void* ptr_p);
static int my_struct_compare(const void* a_p, const void* b_p);
static int my_compare_int64_t(const void* a_p, const void* b_p);


/* unit tests function declaraions */
static void test_list_create_destroy_empty(void);
static void test_list_insert(void);
static void test_list_delete(void);
static void test_list_delete_with_entry(void);
static void test_list_delete_all(void);
static void test_list_delete_all_with_entries(void);
static void test_list_search(void);


/* implementation */
static ___inline___ MyStruct* my_struct_create(const int64_t key)
{
    MyStruct* ms_p = (MyStruct*)malloc(sizeof(*ms_p));

    if (ms_p == NULL)
        return NULL;

    ms_p->key = key;
    ms_p->a = 0;
    ms_p->b = 0;
    ms_p->c = 0;

    return ms_p;
}


static void my_struct_destroy(void* ptr_p)
{
    MyStruct* ms_p = *(MyStruct**)ptr_p;
    FREE(ms_p);
}


static int my_struct_compare(const void* a_p, const void* b_p)
{
    assert(a_p != NULL && b_p != NULL);

    const MyStruct* ms1_p = *(const MyStruct**)a_p;
    const MyStruct* ms2_p = *(const MyStruct**)b_p;

    if (ms1_p->key > ms2_p->key) return 1;
    if (ms1_p->key == ms2_p->key) return 0;
	return -1;
}


static int my_compare_int64_t(const void* a_p, const void* b_p)
{
    assert(a_p != NULL && b_p != NULL);

    const int64_t *iptr1_p = (const int64_t*)a_p;
    const int64_t *iptr2_p = (const int64_t*)b_p;

    if (*iptr1_p > *iptr2_p) return 1;
    if (*iptr1_p == *iptr2_p) return 0;
    return -1;
}


static void test_list_create_destroy_empty(void)
{
    List *list_p = NULL;

    list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));
    list_destroy(list_p);

    list_p = list_create(0, NULL, NULL);
    T_ASSERT(list_p, NULL);
    
    list_p = list_create(0, my_compare_int64_t, NULL);
    T_ASSERT(list_p, NULL);

    list_p = list_create(sizeof(double), NULL, NULL);
    T_ASSERT(list_p, NULL);
}


static void test_list_insert(void)
{
    #define ARRAY_TEST_SIZE 100

    List* list_p = NULL;
    int64_t val = 0;
    size_t size = 0;
    int64_t* rarr_p = NULL;
    size_t rsize = 0;

    /* scenario when we insert only first element */
    list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

    size = 1;
    val = 13;

    T_EXPECT(list_insert(list_p, (const void * const)&val), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
    T_EXPECT(memcmp((const void *)rarr_p, (const void *)&val, rsize), 0);

    FREE(rarr_p);
    list_destroy(list_p);

    /* scenario when we insert only at the end of list_p */
    list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

    size = ARRAY_TEST_SIZE;
    int64_t arr[ARRAY_TEST_SIZE] = {0};

    for (size_t i = 0; i < size; ++i)
        arr[i] = (int64_t)(i + 1);

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list_p, (const void * const)&arr[i]), 0);

    T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_ASSERT(size, rsize);
    T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr_p[0], rsize), 0);

    FREE(rarr_p);
    list_destroy(list_p);

    /* scenario when we insert in the middle */
    list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

    size = ARRAY_TEST_SIZE;

    for (size_t i = 0; i < size; ++i)
        arr[i] = (int64_t)(i + 1);

    for (size_t i = 0; i < size; ++i)
    {
        size_t index = (size_t)rand() % (size - 1);
        SWAP(*(BYTE *)&arr[index], *(BYTE *)&arr[size - i - 1], sizeof(int64_t));
    }

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list_p, (const void * const)&arr[i]), 0);

    T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_ASSERT(size, rsize);

    for (size_t i = 0; i < size; ++i)
        arr[i] = (int64_t)(i + 1);

    T_EXPECT(memcmp((const void *)&arr[0], (const void *)&rarr_p[0], rsize), 0);

    FREE(rarr_p);
    list_destroy(list_p);

    #undef ARRAY_TEST_SIZE
}


static void test_list_delete(void)
{
    #define ARRAY_TEST_SIZE 100

    List *list_p = NULL;
    int64_t del = 0;
    int64_t val = 0;
    size_t size = 0;
    int64_t* rarr_p = NULL;
    size_t rsize = 0;

    /* scenario when we insert only first element and we delete it */
    list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

    size = 1;
    val = 13;
    del = 13;

    T_EXPECT(list_insert(list_p, (const void * const)&val), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
    T_EXPECT(memcmp((const void *)rarr_p, (const void *)&val, rsize), 0);

    size = 0;

    T_EXPECT(list_delete(list_p, (const void * const)&del), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);

    FREE(rarr_p);
    list_destroy(list_p);

    /* scenario when we insert many entries and we delete always from head */
    list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

    size = ARRAY_TEST_SIZE;
    int64_t arr[ARRAY_TEST_SIZE] = {0};
    int64_t del_arr[ARRAY_TEST_SIZE] = {0};

    for (size_t i = 0; i < size; ++i)
    {
        arr[i] = (int64_t)(i + 1);
        del_arr[i] = arr[i];
    }

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list_p, (const void * const)&arr[i]), 0);

    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
    T_EXPECT(memcmp((const void *)rarr_p, (const void *)&arr[0], rsize), 0);
    FREE(rarr_p);

    for (size_t i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete(list_p, (const void * const)&del_arr[i]), 0);
        T_EXPECT(list_get_num_entries(list_p), (ssize_t)(size - i - 1));
        T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
        T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
        T_EXPECT(memcmp((const void *)rarr_p, (const void *)&arr[i + 1], rsize), 0);
        FREE(rarr_p);
    }

    T_EXPECT(list_delete(list_p, (const void * const)&del_arr[size - 1]), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);

    list_destroy(list_p);

    /* scenario when we insert many entries and we delete always from tail */
    list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

    size = ARRAY_TEST_SIZE;
    int64_t arr2[ARRAY_TEST_SIZE] = {0};
    int64_t del_arr2[ARRAY_TEST_SIZE] = {0};

    for (size_t i = 0; i < size; ++i)
    {
        arr2[i] = (int64_t)(i + 1);
        del_arr2[i] = arr2[i];
    }

    for (size_t i = 0; i < size; ++i)
        T_EXPECT(list_insert(list_p, (const void * const)&arr2[i]), 0);

    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
    T_EXPECT(memcmp((const void *)&rarr_p[0], (const void *)&arr2[0], rsize), 0);
    FREE(rarr_p);

    for (size_t i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete(list_p, (const void * const)&del_arr2[size - i - 1]), 0);
        T_EXPECT(list_get_num_entries(list_p), (ssize_t)(size - i - 1));
        T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
        T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
        T_EXPECT(memcmp((const void *)&rarr_p[0], (const void *)&arr2[0], rsize), 0);
        FREE(rarr_p);
    }

    T_EXPECT(list_delete(list_p, (const void * const)&del_arr2[0]), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);

    list_destroy(list_p);

    /* scenario when we insert many entries and we delete usually from middle */
    list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

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
        T_EXPECT(list_insert(list_p, (const void * const)&arr2[i]), 0);

    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
    T_EXPECT(memcmp((const void *)&rarr_p[0], (const void *)&arr2[0], rsize), 0);
    FREE(rarr_p);

    for (size_t i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete(list_p, (const void * const)&del_arr3[i]), 0);
        T_EXPECT(list_get_num_entries(list_p), (ssize_t)(size - i - 1));
    }

    T_EXPECT(list_delete(list_p, (const void * const)&del_arr3[size - 1]), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);

    list_destroy(list_p);

    /* scenario when all entries is hardcoded */
    list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

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
        T_EXPECT(list_insert(list_p, (const void * const)&arr4[i]), 0);

    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
    T_EXPECT(memcmp((const void *)&rarr_p[0], (const void *)&expt_arr4[0], rsize), 0);
    FREE(rarr_p);

    for (size_t i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete(list_p, (const void * const)&del_arr4[i]), 0);
        T_EXPECT(list_get_num_entries(list_p), (ssize_t)(size - i - 1));
        T_EXPECT(list_to_array(list_p, &rarr_p, &rsize), 0);
        T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
        T_EXPECT(memcmp((const void *)&rarr_p[0], (const void *)expt_del_arr4[i], rsize), 0);
        FREE(rarr_p);
    }

    T_EXPECT(list_delete(list_p, (const void * const)&del_arr4[size - 1]), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);

    list_destroy(list_p);

    #undef ARRAY_TEST_SIZE
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

    int64_t *keys_p = NULL;
    MyStruct *my_struct_p = NULL;
    MyStruct fake_struct = {0};
    MyStruct *fake_ptr_p = &fake_struct;
    MyStruct **rfake_ptr_pp = NULL;

    const size_t size = ARRAY_SIZE(arr);
    size_t rsize = 0;

    List *list_p = list_create(sizeof(MyStruct *), my_struct_compare, my_struct_destroy);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(MyStruct *));

    for (size_t i = 0; i < size; ++i)
    {
        my_struct_p = my_struct_create(arr[i]);
        T_ERROR(my_struct_p == NULL);
        T_EXPECT(list_insert(list_p, (void *)&my_struct_p), 0);
    }

    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_EXPECT(list_to_array(list_p, (void *)&rfake_ptr_pp, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
    T_ASSERT(rsize, size);

    keys_p = (int64_t *)malloc(sizeof(int64_t) * rsize);
    T_ERROR(keys_p == NULL);
    
    for (size_t i = 0; i < size; ++i)   
        keys_p[i] = rfake_ptr_pp[i]->key;

    T_EXPECT(memcmp((const void *)&keys_p[0], (const void *)&expt_arr[0], rsize), 0);

    FREE(rfake_ptr_pp);
    FREE(keys_p);

    for (size_t i = 0; i < size - 1; ++i)
    {
        fake_ptr_p->key = del_arr[i];

        T_EXPECT(list_delete_with_entry(list_p, (const void * const)&fake_ptr_p), 0);
        T_EXPECT(list_get_num_entries(list_p), (ssize_t)(size - i - 1));
        T_EXPECT(list_to_array(list_p, &rfake_ptr_pp, &rsize), 0);
        T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
        T_ASSERT(rsize, (size - i - 1));

        keys_p = (int64_t *)malloc(sizeof(int64_t) * rsize);
        T_ERROR(keys_p == NULL);
    
        for (size_t j = 0; j < rsize; ++j)   
            keys_p[j] = rfake_ptr_pp[j]->key;

        T_EXPECT(memcmp((const void *)&keys_p[0], (const void *)expt_del_arr[i], rsize), 0);

        FREE(rfake_ptr_pp);
        FREE(keys_p);
    }

    fake_ptr_p->key = del_arr[size - 1];
    T_EXPECT(list_delete_with_entry(list_p, (const void * const)&fake_ptr_p), 0);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)(0));

    list_destroy_with_entries(list_p);
}


static void test_list_delete_all(void)
{
    /* after inserted: [0, 0, 1, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9] */
    int64_t arr[] = { 9, 0, 0, 8, 1, 7, 7, 2, 6, 3, 4, 5, 5 };
    int64_t arr_expt[] = { 0, 0, 1, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9 };
    int64_t del_arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1 };
    int64_t deleted_entries[] = { 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, -1 };
    int64_t sizes[] = { 11, 10, 9, 8, 7, 5, 4, 2, 1, 0, 0 };

    size_t i = 0;
    size_t rsize = 0;
    int64_t* rarr_p = NULL;

    int64_t expt_del_arr[ARRAY_SIZE(arr)][ARRAY_SIZE(arr)] =
    {
       { 1, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9 },
       { 2, 3, 4, 5, 5, 6, 7, 7, 8, 9 },
       { 3, 4, 5, 5, 6, 7, 7, 8, 9 },
       { 4, 5, 5, 6, 7, 7, 8, 9 },
       { 5, 5, 6, 7, 7, 8, 9 },
       { 6, 7, 7, 8, 9 },
       { 7, 7, 8, 9 },
       { 8, 9 },
       { 9 },
    };

    List *list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

    for (i = 0; i < ARRAY_SIZE(arr); ++i)
        T_EXPECT(list_insert(list_p, (void *)&arr[i]), 0);

    T_EXPECT(list_get_num_entries(list_p), (ssize_t)ARRAY_SIZE(arr));
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));
    T_EXPECT(list_to_array(list_p, (void *)&rarr_p, &rsize), 0);
    T_ASSERT(ARRAY_SIZE(arr), rsize);
    T_EXPECT(memcmp((const void *)&rarr_p[0], (const void *)&arr_expt[0], rsize), 0);
    FREE(rarr_p);

    for (i = 0; i < ARRAY_SIZE(del_arr) - 2; ++i)
    {
        T_EXPECT(list_delete_all(list_p, &del_arr[i]), (int)deleted_entries[i]);
        T_EXPECT(list_get_num_entries(list_p), (ssize_t)sizes[i]);
        T_EXPECT(list_to_array(list_p, (void *)&rarr_p, &rsize), 0);
        T_ASSERT(rsize, (size_t)sizes[i]);
        T_EXPECT(memcmp((const void *)&rarr_p[0], (const void *)expt_del_arr[i], (size_t)sizes[i]), 0);
        FREE(rarr_p);
    }

    T_EXPECT(list_delete_all(list_p, &del_arr[i]), (int)deleted_entries[i]);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)sizes[i]);

    ++i;
    T_EXPECT(list_delete_all(list_p, &del_arr[i]), (int)deleted_entries[i]);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)sizes[i]);

    list_destroy(list_p);
}


static void test_list_delete_all_with_entries(void)
{
    int64_t arr[] = { 9, 0, 0, 8, 1, 7, 7, 2, 6, 3, 4, 5, 5 };
    int64_t arr_expt[] = { 0, 0, 1, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9 };
    int64_t del_arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1 };
    int64_t deleted_entries[] = { 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, -1 };
    int64_t sizes[] = { 11, 10, 9, 8, 7, 5, 4, 2, 1, 0, 0 };

    size_t i = 0;
    const size_t size = ARRAY_SIZE(arr);
    size_t rsize = 0;

    int64_t *keys_p = NULL;
    MyStruct *my_struct_p = NULL;
    MyStruct fake_struct = {0};
    MyStruct *fake_ptr_p = &fake_struct;
    MyStruct **rfake_ptr_pp = NULL;

    int64_t expt_del_arr[ARRAY_SIZE(arr)][ARRAY_SIZE(arr)] =
    {
        { 1, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9 },
        { 2, 3, 4, 5, 5, 6, 7, 7, 8, 9 },
        { 3, 4, 5, 5, 6, 7, 7, 8, 9 },
        { 4, 5, 5, 6, 7, 7, 8, 9 },
        { 5, 5, 6, 7, 7, 8, 9 },
        { 6, 7, 7, 8, 9 },
        { 7, 7, 8, 9 },
        { 8, 9 },
        { 9 },
    }; 

    List *list_p = list_create(sizeof(MyStruct *), my_struct_compare, my_struct_destroy);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(MyStruct *));

    for (i = 0; i < size; ++i)
    {
        my_struct_p = my_struct_create(arr[i]);
        T_ERROR(my_struct_p == NULL);
        T_EXPECT(list_insert(list_p, (void *)&my_struct_p), 0);
    }

    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_EXPECT(list_to_array(list_p, (void *)&rfake_ptr_pp, &rsize), 0);
    T_ASSERT(rsize, (size_t)list_get_num_entries(list_p));
    T_ASSERT(rsize, size);

    keys_p = (int64_t *)malloc(sizeof(int64_t) * rsize);
    T_ERROR(keys_p == NULL);
    
    for (i = 0; i < size; ++i)   
        keys_p[i] = rfake_ptr_pp[i]->key;

    T_EXPECT(memcmp((const void *)&keys_p[0], (const void *)&arr_expt[0], rsize), 0);

    FREE(rfake_ptr_pp);
    FREE(keys_p);

    for (i = 0; i < ARRAY_SIZE(del_arr) - 2; ++i)
    {
        fake_ptr_p->key = del_arr[i];

        T_EXPECT(list_delete_all_with_entry(list_p, (const void* const)&fake_ptr_p), (int)deleted_entries[i]);
        T_EXPECT(list_get_num_entries(list_p), (ssize_t)sizes[i]);
        T_EXPECT(list_to_array(list_p, (void *)&rfake_ptr_pp, &rsize), 0);
        T_ASSERT(rsize, (size_t)sizes[i]);

        keys_p = (int64_t *)malloc(sizeof(int64_t) * rsize);
        T_ERROR(keys_p == NULL);

        for (size_t j = 0; j < rsize; ++j)
            keys_p[j] = rfake_ptr_pp[j]->key;

        T_EXPECT(memcmp(expt_del_arr[i], keys_p, (size_t)sizes[i]), 0);

        FREE(keys_p);
        FREE(rfake_ptr_pp);
    }

    fake_ptr_p->key = del_arr[i];
    T_EXPECT(list_delete_all_with_entry(list_p, (const void* const)&fake_ptr_p), (int)deleted_entries[i]);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)sizes[i]);

    ++i;
    fake_ptr_p->key = del_arr[i];
    T_EXPECT(list_delete_all_with_entry(list_p, (const void* const)&fake_ptr_p), (int)deleted_entries[i]);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)sizes[i]);

    list_destroy_with_entries(list_p);
}


static void test_list_search(void)
{
    int64_t arr[] = { 9, 0, 1, 8, 2, 7, 7, 6, 5, 3, 4 };
    int64_t arr_expt[] = { 0, 1, 2, 2, 3, 4, 5, 6, 7, 7, 8, 9 };
    int64_t* rarr_p = NULL;

    int64_t arr_nexpt[] = { 11, 13, 17, -3, -5, -7, 101, 103, 107, 111, 113 };

    int64_t val = 0;
    size_t rsize = 0;
    const size_t size = ARRAY_SIZE(arr);

    List *list_p = list_create(sizeof(int64_t), my_compare_int64_t, NULL);
    T_ERROR(list_p == NULL);
    T_EXPECT(list_get_num_entries(list_p), (ssize_t)0);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));

    for (size_t i = 0; i < ARRAY_SIZE(arr); ++i)
        T_EXPECT(list_insert(list_p, (void *)&arr[i]), 0);

    T_EXPECT(list_get_num_entries(list_p), (ssize_t)size);
    T_EXPECT(list_get_data_size(list_p), (ssize_t)sizeof(int64_t));
    T_EXPECT(list_to_array(list_p, (void *)&rarr_p, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(memcmp((const void *)&rarr_p[0], (const void *)&arr_expt[0], rsize), 0);
    FREE(rarr_p);

    for (size_t i = 0; i < size; ++i)
    {
        T_EXPECT(list_search(list_p, (void*)&arr[i], (void*)&val), 0);
        T_ASSERT(val, arr[i]);

        T_EXPECT(list_search(list_p, (void*)&arr_nexpt[i], (void*)&val), -1);
    }

    list_destroy(list_p);
}


int main(void)
{
    TEST_INIT("TESTING LINKED LIST");
    TEST(test_list_create_destroy_empty());
    TEST(test_list_insert());
    TEST(test_list_delete());
    TEST(test_list_delete_with_entry());
    TEST(test_list_delete_all());
    TEST(test_list_delete_all_with_entries());
    TEST(test_list_search());
    TEST_SUMMARY();

    return 0;
}