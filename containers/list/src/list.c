#include <list.h>
#include <common.h>
#include <stdlib.h>  /* calloc, free */
#include <string.h>  /* memcpy */
#include <stdbool.h> /* bool */
#include <assert.h>


struct List_node
{
    struct List_node* next_p;     /* pointer to next_p node */
    size_t size_of;             /* size of node */

    BYTE data_p[];                /* placeholder for data_p */
};


struct List
{
    List_node* head_p;            /* pointer to head_p */
    List_node* tail_p;            /* pointer to tail_p */

    size_t length;              /* num of nodes in list_p */
    size_t size_of;             /* size of element */

    compare_f cmp_f;            /* compare function */
    destructor_f destroy_f;     /* destructor function */
};


/*
    Destroy list_p node.

    PARAMS:
    @IN list_node_p - pointer to list_p node

    RETURN:
    %This is void function.
*/
static ___inline___ void __list_node_destroy(List_node* list_node_p);


/*
    Destroy whole list_p.

    PARAMS:
    @IN list_p - pointer to list_p.
    @IN destroy - call destructor.

    RETURN:
    %This is void function.
*/
static void __list_destroy(List* list_p, bool destroy);


/*
    Create new list_p node.

    PARAMS:
    @IN next_p - pointer to next_p node.
    @IN data_p - pointer to data_p stored in node.
    @IN size_of - size of data_p.

    RETURN:
    %Pointer to node if success.
    %NULL if failure.
*/ 
static ___inline___ List_node* __list_node_create(List_node* const __restrict__ next_p, 
                                                  const void* const __restrict__ data_p, 
                                                  const size_t size_of);


/*
    Delete the first entry_p which compare(list_p->entry_p, entry_p) == 0

    PARAMS:
    @IN list_p - pointer to list_p.
    @IN entry_p - pointer to entry_p.
    @IN destroy - call destructor.

    RETURN:
    %0 if success.
    %negative value if failure.
*/
static int __list_delete(List* __restrict__ list_p, const void* __restrict__ const entry_p, const bool destroy);


static ___inline___ void __list_node_destroy(List_node* list_node_p)
{
    if (list_node_p == NULL)
        VERROR("list_node_p == NULL\n");

    FREE(list_node_p);
}


static void __list_destroy(List* list_p, bool destroy)
{
    if (list_p == NULL)
        VERROR("list_p == NULL\n");

    List_node* ptr_p = list_p->head_p;
    List_node* next_p = NULL;

    while (ptr_p != NULL)
    {
        next_p = ptr_p->next_p;

        if (list_p->destroy_f != NULL && destroy == true)
            list_p->destroy_f((void*)ptr_p->data_p);
        
        __list_node_destroy(ptr_p);

        ptr_p = next_p;
    }

    FREE(list_p);
}


static ___inline___ List_node* __list_node_create(List_node* const __restrict__ next_p, 
                                                  const void* const __restrict__ data_p, 
                                                  const size_t size_of)
{
    assert(data_p != NULL);
    assert(size_of >= 1);

    List_node* node = (List_node*)calloc(1, sizeof(*node) + size_of);

    if (node == NULL)
        ERROR("calloc error\n", NULL);

    node->next_p = next_p;
    node->size_of = size_of;
    __ASSIGN__(*(BYTE*)node->data_p, *(BYTE*)data_p, size_of);

    return node;
}


static int __list_delete(List* __restrict__ list_p, const void* __restrict__ const entry_p, const bool destroy)
{
    if (list_p == NULL)
        ERROR("list_p == NULL\n", -1);

    if (entry_p == NULL)  
        ERROR("entry_p == NULL\n", -1);

    List_node* ptr_p = list_p->head_p;
    List_node* prev_p = NULL;
    
    List_node* guard_p = __list_node_create(NULL, entry_p, list_p->size_of);

    if (guard_p == NULL)
        ERROR("__list_node_create error\n", -1);

    list_p->tail_p->next_p = guard_p;

    /* skip all entries < in entry_p */
    while (list_p->cmp_f(ptr_p->data_p, entry_p) < 0)
    {
        prev_p = ptr_p;
        ptr_p = ptr_p->next_p;
    }

    if (list_p->cmp_f(ptr_p->data_p, entry_p) == 0 && ptr_p != guard_p)
    {
        /* we delete head_p node */
        if (prev_p == NULL)
        {
            ptr_p = list_p->head_p->next_p;

            if (list_p->destroy_f != NULL && destroy == true)
                list_p->destroy_f((void *)list_p->head_p->data_p);

            __list_node_destroy(list_p->head_p);
            list_p->head_p = ptr_p;

            if (list_p->head_p == guard_p)
                list_p->head_p = NULL;
        }
        /* we delete in middle or at the end */
        else
        {
            prev_p->next_p = ptr_p->next_p;

            if (ptr_p == list_p->tail_p)
                list_p->tail_p = prev_p;

            if (list_p->destroy_f != NULL && destroy == true)
                list_p->destroy_f((void *)ptr_p->data_p);

            __list_node_destroy(ptr_p);
        }
    }
    /* this entry_p doesn't exist in list_p */
    else 
    {
        __list_node_destroy(guard_p);
        list_p->tail_p->next_p = NULL;
        return 1;
    }

    /* corner case, if we have only one node and we delete it, we cannot do offset on freed node [valgrind error] */
    if (list_p->head_p != NULL)
        list_p->tail_p->next_p = NULL;

    __list_node_destroy(guard_p);
    --list_p->length;

    if (list_p->length == 0)
    {
        list_p->head_p = NULL;
        list_p->tail_p = NULL;
    }

    return 0;
}


