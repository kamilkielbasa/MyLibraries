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

	darray_destroy(darray);
}

static void test_insert_pos_unsorted_darray(void)
{
	Darray *darray = darray_create(DARRAY_UNSORTED, sizeof(S), (size_t)0, compare);
	T_ERROR(darray == NULL);

	const S arr[] = 
	{
		{ 0, 36 },
		{ 1, 37 },
		{ 2, 38 },
		{ 3, 39 },
		{ 4, 40 },
		{ 5, 41 },
		{ 6, 42 },
		{ 7, 43 }
	};

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		T_EXPECT(darray_insert_pos(darray, &arr[index], index), 0);
	}

	T_CHECK(darray->array != NULL);
	T_CHECK(darray->num_entries == ARRAY_SIZE(arr));

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);
		T_CHECK(arr[index].a == expt.a);
		T_CHECK(arr[index].b == expt.b);
	}

	darray_destroy(darray);

	darray = darray_create(DARRAY_UNSORTED, sizeof(S), (size_t)64, compare);
	T_ERROR(darray == NULL);

	const S expt_arr[] = 
	{
		{ 2, 38 },
		{ 5, 41 },
		{ 7, 43 },
		{ 3, 39 },
		{ 0, 36 },
		{ 4, 40 },
		{ 1, 37 },
		{ 6, 42 }
	};

	T_EXPECT(darray_insert_pos(darray, &arr[2], 0), 0);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);
		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	T_EXPECT(darray_insert_pos(darray, &arr[5], 1), 0);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);
		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	T_EXPECT(darray_insert_pos(darray, &arr[7], 2), 0);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);
		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	T_EXPECT(darray_insert_pos(darray, &arr[3], 3), 0);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);
		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	T_EXPECT(darray_insert_pos(darray, &arr[0], 4), 0);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);
		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	T_EXPECT(darray_insert_pos(darray, &arr[4], 5), 0);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);
		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	T_EXPECT(darray_insert_pos(darray, &arr[1], 6), 0);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);
		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	T_EXPECT(darray_insert_pos(darray, &arr[6], 7), 0);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);
		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	T_CHECK(darray->size == 64);

	darray_destroy(darray);
}

