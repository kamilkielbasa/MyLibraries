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


static void test_create_and_delete_darray(void)
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

static void test_insert_sorted_darray(void)
{
	Darray *darray = darray_create(DARRAY_SORTED, sizeof(S), (size_t)9, compare);
	T_ERROR(darray == NULL);

	const S arr[] = 
	{
		{ 0, 1  },
		{ 0, 27 },
		{ 0, 12 },
		{ 0, 2  },
		{ 0, 25 },
		{ 0, 50 },
		{ 0, 13 },
		{ 0, 17 }
	};

	const S expt_arr[] =
	{
		{ 0, 1  },
		{ 0, 2  },
		{ 0, 12 },
		{ 0, 13 },
		{ 0, 17 },
		{ 0, 25 },
		{ 0, 27 },
		{ 0, 50 }
	};

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		T_EXPECT(darray_insert(darray, &arr[index]), 0);

		T_CHECK(darray->num_entries == (index + 1));
	}

	for (size_t index = 0; index < (size_t)darray_get_num_entries(darray); ++index)
	{
		S val_out = {0};
		T_EXPECT(darray_get_data(darray, &val_out, index), 0);

		T_CHECK(val_out.a == expt_arr[index].a);
		T_CHECK(val_out.b == expt_arr[index].b);
	}

	darray_destroy(darray);

	darray = darray_create(DARRAY_SORTED, sizeof(S), (size_t)0, compare);

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		T_EXPECT(darray_insert(darray, &arr[index]), 0);

		T_CHECK(darray->num_entries == (index + 1));
	}

	for (size_t index = 0; index < (size_t)darray_get_num_entries(darray); ++index)
	{
		S val_out = {0};
		T_EXPECT(darray_get_data(darray, &val_out, index), 0);

		T_CHECK(val_out.a == expt_arr[index].a);
		T_CHECK(val_out.b == expt_arr[index].b);
	}

	darray_destroy(darray);
}

static void test_insert_unsorted_darray(void)
{
	Darray *darray = darray_create(DARRAY_UNSORTED, sizeof(S), (size_t)9, compare);
	T_ERROR(darray == NULL);

	const S arr[] = 
	{
		{ 0, 27 },
		{ 0, 13 },
		{ 0, 12 },
		{ 0, 37 },
		{ 0, 99 },
		{ 0, 57 },
		{ 0, 14 },
		{ 0, 33 }
	};

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		T_EXPECT(darray_insert(darray, &arr[index]), 0);

		T_CHECK(darray->num_entries == (index + 1));
	}

	for (size_t index = 0; index < (size_t)darray_get_num_entries(darray); ++index)
	{
		S val_out = {0};
		T_EXPECT(darray_get_data(darray, &val_out, index), 0);

		T_CHECK(val_out.a == arr[index].a);
		T_CHECK(val_out.b == arr[index].b);
	}

	darray_destroy(darray);

	darray = darray_create(DARRAY_UNSORTED, sizeof(S), (size_t)0, compare);

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		T_EXPECT(darray_insert(darray, &arr[index]), 0);

		T_CHECK(darray->num_entries == (index + 1));
	}

	for (size_t index = 0; index < (size_t)darray_get_num_entries(darray); ++index)
	{
		S val_out = {0};
		T_EXPECT(darray_get_data(darray, &val_out, index), 0);

		T_CHECK(val_out.a == arr[index].a);
		T_CHECK(val_out.b == arr[index].b);
	}

	darray_destroy(darray);
}

static void test_delete_sorted_unsorted_darray(void)
{
	Darray *darray = darray_create(DARRAY_UNSORTED, sizeof(S), (size_t)9, compare);
	T_ERROR(darray == NULL);

	const S arr[] = 
	{
		{ 10, 37 },
		{ 67, 27 },
		{ 93, 13 },
		{ 48, 12 },
		{ 11, 14 },
		{ 79, 99 },
		{ 23, 57 },
		{ 70, 33 }
	};

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		T_EXPECT(darray_insert(darray, &arr[index]), 0);
	}

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		S val_out = {0};

		T_EXPECT(darray_delete(darray, &val_out), 0);

		size_t num_entries = ARRAY_SIZE(arr) - index;
		size_t del_index = num_entries - 1;
		
		T_CHECK(darray->num_entries == (num_entries - 1));
		T_CHECK(val_out.a == arr[del_index].a);
		T_CHECK(val_out.b == arr[del_index].b);
	}
}

int main(void)
{
	TEST_INIT("TESTING DYNAMIC ARRAY");
	TEST(test_create_and_delete_darray());
	TEST(test_insert_sorted_darray());
	TEST(test_insert_unsorted_darray());
	TEST(test_delete_sorted_unsorted_darray());
	TEST_SUMMARY();

	return 0;
}