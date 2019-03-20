#include <stack.h>
#include <common.h>


Stack *stack_create(size_t size_of)
{
	if (size_of < 1)
		ERROR("size_of < 1\n", NULL);

	return darray_create(DARRAY_UNSORTED, size_of, 0, NULL, NULL);
}


void stack_destroy(Stack *stack)
{
	if (stack == NULL)
        return;

	darray_destroy(stack);
}


int stack_push(Stack * restrict stack, const void * restrict val_in)
{
	if (stack == NULL || val_in == NULL)
		ERROR("stack == NULL || val_in == NULL\n", -1);

	return darray_insert(stack, val_in);
}


bool stack_is_empty(const Stack *stack)
{
	if (stack == NULL)
		ERROR("stack == NULL\n", false);

	return darray_get_num_entries(stack) == 0;
}


int stack_pop(Stack * restrict stack, void * restrict val_out)
{
	if (stack == NULL || val_out == NULL)
		ERROR("stack == NULL || val_out == NULL\n", -1);

	if (stack_is_empty(stack))
		ERROR("stack is empty\n", -1);

	return darray_delete(stack, val_out);
}


int stack_get_top(const Stack * restrict stack, void * restrict val_out)
{
	if (stack == NULL || val_out == NULL)
		ERROR("stack == NULL || val_out == NULL\n", -1);

	if (stack_is_empty(stack))
		ERROR("stack is empty\n", -1);

	return darray_get_data(stack, val_out, (size_t)darray_get_num_entries(stack) - 1);
}

ssize_t stack_get_size(const Stack *stack)
{
	if (stack == NULL)
		ERROR("stack == NULL\n", -1);

	return darray_get_size(stack);
}

ssize_t stack_get_num_entries(const Stack *stack)
{
	if (stack == NULL)
		ERROR("stack == NULL\n", -1);

	return darray_get_num_entries(stack);
}


void *stack_get_array(const Stack *stack)
{
	if (stack == NULL)
		ERROR("stack == NULL\n", NULL);

	return darray_get_array(stack);
}
