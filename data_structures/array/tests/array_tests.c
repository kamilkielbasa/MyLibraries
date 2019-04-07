#include <common.h>
#include <ctest.h>
#include <array.h>
#include <stdint.h>
#include <assert.h>


typedef struct MyStruct {
    int64_t key;
    int64_t a;
    int64_t b;
    int64_t c;
} MyStruct;


// static int my_struct_compare(const void *a, const void *b)
// {
//     assert(a != NULL);
//     assert(b != NULL);

//     if ((*(MyStruct *)a).key > (*(MyStruct *)b).key) return 1;
//     if ((*(MyStruct *)a).key == (*(MyStruct *)b).key) return 0;
//     return -1;
// }


static void my_struct_destroy(void *s)
{
    MyStruct *ms = *(MyStruct **)s;
    FREE(ms);
}


static MyStruct *my_struct_create(int64_t key)
{
    MyStruct *ms = (MyStruct *)malloc(sizeof(MyStruct));

    (void)memset((void *)ms, 0, sizeof(MyStruct));

    ms->key = key;

    return ms;
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


#define TEST_ARRAY_CREATE(type, len) \
    do { \
        type *ptr = (type *)array_create(len, sizeof(type)); \
        T_ERROR(ptr == NULL); \
        array_destroy(ptr); \
    } while (0)


static void test_array_create(void)
{
    const size_t len = 1000;

    TEST_ARRAY_CREATE(int8_t, len * 1);
    TEST_ARRAY_CREATE(int16_t, len * 2);
    TEST_ARRAY_CREATE(int32_t, len * 3);
    TEST_ARRAY_CREATE(int64_t, len * 4);
    TEST_ARRAY_CREATE(char, len * 5);
    TEST_ARRAY_CREATE(float, len * 6);
    TEST_ARRAY_CREATE(double, len * 7);
    TEST_ARRAY_CREATE(size_t, len * 8);
}


static void test_destroy_with_entries(void)
{
    const size_t len = 1000;

    int64_t *keys; 
    MyStruct **ms;

    keys = (int64_t *)array_create(len, sizeof(int64_t));
    T_ERROR(keys == NULL);

    for (size_t i = 0; i < len; ++i)
        keys[i] = (int64_t)i + 1;

    ms = (MyStruct **)array_create(len, sizeof(*ms));
    T_ERROR(ms == NULL);

    for (size_t i = 0; i < len; ++i)
        ms[i] = my_struct_create(keys[i]);

    FREE(keys);
    array_destroy_with_entries(ms, my_struct_destroy, len, sizeof(*ms));
}


#define TEST_ARRAY_COPY(type, len) \
    do { \
        type *ptr1; \
        type *ptr2; \
        ptr1 = (type *)array_create(len, sizeof(type)); \
        T_ERROR(ptr1 == NULL); \
        for (size_t i = 0; i < len; ++i) \
            ptr1[i] = (type)(i + 1); \
        ptr2 = (type *)array_create(len, sizeof(type)); \
        T_ERROR(ptr2 == NULL); \
        array_copy(ptr2, ptr1, len, sizeof(type)); \
        T_EXPECT(memcmp((const void *)&ptr1[0], (const void *)&ptr2[0], len), 0); \
        array_destroy(ptr1); \
        array_destroy(ptr2); \
    } while (0)


static void test_array_copy(void)
{
    const size_t len = 1000;

    TEST_ARRAY_COPY(int8_t, len * 1);
    TEST_ARRAY_COPY(int16_t, len * 2);
    TEST_ARRAY_COPY(int32_t, len * 3);
    TEST_ARRAY_COPY(int64_t, len * 4);
    TEST_ARRAY_COPY(char, len * 5);
    TEST_ARRAY_COPY(float, len * 6);
    TEST_ARRAY_COPY(double, len * 7);
    TEST_ARRAY_COPY(size_t, len * 8);
}


#define TEST_ARRAY_CLONE(type, len) \
    do { \
        type *ptr1; \
        type *ptr2; \
        ptr1 = (type *)array_create(len, sizeof(type)); \
        T_ERROR(ptr1 == NULL); \
        for (size_t i = 0; i < len; ++i) \
            ptr1[i] = (type)(i + 1); \
        ptr2 = (type *)array_clone(ptr1, len, sizeof(type)); \
        T_ERROR(ptr2 == NULL); \
        T_EXPECT(memcmp((const void *)&ptr1[0], (const void *)&ptr2[0], len), 0); \
        array_destroy(ptr1); \
        array_destroy(ptr2); \
    } while (0)


static void test_array_clone(void)
{
    const size_t len = 1000;

    TEST_ARRAY_CLONE(int8_t, len * 1);
    TEST_ARRAY_CLONE(int16_t, len * 2);
    TEST_ARRAY_CLONE(int32_t, len * 3);
    TEST_ARRAY_CLONE(int64_t, len * 4);
    TEST_ARRAY_CLONE(char, len * 5);
    TEST_ARRAY_CLONE(float, len * 6);
    TEST_ARRAY_CLONE(double, len * 7);
    TEST_ARRAY_CLONE(size_t, len * 8);
}


#define TEST_ARRAY_MOVE(type, len) \
    do { \
        assert(len >= 100); \
        type *ptr1; \
        type *ptr2; \
        ptr1 = (type *)array_create(len, sizeof(type)); \
        T_ERROR(ptr1 == NULL); \
        for (size_t i = 0; i < len; ++i) \
            ptr1[i] = (type)(i + 1); \
        ptr2 = (type *)array_clone(ptr1, len, sizeof(type)); \
        T_ERROR(ptr2 == NULL); \
        array_move(ptr1, ptr1 + 1, len - 1, sizeof(type)); \
        T_EXPECT(memcmp((const void *)&ptr1[0], (const void *)&ptr2[0 + 1], len - sizeof(type)), 0); \
        array_move(ptr1, ptr1 + 99, len - 100, sizeof(type)); \
        T_EXPECT(memcmp((const void *)&ptr1[0], (const void *)&ptr2[0 + 100], len - (sizeof(type) * 100)), 0); \
        array_destroy(ptr1); \
        array_destroy(ptr2); \
    } while (0)


static void test_array_move(void)
{
    const size_t len = 1000;

    TEST_ARRAY_MOVE(int8_t, len * 1);
    TEST_ARRAY_MOVE(int16_t, len * 2);
    TEST_ARRAY_MOVE(int32_t, len * 3);
    TEST_ARRAY_MOVE(int64_t, len * 4);
    TEST_ARRAY_MOVE(char, len * 5);
    TEST_ARRAY_MOVE(float, len * 6);
    TEST_ARRAY_MOVE(double, len * 7);
    TEST_ARRAY_MOVE(size_t, len * 8);
}


#define TEST_ARRAY_ZEROS(type, len) \
    do { \
        type *ptr1; \
        type *ptr2; \
        ptr1 = (type *)array_create(len, sizeof(type)); \
        T_ERROR(ptr1 == NULL); \
        for (size_t i = 0; i < len; ++i) \
            ptr1[i] = (type)(i + 1); \
        /* array_create use calloc instead of malloc */ \
        ptr2 = (type *)array_create(len, sizeof(type)); \
        array_zeros(ptr1, len, sizeof(type)); \
        T_EXPECT(memcmp((const void *)&ptr1[0], (const void *)&ptr2[0], len), 0); \
        array_destroy(ptr1); \
        array_destroy(ptr2); \
    } while (0)


static void test_array_zeros(void)
{
    const size_t len = 1000;

    TEST_ARRAY_ZEROS(int8_t, len * 1);
    TEST_ARRAY_ZEROS(int16_t, len * 2);
    TEST_ARRAY_ZEROS(int32_t, len * 3);
    TEST_ARRAY_ZEROS(int64_t, len * 4);
    TEST_ARRAY_ZEROS(char, len * 5);
    TEST_ARRAY_ZEROS(float, len * 6);
    TEST_ARRAY_ZEROS(double, len * 7);
    TEST_ARRAY_ZEROS(size_t, len * 8);
}


#define TEST_ARRAY_UNSORTED_INSERT_FIRST(type, len) \
    do { \
        type *array = (type *)array_create(len, sizeof(type)); \
        T_ERROR(array == NULL); \
        for (size_t i = 0; i < len; ++i) \
        { \
            type val = (type)(i + 1); \
            T_EXPECT(array_unsorted_insert_first(array, len, sizeof(type), &val), 0); \
        } \
        for (size_t i = 0; i < len; ++i) \
            T_ASSERT(array[i], (type)(len - i)); \
        array_destroy(array); \
    } while (0)


static void test_array_unsorted_insert_first(void)
{
    const size_t len = 1000;

    TEST_ARRAY_UNSORTED_INSERT_FIRST(int8_t, len * 1);
    TEST_ARRAY_UNSORTED_INSERT_FIRST(int16_t, len * 2);
    TEST_ARRAY_UNSORTED_INSERT_FIRST(int32_t, len * 3);
    TEST_ARRAY_UNSORTED_INSERT_FIRST(int64_t, len * 4);
    TEST_ARRAY_UNSORTED_INSERT_FIRST(char, len * 5);
    TEST_ARRAY_UNSORTED_INSERT_FIRST(float, len * 6);
    TEST_ARRAY_UNSORTED_INSERT_FIRST(double, len * 7);
    TEST_ARRAY_UNSORTED_INSERT_FIRST(size_t, len * 8);
}


#define TEST_ARRAY_UNSORTED_INSERT_LAST(type, len) \
    do { \
        type *array = (type *)array_create(len, sizeof(type)); \
        T_ERROR(array == NULL); \
        for (size_t i = 0; i < len; ++i) \
        { \
            type val = (type)(len - i); \
            T_EXPECT(array_unsorted_insert_last(array, len - i, sizeof(type), &val), 0); \
        } \
        for (size_t i = 0; i < len; ++i) \
            T_ASSERT(array[i], (type)(i + 1)); \
        array_destroy(array); \
    } while (0)


static void test_array_unsorted_insert_last(void)
{
    const size_t len = 1000;

    TEST_ARRAY_UNSORTED_INSERT_LAST(int8_t, len);
    TEST_ARRAY_UNSORTED_INSERT_LAST(int16_t, len * 2);
    TEST_ARRAY_UNSORTED_INSERT_LAST(int32_t, len * 3);
    TEST_ARRAY_UNSORTED_INSERT_LAST(int64_t, len * 4);
    TEST_ARRAY_UNSORTED_INSERT_LAST(char, len * 5);
    TEST_ARRAY_UNSORTED_INSERT_LAST(float, len * 6);
    TEST_ARRAY_UNSORTED_INSERT_LAST(double, len * 7);
    TEST_ARRAY_UNSORTED_INSERT_LAST(size_t, len * 8);
}


#define TEST_ARRAY_UNSORTED_INSERT_POS(type, len) \
    do { \
        type *array = (type *)array_create(len, sizeof(type)); \
        T_ERROR(array == NULL); \
        for (size_t i = 0; i < len; ++i) \
        { \
            type val = (type)(i + 1); \
            T_EXPECT(array_unsorted_insert_pos(array, len, sizeof(type), i, &val), 0); \
        } \
        for (size_t i = 0; i < len; ++i) \
            T_ASSERT(array[i], (type)(i + 1)); \
        array_destroy(array); \
    } while (0)


static void test_array_unsorted_insert_pos(void)
{
    const size_t len = 1000;

    TEST_ARRAY_UNSORTED_INSERT_POS(int8_t, len * 1);
    TEST_ARRAY_UNSORTED_INSERT_POS(int16_t, len * 2);
    TEST_ARRAY_UNSORTED_INSERT_POS(int32_t, len * 3);
    TEST_ARRAY_UNSORTED_INSERT_POS(int64_t, len * 4);
    TEST_ARRAY_UNSORTED_INSERT_POS(char, len * 5);
    TEST_ARRAY_UNSORTED_INSERT_POS(float, len * 6);
    TEST_ARRAY_UNSORTED_INSERT_POS(double, len * 7);
    TEST_ARRAY_UNSORTED_INSERT_POS(size_t, len * 8);
}


static void test_array_delete(void)
{
    const size_t len = 10;

    int64_t arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int64_t ext_arr[] = { 3, 4, 6, 7, 8, 10, 10, 10, 10, 10 };

    T_EXPECT(array_delete_first(arr, ARRAY_SIZE(arr), sizeof(*arr)), 0);

    for (size_t i = 0; i < len - 1; ++i)
        T_ASSERT(arr[i], (int64_t)(i + 2));

    T_ASSERT(arr[ARRAY_SIZE(arr) - 1], 10);

    T_EXPECT(array_delete_pos(arr, len, sizeof(*arr), 3), 0);
    T_EXPECT(array_delete_pos(arr, len, sizeof(*arr), 0), 0);
    T_EXPECT(array_delete_pos(arr, len, sizeof(*arr), 5), 0);

    for (size_t i = 0; i < len; ++i)
        T_ASSERT(arr[i], ext_arr[i]);

    T_EXPECT(array_delete_last(arr, len >> 1, sizeof(*arr)), 0);

    for (size_t i = 0; i < len; ++i)
        T_ASSERT(arr[i], ext_arr[i]);
}


static void test_array_delete_all(void)
{
    int64_t arr1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int64_t arr2[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int64_t arr3[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int64_t expt_arr[] = { 2, 4, 6, 8, 10, 10, 10, 10, 10, 10 };

    for (size_t i = 0; i < ARRAY_SIZE(arr1); ++i)
        T_EXPECT(array_delete_first(arr1, ARRAY_SIZE(arr1), sizeof(*arr1)), 0);

    for (size_t i = 0; i < ARRAY_SIZE(arr1); ++i)
        T_ASSERT(arr1[i], (int64_t)10);

    for (size_t i = 0; i < ARRAY_SIZE(arr2); ++i)
        T_EXPECT(array_delete_last(arr2, ARRAY_SIZE(arr2), sizeof(*arr2)), 0);

    for (size_t i = 0; i < ARRAY_SIZE(arr2); ++i)
        T_ASSERT(arr2[i], (int64_t)(i + 1));

    for (size_t i = 0; i < ARRAY_SIZE(arr3); ++i)
        T_EXPECT(array_delete_pos(arr3, ARRAY_SIZE(arr3), sizeof(*arr3), i), 0);
        
    for (size_t i = 0; i < ARRAY_SIZE(arr3); ++i)
        T_ASSERT(arr3[i], expt_arr[i]);
}


static void test_array_delete_with_entry(void)
{
    int64_t arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int64_t expt_arr[] = { 2, 3, 4, 5, 10, 10, 10, 10, 10, 10 };

    MyStruct *ms[ARRAY_SIZE(arr)];

    for (size_t i = 0; i < ARRAY_SIZE(arr); ++i)
        ms[i] = my_struct_create(arr[i]);

    T_EXPECT(array_delete_first_with_entry(ms, ARRAY_SIZE(ms), sizeof(*ms), my_struct_destroy), 0);

    /* 2, 3, 4, 5, 6, 7, 8, 9, 10, 10*/
    for (size_t i = 0; i < ARRAY_SIZE(arr) - 1; ++i)
        T_ASSERT(ms[i]->key, (int64_t)(i + 2));

    T_ASSERT(arr[ARRAY_SIZE(arr) - 1], 10);

    T_EXPECT(array_delete_pos_with_entry(ms, ARRAY_SIZE(ms) - 1, sizeof(*ms), 7, my_struct_destroy), 0);
    T_EXPECT(array_delete_pos_with_entry(ms, ARRAY_SIZE(ms) - 2, sizeof(*ms), 4, my_struct_destroy), 0);

    T_EXPECT(array_delete_last_with_entry(ms, ARRAY_SIZE(ms) - 3, sizeof(*ms), my_struct_destroy), 0);

    T_EXPECT(array_delete_pos_with_entry(ms, ARRAY_SIZE(ms) - 4, sizeof(*ms), 5, my_struct_destroy), 0);
    T_EXPECT(array_delete_pos_with_entry(ms, ARRAY_SIZE(ms) - 5, sizeof(*ms), 4, my_struct_destroy), 0);

    for (size_t i = 0; i < ARRAY_SIZE(ms) - 6; ++i)
        T_ASSERT(ms[i]->key, expt_arr[i]);

    for (size_t i = 0; i < ARRAY_SIZE(ms) - 6; ++i)
    {
        FREE(ms[i]);
    }
}


static void test_array_lower_bound(void)
{
    int64_t arr1[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23 };
    double arr2[] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 };

    int64_t val1;
    double val2;

    val1 = (int64_t)0;
    T_EXPECT(array_lower_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)0);

    val1 = (int64_t)1;
    T_EXPECT(array_lower_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)0);

    val1 = (int64_t)2;
    T_EXPECT(array_lower_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)0);

    val1 = (int64_t)6;
    T_EXPECT(array_lower_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)3);

    val1 = (int64_t)13;
    T_EXPECT(array_lower_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)5);

    val1 = (int64_t)18;
    T_EXPECT(array_lower_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)7);

    val1 = (int64_t)25;
    T_EXPECT(array_lower_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)9);

    val2 = -0.1;
    T_EXPECT(array_lower_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)0);

    val2 = 0.0;
    T_EXPECT(array_lower_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)0);

    val2 = 0.05;
    T_EXPECT(array_lower_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)0);

    val2 = 0.11;
    T_EXPECT(array_lower_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)1);

    val2 = 0.25;
    T_EXPECT(array_lower_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)2);

    val2 = 0.49;
    T_EXPECT(array_lower_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)4);

    val2 = 0.81;
    T_EXPECT(array_lower_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)8);

    val2 = 0.89;
    T_EXPECT(array_lower_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)8);

    val2 = 1.21;
    T_EXPECT(array_lower_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)9);
}


