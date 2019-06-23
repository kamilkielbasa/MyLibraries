#include <list.h>
#include <ctest.h>
#include <stdint.h>
#include <assert.h>


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


int main(void)
{
    TEST_INIT("TESTING LINKED LIST");
    TEST(test_list_create_destroy_empty());
    TEST_SUMMARY();

    return 0;
}