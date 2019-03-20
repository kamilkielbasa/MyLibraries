#include <rbt.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>


/* RBT COLORS */
#define RBT_BLACK  0
#define RBT_RED    1


__extension__ static ___unused___ Rbt_node __sentinel =
{
    .left_son   = (Rbt_node *)&__sentinel,
    .right_son  = (Rbt_node *)&__sentinel,
    .parent     = (Rbt_node *)&__sentinel,
    .color      = RBT_BLACK
};


static Rbt_node * sentinel = &__sentinel;


/*
    Search for node with min key.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %NULL iff failure.
    %Pointer to Rbt_node iff success.
*/
___inline___ static Rbt_node* __rbt_min_node(const Rbt_node *node);


/*
    Search for node with max key.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %NULL iff failure.
    %Pointer to Rbt_node iff success.
*/
___inline___ static Rbt_node* __rbt_max_node(const Rbt_node *node);


/*
    Search node using compare function (key equals data_key)

    PARAMS:
    @IN tree - pointer to RBT.
    @IN data_key - addr of data_key.

    RETURN:
    %NULL if failure.
    %Pointer to found Rbt_node if success.
*/
___inline___ static Rbt_node* __rbt_search_node(const Rbt * __restrict__ const tree, const void * __restrict__ const data_key);


/*
    Get successor of node.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %NULL iff failure.
    %Pointer to Rbt_node iff success.
*/
___inline___ static Rbt_node* __rbt_successor(const Rbt_node *node);


/*
    Get predecessor of node.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %NULL iff failure.
    %Pointer to Rbt_node iff success.
*/
___inline___ static Rbt_node* __rbt_predecessor(const Rbt_node *node);



/*
    Calculate height of the RBT.

    PARAMS:
    @IN tree - pointer to tree.

    RETURN:
    %Height of tree if success.
    %-1 if failure.
*/
static int __rbt_rek_get_height(const Rbt_node * const node);


/*
    Left tree rotate.

    PARAMS:
    @IN tree - pointer to tree.
    @IN node - pointer to tree node.

    RETURN:
    %This is void function.
*/
___inline___ static void __rbt_left_rotate(Rbt *tree, Rbt_node *node);


/*
    Right tree rotate.

    PARAMS:
    @IN tree - pointer to tree.
    @IN node - pointer to tree node.

    RETURN:
    %This is void function.
*/
___inline___ static void __rbt_right_rotate(Rbt *tree, Rbt_node *node);


/*
    Create node.

    PARAMS:
    @IN data - pointer to input data.
    @IN size_of - size_of input data.
    @IN parent - pointer to parent.

    RETURN:
    %NULL if failure.
    %Pointer to Rbt_node iff success.
*/
___inline___ static Rbt_node* __rbt_create_node(const void * __restrict__ const data, const size_t size_of, const Rbt_node * __restrict const parent);


/*
    Destroy node.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %This is void function.
*/
___inline___ static void __rbt_node_destroy(Rbt_node *node);


/*
    Fix property RBT after insert, starting from inserted node.

    PARAMS:
    @IN tree - pointer to tree.
    @IN node - pointer to inserted node.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
static int __rbt_insert_fixup(Rbt * __restrict__ tree, Rbt_node * __restrict__ node);


/*
    Fix property RBT after delete, starting from deleted node.

    PARAMS:
    @IN tree - pointer to tree.
    @IN node - pointer to node.

    RETURN:
    %0 if success.
    %1 if key doesn't exist.
    %-1 if failure.
*/
static int __rbt_delete_fixup(Rbt *tree, Rbt_node *node);


/*
    Destroy whole RBT.

    PARAMS:
    @IN node - pointer to node.
    @IN destroy - call destructor.

    RETURN:
    %This is void function.
*/
static void __rbt_destroy(Rbt *tree, bool destroy);

/*
    Transplant two nodes in RBT.

    PARAMS:
    @IN tree - pointer to RBT.
    @IN ptr1 - pointer to node.
    @IN ptr2 - pointer to node.

    RETURN:
    %This is void function.
*/
___inline___ static void __rbt_transplant(Rbt * tree, Rbt_node *ptr1, Rbt_node *ptr2);

/*
    Delete entry with key equals data_key using compare function.

    PARAMS:
    @IN tree - pointer to RBT.
    @IN data_key - fake data with real key.
    @IN destroy - call destructor.

    RETURN:
    %0 if success.
    %1 if key doesn't exist.
    %-1 if failure.
*/
static int __rbt_delete(Rbt * __restrict__ tree, const void * __restrict__ const data_key, bool destroy);