static int __list_delete_all(List* __restrict__ list_p, const void* __restrict__ const entry_p, const bool destroy)
{
    if (list_p == NULL)
        ERROR("list_p == NULL\n", -1);

    if (entry_p == NULL)  
        ERROR("entry_p == NULL\n", -1);

    if (list_p->head_p == NULL)
        ERROR("list_p->head_p == NULL\n", -1);

    List_node* ptr_p = list_p->head_p;
    List_node* prev_p = NULL;
    
    List_node* guard_p = __list_node_create(NULL, entry_p, list_p->size_of);

    if (guard_p == NULL)
        ERROR("__list_node_create error\n", -1);

    list_p->tail_p->next_p = guard_p;

    /* skip all entries < in entry_p */
    while (list_p->cmp_f(ptr_p->data_p, entry_p) < 0)
    {
        prev_p = ptr_p;
        ptr_p = ptr_p->next_p;
    }

    size_t deleted = 0;

    if (list_p->cmp_f(ptr_p->data_p, entry_p) == 0 && ptr_p != guard_p)
    {
        /* we delete head_p node */
        if (prev_p == NULL)
        {
            while (list_p->cmp_f(ptr_p->data_p, entry_p) == 0 && ptr_p != guard_p)
            {
                ptr_p = list_p->head_p->next_p;

                if (list_p->destroy_f != NULL && destroy == true)
                    list_p->destroy_f((void *)list_p->head_p->data_p);

                __list_node_destroy(list_p->head_p);
                list_p->head_p = ptr_p;
                ++deleted;
            }

            if (list_p->head_p == guard_p)
                list_p->head_p = NULL;
        }
        /* we delete in middle or at the end */
        else
        {
            while (list_p->cmp_f(ptr_p->data_p, entry_p) == 0 && ptr_p != guard_p)
            {
                prev_p->next_p = ptr_p->next_p;

                if (ptr_p == list_p->tail_p)
                    list_p->tail_p = prev_p;

                if (list_p->destroy_f != NULL && destroy == true)
                    list_p->destroy_f((void *)ptr_p->data_p);

                __list_node_destroy(ptr_p);
                ptr_p = prev_p->next_p;
                ++deleted;
            }
        }
    }
    /* this entry_p doesn't exist in list_p */
    else 
    {
        __list_node_destroy(guard_p);
        list_p->tail_p->next_p = NULL;
        return -1;
    }

    __list_node_destroy(guard_p);

    if (list_p->head_p != NULL)
        list_p->tail_p->next_p = NULL;

    list_p->length -= deleted;

    if (list_p->length == 0)
    {
        list_p->head_p = NULL;
        list_p->tail_p = NULL;
    }

    return (int)deleted;
}


List* list_create(const size_t size_of, const compare_f cmp_f, const destructor_f destroy_f)
{
    if (size_of == 0)
        ERROR("size_of == 0\n", NULL);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", NULL);

    List* list_p = (List*)calloc(1, sizeof(*list_p));

    if (list_p == NULL)
        ERROR("calloc error\n", NULL);

    list_p->head_p = NULL;
    list_p->tail_p = NULL;

    list_p->length = 0;
    list_p->size_of = size_of;

    list_p->cmp_f = cmp_f;
    list_p->destroy_f = destroy_f;

    return list_p;
}


void list_destroy(List* list_p)
{
    __list_destroy(list_p, false);
}


void list_destroy_with_entries(List* list_p)
{
    __list_destroy(list_p, true);
}


