#include <darray.h>
#include <ctest.h>
#include <stdint.h> 
#include <assert.h>

typedef struct S
{
	int64_t a;
	int64_t b;
} S;

static int compare(const void* a, const void* b)
{
    assert(a && b);
    const S* ia = (const S*)a;
    const S* ib = (const S*)b;

	if ((ia->a + ia->b) > (ib->a + ib->b)) return 1;
	if ((ia->a + ia->b) == (ib->a + ib->b)) return 0;
	return -1;
}

static void test_create_and_delete(void)
{
	Darray *darray = darray_create(DARRAY_SORTED, sizeof(S), (size_t)0, compare);

	T_ERROR(darray == NULL);

	T_CHECK(darray->array == NULL);
	T_CHECK(darray->cmp_f == compare);
	T_CHECK(darray->type == DARRAY_SORTED);
	T_CHECK(darray->size_of == sizeof(S));
	T_CHECK(darray->num_entries == 0);
	T_CHECK(darray->size == 0);

	darray_destroy(darray);

	darray = darray_create(DARRAY_UNSORTED, sizeof(S), (size_t)256, compare);

	T_ERROR(darray == NULL);

	T_CHECK(darray->array != NULL);
	T_CHECK(darray->cmp_f == compare);
	T_CHECK(darray->type == DARRAY_UNSORTED);
	T_CHECK(darray->size_of == sizeof(S));
	T_CHECK(darray->num_entries == 0);
	T_CHECK(darray->size == (size_t)256);

	darray_destroy(darray);
}

int main(void)
{
	TEST_INIT("TESTING DYNAMIC ARRAY");
	TEST(test_create_and_delete());
	TEST_SUMMARY();

	return 0;
}