___inline___ static Rbt_node* __rbt_min_node(const Rbt_node *node)
{
    assert(node != NULL);
    assert(node != sentinel);

    Rbt_node *parent = NULL;

    while (node != sentinel)
    {
        parent = (Rbt_node *)node;
        node = node->left_son;
    }

    return parent;
}


___inline___ static Rbt_node* __rbt_max_node(const Rbt_node *node)
{
    assert(node != NULL);
    assert(node != sentinel);

    Rbt_node *parent = NULL;

    while (node != sentinel)
    {
        parent = (Rbt_node *)node;
        node = node->right_son;
    }

    return parent;
}


___inline___ static Rbt_node *__rbt_search_node(const Rbt * __restrict__ const tree, const void * __restrict__ const data_key)
{
    assert(tree != NULL);
    assert(data_key != NULL);

    Rbt_node *node = tree->root;

    while (node != sentinel)
    {
        if (tree->cmp_f(node->data, data_key) == 0)
            return node;
        if (tree->cmp_f(node->data, data_key) > 0)
            node = node->left_son;
        else 
            node = node->right_son;
    }

    return NULL;
}


___inline___ static Rbt_node* __rbt_successor(const Rbt_node *node)
{
    assert(node != NULL);
    assert(node != sentinel);

    Rbt_node *parent;

    if (node->right_son != sentinel)
        return __rbt_min_node(node->right_son);

    parent = node->parent;

    while (parent != sentinel && node == parent->right_son)
    {
        node = (Rbt_node *)parent;
        parent = parent->parent;
    }

    return parent;
}


___inline___ static Rbt_node* __rbt_predecessor(const Rbt_node *node)
{
    assert(node != NULL);
    assert(node != sentinel);

    Rbt_node *parent;

    if (node->left_son != sentinel)
        return __rbt_max_node(node->left_son);

    parent = node->parent;

    while (parent != sentinel && node == parent->left_son)
    {
        node = (Rbt_node *)parent;
        parent = parent->parent;
    }

    return parent;
}


static int __rbt_rek_get_height(const Rbt_node * const node)
{
    if (node == sentinel)
        return 0;

    int left = __rbt_rek_get_height(node->left_son);
    int right = __rbt_rek_get_height(node->right_son);

    return MAX(left, right) + 1;
}


/*

    parent                           parent
       |                               |
      node                         right_son
    /      \                        /      \
left_son   right_son   ---->     node       Y
            /     \              /   \
           X       Y        left_son  X
*/
___inline___ static void __rbt_left_rotate(Rbt *tree, Rbt_node *node)
{
    assert(tree != NULL);
    assert(node != NULL);
    assert(node != sentinel);
    assert(node->right_son != NULL);
    assert(node->right_son != sentinel);

    Rbt_node *right_son = node->right_son;
    node->right_son = right_son->left_son;

    if (right_son->left_son != sentinel)
        right_son->left_son->parent = node;

    right_son->parent = node->parent;

    if (node->parent == sentinel)
        tree->root = right_son;
    else if (node == node->parent->left_son)
        node->parent->left_son = right_son;
    else
        node->parent->right_son = right_son;

    right_son->left_son = node;
    node->parent = right_son;
}


/*

        parent                        parent
          |                              |
         node                         left_son
       /      \          ---->       /         \
   left_son   right_son             X           node
   /    \                                      /     \
  X      Y                                    Y       right_son

*/
___inline___ static void __rbt_right_rotate(Rbt *tree, Rbt_node *node)
{
    assert(tree != NULL);
    assert(node != NULL);
    assert(node != sentinel);
    assert(node->left_son != NULL);
    assert(node->left_son != sentinel);

    Rbt_node *left_son = node->left_son;
    node->left_son = left_son->right_son;

    if (left_son->right_son != sentinel)
        left_son->right_son->parent = node;

    left_son->parent = node->parent;

    if (node->parent == sentinel)
        tree->root = left_son;
    else if (node == node->parent->right_son)
        node->parent->right_son = left_son;
    else
        node->parent->left_son = left_son;

    left_son->right_son = node;
    node->parent = left_son;
}


