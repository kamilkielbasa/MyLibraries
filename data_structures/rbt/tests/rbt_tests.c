#include <rbt.h>
#include <ctest.h>
#include <stdint.h> /* int64_t */
#include <assert.h>

typedef struct MyStruct
{
    int64_t key;
    int64_t a;
    int64_t b;
    int64_t c;
} MyStruct;

static int my_struct_compare(const void* a, const void* b)
{
    assert(a != NULL && b != NULL);
    const MyStruct *ia = (const MyStruct *)a;
    const MyStruct *ib = (const MyStruct *)b;

    if (ia->key > ib->key) return 1;
    if (ia->key == ib->key) return 0;
	return -1;
}

static void my_struct_destroy(void *s)
{
    MyStruct *ms = *(MyStruct **)s;
    FREE(ms);
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

int main(void)
{
	TEST_INIT("TESTING RED BLACK TREE");
    TEST(test_rbt_create());
    TEST(test_rbt_insert_only_root());
	TEST_SUMMARY();

	return 0;
}