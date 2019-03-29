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


int main(void)
{
    TEST_INIT("ARRAY TESTING\n");
    TEST(test_array_create());
    TEST(test_destroy_with_entries());
    TEST(test_array_copy());
    TEST(test_array_clone());
    TEST(test_array_move());
    TEST(test_array_zeros());
    TEST_SUMMARY();
}