___inline___ static Rbt_node* __rbt_create_node(const void * __restrict__ const data, const size_t size_of, const Rbt_node * __restrict__ const parent)
{
    assert(data != NULL);
    assert(size_of >= 1);

    Rbt_node *node = (Rbt_node *)malloc(sizeof(Rbt_node) + size_of);

    if (node == NULL)
        ERROR("malloc error\n", NULL);

    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data, size_of);

    node->parent = (Rbt_node *)parent;
    node->left_son = (Rbt_node *)sentinel;
    node->right_son = (Rbt_node *)sentinel;
    node->size_of = size_of;
    
    /* every single new node has a red color */
    node->color = RBT_RED;

    return node;
}


___inline___ static void __rbt_node_destroy(Rbt_node *node)
{
    if (node == NULL)
        return;

    FREE(node);
}


static int __rbt_insert_fixup(Rbt * __restrict__ tree, Rbt_node * __restrict__ node)
{
    assert(tree != NULL);
    assert(node != NULL);

    Rbt_node *uncle;

    while (node->parent->color == RBT_RED)
    {
        if (node->parent == node->parent->parent->left_son)
        {
            uncle = node->parent->parent->right_son;

            if (uncle->color == RBT_RED)
            {
                node->parent->color = RBT_BLACK;
                uncle->color = RBT_BLACK;
                node->parent->parent->color = RBT_RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right_son)
                {
                    node = node->parent;
                    __rbt_left_rotate(tree, node);
                }

                node->parent->color = RBT_BLACK;
                node->parent->parent->color = RBT_RED;
                __rbt_right_rotate(tree, node->parent->parent);
            }
        }
        else
        {
            uncle = node->parent->parent->left_son;

            if (uncle->color == RBT_RED)
            {
                node->parent->color = RBT_BLACK;
                uncle->color = RBT_BLACK;
                node->parent->parent->color = RBT_RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left_son)
                {
                    node = node->parent;
                    __rbt_right_rotate(tree, node);
                }

                node->parent->color = RBT_BLACK;
                node->parent->parent->color = RBT_RED;
                __rbt_left_rotate(tree, node->parent->parent);
            }
        }
    }

    tree->root->color = RBT_BLACK;
    return 0;
}


static int __rbt_delete_fixup(Rbt *tree, Rbt_node *node)
{
    assert(tree != NULL);
    assert(node != NULL);

    Rbt_node *ptr;

    while (node != tree->root && node->color == RBT_BLACK)
    {
        if (node == node->parent->left_son)
        {
            ptr = node->parent->right_son;

            if (ptr->color == RBT_RED)
            {
                ptr->color = RBT_BLACK;
                node->parent->color = RBT_RED;
                __rbt_left_rotate(tree, node->parent);
                ptr = node->parent->right_son;
            }

            if (ptr->left_son->color == RBT_BLACK && ptr->right_son->color == RBT_BLACK)
            {
                ptr->color = RBT_RED;
                node = node->parent;
            }
            else
            {
                if (ptr->right_son->color == RBT_BLACK)
                {
                    ptr->left_son->color = RBT_BLACK;
                    ptr->color = RBT_RED;
                    __rbt_right_rotate(tree, ptr);
                    ptr = node->parent->right_son;
                }

                ptr->color = node->parent->color;
                node->parent->color = RBT_BLACK;
                ptr->right_son->color = RBT_BLACK;
                __rbt_left_rotate(tree, node->parent);
                node = tree->root;
            }
        }
        else
        {
            ptr = node->parent->left_son;

            if (ptr->color == RBT_RED)
            {
                ptr->color = RBT_BLACK;
                node->parent->color = RBT_RED;
                __rbt_right_rotate(tree, node->parent);
                ptr = node->parent->left_son;
            }

            if (ptr->right_son->color == RBT_BLACK && ptr->left_son->color == RBT_BLACK)
            {
                ptr->color = RBT_RED;
                node = node->parent;
            }
            else
            {
                if (ptr->left_son->color == RBT_BLACK)
                {
                    ptr->right_son->color = RBT_BLACK;
                    ptr->color = RBT_RED;
                    __rbt_left_rotate(tree, ptr);
                    ptr = node->parent->left_son;
                }

                ptr->color = node->parent->color;
                node->parent->color = RBT_BLACK;
                ptr->left_son->color = RBT_BLACK;
                __rbt_right_rotate(tree, node->parent);
                node = tree->root;
            }    
        }
    }

    node->color = RBT_BLACK;

    return 0;
}


