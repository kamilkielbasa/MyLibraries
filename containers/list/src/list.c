#include <list.h>
#include <common.h>
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy */
#include <stdbool.h> /* bool */
#include <assert.h>


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


/*
    Create new list node.

    PARAMS:
    @IN next - pointer to next node.
    @IN data - pointer to data stored in node.
    @IN size_of - size of data.

    RETURN:
    %Pointer to node if success.
    %NULL if failure.
*/ 
___inline___ static List_node *__list_node_create(List_node * const __restrict__ next, const void * const __restrict__ data, const size_t size_of);


/*
    Delete the first entry which compare(list->entry, entry) == 0

    PARAMS:
    @IN list - pointer to list.
    @IN entry - pointer to entry.
    @IN destroy - call destructor.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
static int __list_delete(List * __restrict__ list, const void * __restrict__ const entry, const bool destroy);

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

        if (list->destroy_f != NULL && destroy == true)
            list->destroy_f((void *)ptr->data);
        
        __list_node_destroy(ptr);

        ptr = next;
    }

    FREE(list);
}


___inline___ static List_node *__list_node_create(List_node * const __restrict__ next, const void * const __restrict__ data, const size_t size_of)
{
    assert(data != NULL);
    assert(size_of >= 1);

    List_node *node = malloc(sizeof(List_node) + size_of);

    if (node == NULL)
        ERROR("malloc error\n", NULL);

    node->next = next;
    node->size_of = size_of;
    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data, size_of);

    return node;
}


static int __list_delete(List * __restrict__ list, const void * __restrict__ const entry, const bool destroy)
{
    if (list == NULL)
        ERROR("list == NULL\n", -1);

    if (entry == NULL)  
        ERROR("entry == NULL\n", -1);

    List_node *ptr = list->head;
    List_node *prev = NULL;
    
    List_node *guard = __list_node_create(NULL, entry, list->size_of);

    if (guard == NULL)
        ERROR("__list_node_create error\n", -1);

    list->tail->next = guard;

    /* skip all entries < in entry */
    while (list->cmp_f(ptr->data, entry) < 0)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    if (list->cmp_f(ptr->data, entry) == 0 && ptr != guard)
    {
        /* we delete head node */
        if (prev == NULL)
        {
            ptr = list->head->next;

            if (list->destroy_f != NULL && destroy == true)
                list->destroy_f((void *)list->head->data);

            __list_node_destroy(list->head);
            list->head = ptr;

            if (list->head == guard)
                list->head = NULL;
        }
        /* we delete in middle or at the end */
        else
        {
            prev->next = ptr->next;

            if (ptr == list->tail)
                list->tail = prev;

            if (list->destroy_f != NULL && destroy == true)
                list->destroy_f((void *)ptr->data);

            __list_node_destroy(ptr);
        }
    }
    /* this entry doesn't exist in list */
    else 
    {
        __list_node_destroy(guard);
        list->tail->next = NULL;
        return 1;
    }

    /* corner case, if we have only one node and we delete it, we cannot do offset on freed node [vargrind error] */
    if (list->head != NULL)
        list->tail->next = NULL;

    __list_node_destroy(guard);
    --list->length;

    if (list->length == 0)
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return 0;
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


int list_insert(List * __restrict__ list, const void * __restrict__ const entry)
{
    if (list == NULL)
        ERROR("list == NULL\n", -1);

    if (entry == NULL)
        ERROR("entry == NULL\n", -1);

    if (list->head == NULL)
    {
        List_node *new_node = __list_node_create(NULL, entry, list->size_of);

        if (new_node == NULL)
            ERROR("__list_node_create error\n", -1);

        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        List_node *ptr = list->head;
        List_node *prev = NULL;

        /* create guard node which allows for doesn't check in first while
           if ptr != NULL */
        List_node *guard = __list_node_create(NULL, entry, list->size_of);
        list->tail->next = guard;

        /* skip all entries < new entry */
        while (list->cmp_f(ptr->data, entry) < 0)
        {
            prev = ptr;
            ptr = ptr->next;
        }

        /* skip all entries == new entry */
        while (list->cmp_f(ptr->data, entry) == 0 && ptr != guard)
        {
            prev = ptr;
            ptr = ptr->next;
        }

        guard->next = ptr;

        if (prev != NULL)
        {
            prev->next = guard;

            if (prev == list->tail)
                list->tail = guard;
        }
        else
            list->head = guard;
        
        list->tail->next = NULL;
    }

    ++list->length;
    return 0;
}


int list_delete(List * __restrict__ list, const void * __restrict__ const entry)
{
    return __list_delete(list, entry, false);
}


int list_delete_with_entry(List * __restrict__ list, const void * __restrict__ const entry)
{
    return __list_delete(list, entry, true);
}


int list_to_array(const List * __restrict__ const list, void * __restrict__ array, size_t * __restrict__ size)
{
    if (list == NULL)
        ERROR("list == NULL\n", -1);

    if (list->length == 0)
        ERROR("list->lenght == 0\n", -1);

    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (size == NULL)   
        ERROR("size == NULL\n", -1);

    BYTE *arr = (BYTE *)malloc(list->length * list->size_of);

    if (arr == NULL)    
        ERROR("malloc error\n", -1);

    List_node *ptr = list->head;
    size_t offset = 0;

    while (ptr != NULL)
    {
        __ASSIGN__(arr[offset], *(BYTE *)ptr->data, list->size_of);

        ptr = ptr->next;
        offset += list->size_of;
    }

    *(void **)array = arr;
    *size = list->length;

    return 0;
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