int list_insert(List* __restrict__ list_p, const void* __restrict__ const entry_p)
{
    if (list_p == NULL)
        ERROR("list_p == NULL\n", -1);

    if (entry_p == NULL)
        ERROR("entry_p == NULL\n", -1);

    if (list_p->head_p == NULL)
    {
        List_node* new_node_p = __list_node_create(NULL, entry_p, list_p->size_of);

        if (new_node_p == NULL)
            ERROR("__list_node_create error\n", -1);

        list_p->head_p = new_node_p;
        list_p->tail_p = new_node_p;
    }
    else
    {
        List_node* ptr_p = list_p->head_p;
        List_node* prev_p = NULL;

        /* create guard_p node which allows for doesn't check in first while
           if ptr_p != NULL */
        List_node* guard_p = __list_node_create(NULL, entry_p, list_p->size_of);
        list_p->tail_p->next_p = guard_p;

        /* skip all entries < new entry_p */
        while (list_p->cmp_f(ptr_p->data_p, entry_p) < 0)
        {
            prev_p = ptr_p;
            ptr_p = ptr_p->next_p;
        }

        /* skip all entries == new entry_p */
        while (list_p->cmp_f(ptr_p->data_p, entry_p) == 0 && ptr_p != guard_p)
        {
            prev_p = ptr_p;
            ptr_p = ptr_p->next_p;
        }

        guard_p->next_p = ptr_p;

        if (prev_p != NULL)
        {
            prev_p->next_p = guard_p;

            if (prev_p == list_p->tail_p)
                list_p->tail_p = guard_p;
        }
        else
            list_p->head_p = guard_p;
        
        list_p->tail_p->next_p = NULL;
    }

    ++list_p->length;
    return 0;
}


int list_delete(List* __restrict__ list_p, const void* __restrict__ const entry_p)
{
    return __list_delete(list_p, entry_p, false);
}


int list_delete_with_entry(List* __restrict__ list_p, const void* __restrict__ const entry_p)
{
    return __list_delete(list_p, entry_p, true);
}


int list_delete_all(List* __restrict__ const list_p, const void* __restrict__ const entry_p)
{
    return __list_delete_all(list_p, entry_p, false);
}


int list_delete_all_with_entry(List* __restrict__ const list_p, const void* __restrict__ const entry_p)
{
    return __list_delete_all(list_p, entry_p, true);
}


int list_search(const List* __restrict__ const list_p, const void* const entry_p, void* val_out)
{
    if (list_p == NULL)
        ERROR("list_p == NULL\n", -1);

    if (entry_p == NULL)
        ERROR("entry_p == NULL\n", -1);

    if (val_out == NULL)
        ERROR("val == NULL\n", -1);

    List_node *ptr_p = list_p->head_p;

    List_node *guard_p = __list_node_create(NULL, entry_p, list_p->size_of);

    if (guard_p == NULL)
        ERROR("__list_node_create error\n", -1);

    list_p->tail_p->next_p = guard_p;

    /* skip all entries < in entry_p */
    while (list_p->cmp_f(ptr_p->data_p, entry_p) < 0)
        ptr_p = ptr_p->next_p;
    
    bool found = false;

    if (list_p->cmp_f(ptr_p->data_p, entry_p) == 0 && ptr_p != guard_p)
    {
        __ASSIGN__(*(BYTE*)val_out, *(BYTE*)ptr_p->data_p, list_p->size_of);
        found = true;
    }

    __list_node_destroy(guard_p);
    list_p->tail_p->next_p = NULL;

    return found == true ? 0 : -1;
}


int list_to_array(const List* __restrict__ const list_p, void* __restrict__ array_p, size_t* __restrict__ size_p)
{
    if (list_p == NULL)
        ERROR("list_p == NULL\n", -1);

    if (list_p->length == 0)
        ERROR("list_p->lenght == 0\n", -1);

    if (array_p == NULL)
        ERROR("array_p == NULL\n", -1);

    if (size_p == NULL)   
        ERROR("size_p == NULL\n", -1);

    BYTE* arr_p = (BYTE*)calloc(list_p->length, list_p->size_of);

    if (arr_p == NULL)    
        ERROR("calloc error\n", -1);

    List_node* ptr_p = list_p->head_p;
    size_t offset = 0;

    while (ptr_p != NULL)
    {
        __ASSIGN__(arr_p[offset], *(BYTE*)ptr_p->data_p, list_p->size_of);

        ptr_p = ptr_p->next_p;
        offset += list_p->size_of;
    }

    *(void**)array_p = arr_p;
    *size_p = list_p->length;

    return 0;
}


ssize_t list_get_num_entries(const List* const list_p)
{
    if (list_p == NULL)   
        ERROR("list_p == NULL\n", -1);

    return (ssize_t)list_p->length;
}


ssize_t list_get_data_size(const List* const list_p)
{
    if (list_p == NULL)
        ERROR("list_p == NULL\n", -1);

    return (ssize_t)list_p->size_of;
}


