#include <list.h>
#include <common.h>
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy */
#include <stdbool.h> /* bool */


struct List_node
{
    struct List_node *next;     /* pointer to next node */
    size_t size_of;             /* size of node */

    BYTE data[];                /* placeholder for data */
};


struct List
{
    List_node *head;            /* pointer to head */
    List_node *tail;            /* pointer to tail */

    size_t length;              /* num of nodes in list */
    size_t size_of;             /* size of element */

    compare_f cmp_f;            /* compare function */
    destructor_f destroy_f;     /* destructor function */
};


/*
    Destroy list node.

    PARAMS:
    @IN list_node - pointer to list node

    RETURN:
    %This is void function.
*/
___inline___ static void __list_node_destroy(List_node *list_node);


/*
    Destroy whole list.

    PARAMS:
    @IN list - pointer to list.
    @IN destroy - call destructor.

    RETURN:
    %This is void function.
*/
static void __list_destroy(List *list, bool destroy);


___inline___ static void __list_node_destroy(List_node *list_node)
{
    if (list_node == NULL)
        VERROR("list_node == NULL\n");

    FREE(list_node);
}


static void __list_destroy(List *list, bool destroy)
{
    if (list == NULL)
        VERROR("list == NULL\n");

    List_node *ptr = list->head;
    List_node *next = NULL;

    while (ptr != NULL)
    {
        next = ptr->next;

        if (destroy == true && list->destroy_f != NULL)
            list->destroy_f((void *)ptr->data);
        
        __list_node_destroy(ptr);

        ptr = next;
    }

    FREE(list);
}


List *list_create(const size_t size_of, const compare_f cmp_f, const destructor_f destroy_f)
{
    if (size_of == 0)
        ERROR("size_of == 0\n", NULL);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", NULL);

    List *list = malloc(sizeof(List));

    if (list == NULL)
        ERROR("malloc error\n", NULL);

    list->head = NULL;
    list->tail = NULL;

    list->length = 0;
    list->size_of = size_of;

    list->cmp_f = cmp_f;
    list->destroy_f = destroy_f;

    return list;
}


void list_destroy(List *list)
{
    __list_destroy(list, false);
}


void list_destroy_with_entries(List *list)
{
    __list_destroy(list, true);
}


ssize_t list_get_num_entries(const List * const list)
{
    if (list == NULL)   
        ERROR("list == NULL\n", -1);

    return (ssize_t)list->length;
}


ssize_t list_get_data_size(const List * const list)
{
    if (list == NULL)
        ERROR("list == NULL\n", -1);

    return (ssize_t)list->size_of;
}