static void __rbt_destroy(Rbt *tree, bool destroy)
{
    if (tree == NULL)
        return;

    if (tree->root == NULL || tree->root == sentinel)
    {
        FREE(tree);
        return;
    }

    // Possible stack overflow.
    // It'll be refactored to BFS algorithm with fifo.
    // fifo size 2*log2(height) + 2.
    Rbt_node *array_of_nodes[tree->nodes];

    Rbt_node *node = __rbt_min_node(tree->root);

    for (size_t i = 0; i < tree->nodes; ++i)
    {
        array_of_nodes[i] = node;
        node = __rbt_successor(node);
    }

    for (size_t i = 0; i < tree->nodes; ++i)
    {
        if (destroy == true && tree->destroy_f != NULL)
            tree->destroy_f((void *)array_of_nodes[i]->data);

        FREE(array_of_nodes[i]);
    }

    FREE(tree);
}


___inline___ static void __rbt_transplant(Rbt * tree, Rbt_node *ptr1, Rbt_node *ptr2)
{
    assert(tree != NULL);
    assert(ptr1 != NULL);
    assert(ptr2 != NULL);

    if (ptr1->parent == sentinel)
        tree->root = ptr2;
    else if (ptr1 == ptr1->parent->left_son)
        ptr1->parent->left_son = ptr2;
    else
        ptr1->parent->right_son = ptr2;

    ptr2->parent = ptr1->parent;
}


static int __rbt_delete(Rbt * tree, const void * __restrict__ const data_key, bool destroy)
{
    assert(tree != NULL);
    assert(data_key != NULL);
    assert(tree->root != sentinel);

    Rbt_node *node = __rbt_search_node(tree, data_key);

    if (node == NULL)
        return -1;

    Rbt_node *temp = node;
    char y_original_color = temp->color;
    Rbt_node *ptr;

    if (node->left_son == sentinel)
    {
        ptr = node->right_son;
        __rbt_transplant(tree, node, node->right_son);
    }
    else if (node->right_son == sentinel)
    {
        ptr = node->left_son;
        __rbt_transplant(tree, node, node->left_son);
    }
    else
    {
        temp = __rbt_min_node(node->right_son);
        y_original_color = temp->color;
        ptr = temp->right_son;

        if (temp->parent == node)
            ptr->parent = temp;
        else
        {
            __rbt_transplant(tree, temp, temp->right_son);
            temp->right_son = node->right_son;
            temp->right_son->parent = temp;
        }

        __rbt_transplant(tree, node, temp);
        temp->left_son = node->left_son;
        temp->left_son->parent = temp;
        temp->color = node->color;
    }

    if (y_original_color == RBT_BLACK)
        __rbt_delete_fixup(tree, ptr);

    if (destroy == true && tree->destroy_f != NULL)
        tree->destroy_f((void *)node->data);

    __rbt_node_destroy(node);

    --tree->nodes;

    return 0;
}


Rbt *rbt_create(const size_t size_of, const compare_f cmp_f, const destructor_f destroy_f, const data_print_f print_f)
{
    Rbt *tree;

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    if (cmp_f == NULL)
        ERROR("cmp_f == NULL\n", NULL);

    tree = (Rbt *)malloc(sizeof(Rbt));

    if (tree == NULL)
        ERROR("malloc error\n", NULL);

    tree->root = (Rbt_node *)sentinel;
    tree->size_of = size_of;
    tree->cmp_f = cmp_f;
    tree->destroy_f = destroy_f;
    tree->print_f = print_f;
    tree->nodes = 0;

    return tree;
}


void rbt_destroy(Rbt *tree)
{
    __rbt_destroy(tree, false);
}


void rbt_destroy_with_entries(Rbt *tree)
{
    __rbt_destroy(tree, true);
}


int rbt_insert(Rbt * __restrict__ tree, const void * __restrict__ const data)
{
    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (data == NULL)
        ERROR("data == NULL\n", -1);

    Rbt_node *node;

    /* Special case. Tree is empty */
    if (tree->root == sentinel)
    {
        node = __rbt_create_node(data, tree->size_of, sentinel);

        if (node == NULL)
            ERROR("__rbt_create_node error\n", -1);

        node->color = RBT_BLACK;
        tree->root = node;
    }
    else
    {
        Rbt_node *parent = (Rbt_node *)sentinel;
        node = tree->root;

        /* find correct place, BST search for insert */
        while (node != sentinel)
        {
            parent = node;

            /* data already exists in tree, error code == 1 */
            if (tree->cmp_f(node->data, data) == 0)
                return 1;
            else if (tree->cmp_f(node->data, data) > 0)
                node = node->left_son;
            else
                node = node->right_son;
        }

        Rbt_node *new_node = __rbt_create_node(data, tree->size_of, parent);

        if (new_node == NULL)
            ERROR("__rbt_create_node error\n", -1);

        if (tree->cmp_f(new_node->data, parent->data) > 0)
            parent->right_son = new_node;
        else
            parent->left_son = new_node;

        if (__rbt_insert_fixup(tree, new_node) != 0)
            ERROR("__rbt_insert_fixup(tree, new_node) error\n", -1);
    }

    ++tree->nodes;

    return 0;
}


