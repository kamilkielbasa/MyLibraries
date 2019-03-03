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
    Destroy node.

    PARAMS:
    @IN node - pointer to node.

    RETURN:
    %This is void function.
*/
___inline___ static void __rbt_node_destroy(Rbt_node *node);

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
    Left tree rotate.

    PARAMS:
    @IN tree - pointer to tree.
    @IN node - pointer to tree node.

    RETURN:
    %This is void function.
*/
___inline___ static void __rbt_left_rotate(Rbt *tree, Rbt_node *node);

/*
    Create node.

    PARAMS:
    @IN data - pointer to input data.
    @IN size_of - size_of input data.
    @IN parent - pointer to parent.

    RETURN:
    %NULL iff failure.
    %Pointer to Rbt_node iff success.
*/
___inline___ static Rbt_node* __rbt_create_node(const void * __restrict__ const data, const size_t size_of, const Rbt_node * __restrict const parent);

/*
    Fixup RBT starting from inserted node.

    PARAMS:
    @IN tree - pointer to tree.
    @IN node - pointer to inserted node.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
static int __rbt_insert_fixup(Rbt * __restrict__ tree, Rbt_node * __restrict__ node);

/*
    Destroy whole RBT (recursive version).

    PARAMS:
    @IN node - pointer to node.
    @IN destroy - call destructor.

    RETURN:
    %This is void function.
*/
static void __rbt_rek_destroy(Rbt_node *node);

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

___inline___ static void __rbt_node_destroy(Rbt_node *node)
{
    if (node == NULL)
        return;

    FREE(node);
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

        __rbt_node_destroy(temp);
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
    assert(node->right_son != NULL);
    assert(node != sentinel);
    assert(node->right_son != sentinel);

    Rbt_node *right_son = node->right_son;
    node->right_son = right_son->left_son;

    if (node->right_son != sentinel)
        node->right_son->parent = node;

    right_son->left_son = node;
    right_son->parent = node->parent;
    node->parent = right_son;

    if (right_son->parent != sentinel)
    {
        if (right_son->parent->left_son == node)
            right_son->parent->left_son = right_son;
        else
            right_son->parent->right_son = right_son;
    }
    else
        tree->root = right_son;
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
    assert(node->left_son != NULL);
    assert(node != sentinel);
    assert(node->left_son != sentinel);

    Rbt_node *left_son = node->left_son;
    node->left_son = left_son->right_son;

    if (node->left_son != sentinel)
        node->left_son->parent = node;

    left_son->right_son = node;
    left_son->parent = node->parent;
    node->parent = left_son;

    if (left_son->parent != sentinel)
    {
        if (left_son->parent->left_son == node)
            left_son->parent->left_son = left_son;
        else
            left_son->parent->right_son = left_son;
    }
    else
        tree->root = left_son;
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

static int __rbt_insert_fixup(Rbt * __restrict__ tree, Rbt_node * __restrict__ node)
{
    assert(tree != NULL);
    assert(node != NULL);

    Rbt_node *uncle;

    while (node != tree->root && node->parent->color == RBT_RED)
    {
        if (node->parent == node->parent->parent->left_son)
        {
            uncle = node->parent->parent->right_son;

            if (uncle->color == RBT_RED)
            {
                node->parent->color = RBT_BLACK;
                uncle->color = RBT_BLACK;
                node = node->parent->parent;
                node->color = RBT_RED;
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
                node = node->parent->parent;
                node->color = RBT_RED;
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

static void __rbt_rek_destroy(Rbt_node *node)
{
    if (node != sentinel)
    {
        __rbt_rek_destroy(node->left_son);
        __rbt_rek_destroy(node->right_son);
        FREE(node);
    }
}

void rbt_rek_destroy(Rbt *tree)
{
    __rbt_rek_destroy(tree->root);
    FREE(tree);
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