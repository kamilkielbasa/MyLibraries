#include <rbt.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/* RBT COLORS */
#define RBT_BLACK  0
#define RBT_RED    1

__extension__ static const ___unused___ Rbt_node __sentinel =
{
    .left_son   = (Rbt_node *)&__sentinel,
    .right_son  = (Rbt_node *)&__sentinel,
    .parent     = (Rbt_node *)&__sentinel,
    .color      = RBT_BLACK
};

static const Rbt_node * const sentinel = &__sentinel;

/*
    Search for node with min key.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %NULL iff failure.
    %Pointer to Rbt_node iff success.
*/
__inline__ static Rbt_node* __rbt_min_node(const Rbt_node *node);

/*
    Search for node with max key.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %NULL iff failure.
    %Pointer to Rbt_node iff success.
*/
__inline__ static Rbt_node* __rbt_max_node(const Rbt_node *node);

/*
    Get successor of node.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %NULL iff failure.
    %Pointer to Rbt_node iff success.
*/
__inline__ static Rbt_node* __rbt_successor(const Rbt_node *node);

/*
    Get predecessor of node.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %NULL iff failure.
    %Pointer to Rbt_node iff success.
*/
__inline__ static Rbt_node* __rbt_predecessor(const Rbt_node *node);

/*
    Destroy node.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %This is void function.
*/
__inline__ static void __rbt_node_destroy(Rbt_node *node);

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
    Calculate height of the RBT.

    PARAMS:
    @IN tree - pointer to tree.

    RETURN:
    %Height of tree if success.
    %-1 if failure.
*/
static int __rbt_rek_get_height(const Rbt_node * const node);

__inline__ static Rbt_node* __rbt_min_node(const Rbt_node *node)
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

__inline__ static Rbt_node* __rbt_max_node(const Rbt_node *node)
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

__inline__ static Rbt_node* __rbt_successor(const Rbt_node *node)
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

__inline__ static Rbt_node* __rbt_predecessor(const Rbt_node *node)
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

__inline__ static void __rbt_node_destroy(Rbt_node *node)
{
    if (node == NULL)
        return;

    FREE(node);
}

static void __rbt_destroy(Rbt *tree, bool destroy)
{
    assert(tree != NULL);

    if (tree->root == NULL || tree->root == sentinel)
    {
        FREE(tree);
        return;
    }

    Rbt_node *temp;
    Rbt_node *node;

    /* destroy tree using inorder */
    node = __rbt_min_node(tree->root);

    while (node != sentinel && node != NULL)
    {
        temp = node;
        node = __rbt_successor(node);

        if (destroy == true && tree->destroy_f != NULL)
            tree->destroy_f((void *)temp->data);

        __rbt_node_destroy(node);
    }

    FREE(tree);
}

static int __rbt_rek_get_height(const Rbt_node * const node)
{
    if (node == sentinel)
        return 0;

    int left = __rbt_rek_get_height(node->left_son);
    int right = __rbt_rek_get_height(node->right_son);

    return MAX(left, right) + 1;
}

Rbt* rbt_create(size_t size_of, compare_f cmp_f, destructor_f destroy_f)
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