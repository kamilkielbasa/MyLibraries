#ifndef STACK_H
#define STACK_H

/*
    Stack implementation
  
    Author: Kamil Kiełbasa
    email: kamilkielbasa73@gmail.com
  
    LICENCE: GPL 3.0
*/

#include <darray.h>
#include <stddef.h> /* size_t */
#include <stdbool.h> /* bool */
#include <sys/types.h> /* ssize_t */


typedef Darray Stack;


/*
    Create new instance of stack.

    PARAMS:
    @IN size_of - size of each element.

    RETURN:
    %IN NULL if failure.
    %Pointer to stack if success.
*/
Stack *stack_create(size_t size_of);


/*
    Deallocate stack with all entries.

    PARAMS:
    @IN stack - pointer to stack.

    RETURN:
    %This is void function.
*/
void stack_destroy(Stack *stack);


/*
    Push value on stack.

    PARAMS:
    @IN stack - pointer to stack.
    @IN entry - pointer to entry.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int stack_push(Stack * __restrict__ stack, const void * __restrict__ const entry);


/*
    Pop value from stack.

    PARAMS:
    @IN stack - pointer to stack.
    @IN val - value.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int stack_pop(Stack * __restrict__ stack, void * __restrict__ val_out);


/*
    Check if stack is empty.

    PARAMS:
    @IN stack - pointer to stack.
    @IN val - value.

    RETURN:
    %TRUE if stack is empty.
    %FALSE if is not empty of failure.
*/
bool stack_is_empty(const Stack * const stack);


/*
    Get top value from stack.

    PARAMS:
    @IN stack - pointer to stack.
    @IN val - value

    RETURN:
    %0 if success.
    %negative value if failure.
*/
int stack_get_top(const Stack * __restrict__ const stack, void * __restrict__ val);


/*
    Get size of array.

    PARAMS:
    @IN stack - pointer to stack.

    RETURN:
    %number of entries if success.
    %-1 if failure.
*/
ssize_t stack_get_size(const Stack * const stack);


/*
    Get number of entries.

    PARAMS:
    @IN stack - pointer to stack.

    RETURN:
    %number of entries if success.
    %-1 if failure.
*/
ssize_t stack_get_num_entries(const Stack * const stack);


/*
    Get size of data size.

    PARAMS:
    @IN stack - pointer to stack.

    RETURN:
    %sizeof if success.
    %-1 if failure.
*/
ssize_t stack_get_data_size(const Stack * const stack);


/*
    Get pointer to stack array.

    PARAMS:
    @IN stack - pointer to stack.

    RETURN:
    %pointer to array if success.
    %NULL if stack wasn't create or failure.
*/
void *stack_get_array(const Stack * const stack);


#endif /* STACK_H */