static void test_delete_pos_unsorted_darray(void)
{
	Darray *darray = darray_create(DARRAY_UNSORTED, sizeof(S), (size_t)0, compare);
	T_ERROR(darray == NULL);

	const S arr[] = 
	{
		{ 0, 36 },
		{ 1, 37 },
		{ 2, 38 },
		{ 3, 39 },
		{ 4, 40 },
		{ 5, 41 },
		{ 6, 42 },
		{ 7, 43 }
	};

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		T_EXPECT(darray_insert(darray, &arr[index]), 0);
	}

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		size_t curr_index = darray->num_entries - index - 1;
		T_EXPECT(darray_delete_pos(darray, &expt, curr_index), 0);

		T_CHECK(arr[curr_index].a == expt.a);
		T_CHECK(arr[curr_index].b == expt.b);
	}

	darray_destroy(darray);

	darray = darray_create(DARRAY_UNSORTED, sizeof(S), (size_t)1024, compare);
	T_ERROR(darray == NULL);

	for (size_t index = 0; index < ARRAY_SIZE(arr); ++index)
	{
		T_EXPECT(darray_insert(darray, &arr[index]), 0);
	}

	S curr_expt = {0};
	size_t curr_index = 0;
	S expt_arr[ARRAY_SIZE(arr)] = {0}; 

	expt_arr[0].a = 1;
	expt_arr[0].b = 37;
	expt_arr[1].a = 2;
	expt_arr[1].b = 38;
	expt_arr[2].a = 3;
	expt_arr[2].b = 39;
	expt_arr[3].a = 4;
	expt_arr[3].b = 40;
	expt_arr[4].a = 5;
	expt_arr[4].b = 41;
	expt_arr[5].a = 6;
	expt_arr[5].b = 42;
	expt_arr[6].a = 7;
	expt_arr[6].b = 43;

	T_EXPECT(darray_delete_pos(darray, &curr_expt, curr_index), 0);
	T_CHECK(arr[0].a == curr_expt.a);
	T_CHECK(arr[0].b == curr_expt.b);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);

		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	expt_arr[0].a = 1;
	expt_arr[0].b = 37;
	expt_arr[1].a = 2;
	expt_arr[1].b = 38;
	expt_arr[2].a = 3;
	expt_arr[2].b = 39;
	expt_arr[3].a = 5;
	expt_arr[3].b = 41;
	expt_arr[4].a = 6;
	expt_arr[4].b = 42;
	expt_arr[5].a = 7;
	expt_arr[5].b = 43;

	curr_index = 3;
	T_EXPECT(darray_delete_pos(darray, &curr_expt, curr_index), 0);
	T_CHECK(arr[4].a == curr_expt.a);
	T_CHECK(arr[4].b == curr_expt.b);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);

		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	expt_arr[0].a = 1;
	expt_arr[0].b = 37;
	expt_arr[1].a = 3;
	expt_arr[1].b = 39;
	expt_arr[2].a = 5;
	expt_arr[2].b = 41;
	expt_arr[3].a = 6;
	expt_arr[3].b = 42;
	expt_arr[4].a = 7;
	expt_arr[4].b = 43;

	curr_index = 1;
	T_EXPECT(darray_delete_pos(darray, &curr_expt, curr_index), 0);
	T_CHECK(arr[2].a == curr_expt.a);
	T_CHECK(arr[2].b == curr_expt.b);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);

		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	expt_arr[0].a = 3;
	expt_arr[0].b = 39;
	expt_arr[1].a = 5;
	expt_arr[1].b = 41;
	expt_arr[2].a = 6;
	expt_arr[2].b = 42;
	expt_arr[3].a = 7;
	expt_arr[3].b = 43;

	curr_index = 0;
	T_EXPECT(darray_delete_pos(darray, &curr_expt, curr_index), 0);
	T_CHECK(arr[1].a == curr_expt.a);
	T_CHECK(arr[1].b == curr_expt.b);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);

		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	expt_arr[0].a = 3;
	expt_arr[0].b = 39;
	expt_arr[1].a = 5;
	expt_arr[1].b = 41;
	expt_arr[2].a = 7;
	expt_arr[2].b = 43;

	curr_index = 2;
	T_EXPECT(darray_delete_pos(darray, &curr_expt, curr_index), 0);
	T_CHECK(arr[6].a == curr_expt.a);
	T_CHECK(arr[6].b == curr_expt.b);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);

		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	expt_arr[0].a = 3;
	expt_arr[0].b = 39;
	expt_arr[1].a = 7;
	expt_arr[1].b = 43;

	curr_index = 1;
	T_EXPECT(darray_delete_pos(darray, &curr_expt, curr_index), 0);
	T_CHECK(arr[5].a == curr_expt.a);
	T_CHECK(arr[5].b == curr_expt.b);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);

		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	expt_arr[0].a = 7;
	expt_arr[0].b = 43;

	curr_index = 0;
	T_EXPECT(darray_delete_pos(darray, &curr_expt, curr_index), 0);
	T_CHECK(arr[3].a == curr_expt.a);
	T_CHECK(arr[3].b == curr_expt.b);

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		S expt = {0};
		T_EXPECT(darray_get_data(darray, &expt, index), 0);

		T_CHECK(expt_arr[index].a == expt.a);
		T_CHECK(expt_arr[index].b == expt.b);
	}

	curr_index = 0;
	T_EXPECT(darray_delete_pos(darray, &curr_expt, curr_index), 0);
	T_CHECK(arr[7].a == curr_expt.a);
	T_CHECK(arr[7].b == curr_expt.b);

	T_CHECK(darray->num_entries == 0);
	T_CHECK(darray->array == NULL);

	darray_destroy(darray);
}

int main(void)
{
	TEST_INIT("TESTING DYNAMIC ARRAY");
	TEST(test_create_and_delete_darray());
	TEST(test_insert_sorted_darray());
	TEST(test_insert_unsorted_darray());
	TEST(test_delete_sorted_unsorted_darray());
	TEST(test_insert_pos_unsorted_darray());
	TEST(test_delete_pos_unsorted_darray());
	TEST_SUMMARY();

	return 0;
}