int rbt_delete(Rbt * __restrict__ tree, const void * __restrict__ const data_key)
{
    return __rbt_delete(tree, data_key, false);
}


int rbt_delete_with_entry(Rbt * __restrict__ tree, const void * __restrict__ const data_key)
{
    return __rbt_delete(tree, data_key, true);
}


int rbt_min(const Rbt * __restrict__ const tree, void * __restrict__ data)
{
    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (data == NULL)
        ERROR("data == NULL\n", -1);

    if (tree->root == sentinel)
        ERROR("tree->root == sentinel\n", -1);

    Rbt_node *node = __rbt_min_node(tree->root);

    if (node == NULL)
        ERROR("__rbt_min_node error\n", -1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);

    return 0;
}


int rbt_max(const Rbt * __restrict__ const tree, void * __restrict__ data)
{
    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (data == NULL)
        ERROR("data == NULL\n", -1);

    if (tree->root == sentinel)
        ERROR("tree->root == sentinel\n", -1);

    Rbt_node *node = __rbt_max_node(tree->root);

    if (node == NULL)
        ERROR("__rbt_max_node error\n", -1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);
    
    return 0;
}


int rbt_search(const Rbt * __restrict__ const tree, const void * const data_key, const void * data_out)
{
    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (tree->root == sentinel)
        ERROR("tree->root == sentinel\n", -1);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", -1);

    if (data_out == NULL)
        ERROR("data_out == NULL\n", -1);

    Rbt_node *node = __rbt_search_node(tree, data_key);

    if (node == NULL)
        return 1;

    __ASSIGN__(*(BYTE *)data_out, *(BYTE *)node->data, tree->size_of);
    return 0;
}


bool rbt_key_exist(const Rbt * __restrict__ const tree, const void * __restrict__ const data_key)
{
    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (tree->root == sentinel)
        ERROR("tree->root == sentinel\n", -1);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", -1);

    return __rbt_search_node(tree, data_key) != NULL;
}


int rbt_to_array(const Rbt * __restrict__ const tree, void * __restrict__ array, size_t * __restrict__ size)
{
    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (size == NULL)
        ERROR("size == NULL\n", -1);

    if (tree->root == sentinel)
        ERROR("tree->root == sentinel\n", -1);

    BYTE *arr;
    Rbt_node *node;
    size_t offset;

    arr = (BYTE *)malloc(tree->size_of * tree->nodes);

    if (arr == NULL)
        ERROR("malloc error\n", -1);

    node = __rbt_min_node(tree->root);

    if (node == NULL)
        ERROR("__rbt_min_node error\n", -1);

    offset = 0;

    while (node != sentinel && node != NULL)
    {
        __ASSIGN__(arr[offset], *(BYTE *)node->data, tree->size_of);
        offset += tree->size_of;

        node = __rbt_successor(node);
    }

    *(void **)array = (void *)arr;
    *size = tree->nodes;

    return 0;
}


ssize_t rbt_get_num_entries(const Rbt * const tree)
{
    if (tree == NULL)
        ERROR("tree == NULL\n", (ssize_t)-1);

    return (ssize_t)tree->nodes;
}


ssize_t rbt_get_data_size(const Rbt * const tree)
{
    if (tree == NULL)
        ERROR("tree == NULL\n", (ssize_t)-1);

    return (ssize_t)tree->size_of;
}


int rbt_get_height(const Rbt * const tree)
{
    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (tree->root == sentinel)
        return 0;

    return __rbt_rek_get_height(tree->root);
}


void rbt_print(const Rbt * const tree)
{
    if (tree == NULL || tree->root == sentinel)
    {
        VERROR("tree == NULL || tree->root == sentinel\n");
        return;
    }

    if (tree->print_f == NULL)
    {
        VERROR("tree->print_f == NULL\n");
        return;
    }

    Rbt_node *node = __rbt_min_node(tree->root);

    for (size_t i = 0; i < tree->nodes; ++i)
    {
        tree->print_f((void *)node->data);
        node = __rbt_successor(node);
    }
}