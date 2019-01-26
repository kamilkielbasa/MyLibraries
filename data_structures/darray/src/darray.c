#include <darray.h>
#include <common.h>
#include <stdlib.h>
#include <string.h>

#define RATIO ((size_t)2)

/*
    Calculate offset.

    PARAMS:
    @IN src - pointer to the source.
    @IN size_of - offset to move.

    RETURN:
    %Pointer with offset.
*/
static __inline__ void *__calc_offset(const void * const src, const size_t offset)
{
	return ((char *)src + offset);
}

/*
    Realloc array in dynamic array when inserted new element.

    PARAMS:
    @IN src - pointer to the dynamic array.

    RETURN:
    %This is void function.
*/
static void __darray_resize_insert(Darray *darray)
{
	if (darray->size == 0)
	{
		darray->array = malloc(darray->size_of * RATIO);

		if (darray->array == NULL)
			VERROR("malloc error\n");

		darray->size = RATIO;
	}

	if (darray->num_entries == darray->size)
	{
		darray->array = realloc(darray->array, darray->size * darray->size_of * RATIO);

		if (darray->array == NULL)
			VERROR("realloc error\n");

		darray->size *= RATIO;
	}
}

/*
    Realloc array in dynamic array when deleted element.

    PARAMS:
    @IN src - pointer to the dynamic array.

    RETURN:
    %This is void function.
*/
static void __darray_resize_delete(Darray *darray)
{
	if (darray->num_entries == 1)
	{
		darray->size = 0;
		FREE(darray->array);
		return;
	}

	if (darray->num_entries == (darray->size / (2 * RATIO)))
	{
		darray->array = realloc(darray->array, (darray->size / RATIO) * darray->size_of);

		if (darray->array == NULL)
			VERROR("realloc error\n");

		darray->size /= RATIO;
	}
}

/*
    Insert new element to the sorted dynamic array.

    PARAMS:
    @IN src - pointer to the dynamic array.
	@IN entry - pointer to entry.

    RETURN:
    %0 if success.
	%negative value if failure.
*/
static int __darray_sorted_insert(Darray * restrict darray, const void * restrict entry)
{
	if (entry == NULL)
		ERROR("entry == NULL\n", -1);

	__darray_resize_insert(darray);

	size_t pos = 0;

	for (size_t index = 0; index < darray->num_entries; ++index)
	{
		void *curr = __calc_offset(darray->array, (index * darray->size_of));

		if (darray->cmp_f(entry, curr) >= 0)
		{
			pos = index + 1;
		}
	} 

	const void *src = __calc_offset(darray->array, (pos * darray->size_of));
	void *dst = __calc_offset(darray->array, ((pos + 1) * darray->size_of));

	size_t to_move = ((darray->num_entries - pos) * darray->size_of);
	(void)memmove(dst, src, to_move);
	__ASSIGN__(*(char *)src, *(char *)entry, darray->size_of);

    ++darray->num_entries;

    return 0;
}

/*
    Insert new element to the unsorted dynamic array.

    PARAMS:
    @IN src - pointer to the dynamic array.
	@IN entry - pointer to entry.

    RETURN:
    %0 if success.
	%negative value if failure.
*/
static int __darray_unsorted_insert(Darray * restrict darray, const void * restrict entry)
{
	if (entry == NULL)
		ERROR("entry == NULL\n", -1);

	__darray_resize_insert(darray);

	void *src = __calc_offset(darray->array, (darray->num_entries * darray->size_of));
	__ASSIGN__(*(char *)src, *(char *)entry, darray->size_of);
    ++darray->num_entries;

    return 0;
}

Darray *darray_create(const DARRAY_TYPE type, const size_t size_of, const size_t size, compare_func cmp_f)
{
	if (size_of < 1)
		ERROR("size_of < 1\n", NULL);

	if (cmp_f == NULL)
		ERROR("cmp_f == NULL\n", NULL);

	Darray *darray = (Darray *)malloc(sizeof(Darray));

	if (darray == NULL)
		ERROR("malloc error\n", NULL);

	if (size >= 1)
	{
		darray->array = malloc(size_of * size);

		if (darray->array == NULL)
		{
			FREE(darray);
			ERROR("malloc error\n", NULL);
		}
	}

	darray->cmp_f = cmp_f;
	darray->type = type;
	darray->size_of = size_of;
	darray->num_entries = 0;
	darray->size = size;

	return darray;
}

void darray_destroy(Darray *darray)
{
	if (darray == NULL)
		VERROR("darray == NULL\n");

	if (darray->array != NULL)
		FREE(darray->array);

	FREE(darray);
}

int darray_insert(Darray * restrict darray, const void * restrict entry)
{
	if (darray == NULL)
		ERROR("darray == NULL\n", -1);

	return (darray->type == DARRAY_SORTED) ? __darray_sorted_insert(darray, entry) : __darray_unsorted_insert(darray, entry);
}

int darray_delete(Darray * restrict darray, void * restrict val_out)
{
	if (darray->array == NULL)
		ERROR("darray->array == NULL\n", -1);

	if (val_out != NULL)
	{
		const void *dst = __calc_offset(darray->array, ((darray->num_entries - 1) * darray->size_of));
		__ASSIGN__(*(char *)val_out, *(char *)dst, darray->size_of);
	}

	__darray_resize_delete(darray);
	--darray->num_entries; 

	return 0;
}

int darray_get_data(const Darray * const restrict darray, void * restrict val_out, const size_t pos)
{
	if (darray == NULL)
		ERROR("darray == NULL\n", -1);

	if (darray->array == NULL)
		ERROR("darray->array == NULL\n", -1);

	if (val_out == NULL)
		ERROR("val_out == NULL\n", -1);

	if (pos > (darray->num_entries - 1))
		ERROR("pos > (darray->num_entries - 1)\n", -1);

	void *src = __calc_offset(darray->array, (pos * darray->size_of));
	__ASSIGN__(*(char *)val_out, *(char *)src, darray->size_of);
	
	return 0;
}

ssize_t darray_get_num_entries(const Darray * const darray)
{
	if (darray == NULL)
		ERROR("darray == NULL\n", -1);

	return (ssize_t)darray->num_entries;
}