static void test_array_upper_bound(void)
{
    int64_t arr1[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23 };
    double arr2[] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 };

    int64_t val1;
    double val2;

    val1 = (int64_t)0;
    T_EXPECT(array_upper_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)0);

    val1 = (int64_t)1;
    T_EXPECT(array_upper_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)0);

    val1 = (int64_t)2;
    T_EXPECT(array_upper_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)1);

    val1 = (int64_t)6;
    T_EXPECT(array_upper_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)3);

    val1 = (int64_t)13;
    T_EXPECT(array_upper_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)6);

    val1 = (int64_t)18;
    T_EXPECT(array_upper_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)7);

    val1 = (int64_t)23;
    T_EXPECT(array_upper_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)9);

    val1 = (int64_t)25;
    T_EXPECT(array_upper_bound((const void * const)&arr1[0], ARRAY_SIZE(arr1), sizeof(*arr1), (const void * const)&val1, my_compare_int64_t), (ssize_t)9);

    val2 = -0.1;
    T_EXPECT(array_upper_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)0);

    val2 = 0.0;
    T_EXPECT(array_upper_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)0);

    val2 = 0.1;
    T_EXPECT(array_upper_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)1);

    val2 = 0.11;
    T_EXPECT(array_upper_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)1);

    val2 = 0.25;
    T_EXPECT(array_upper_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)2);

    val2 = 0.5;
    T_EXPECT(array_upper_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)5);

    val2 = 0.81;
    T_EXPECT(array_upper_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)8);

    val2 = 0.89;
    T_EXPECT(array_upper_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)8);

    val2 = 1.21;
    T_EXPECT(array_upper_bound((const void * const)&arr2[0], ARRAY_SIZE(arr2), sizeof(*arr2), (const void * const)&val2, my_compare_double), (ssize_t)9);
}


int main(void)
{
    TEST_INIT("ARRAY TESTING");
    TEST(test_array_create());
    TEST(test_destroy_with_entries());
    TEST(test_array_copy());
    TEST(test_array_clone());
    TEST(test_array_move());
    TEST(test_array_zeros());
    TEST(test_array_unsorted_insert_first());
    TEST(test_array_unsorted_insert_last());
    TEST(test_array_unsorted_insert_pos());
    TEST(test_array_delete());
    TEST(test_array_delete_all());
    TEST(test_array_delete_with_entry());
    TEST(test_array_lower_bound());
    TEST(test_array_upper_bound());
    TEST_SUMMARY();
}