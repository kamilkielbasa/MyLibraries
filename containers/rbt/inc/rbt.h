#ifndef RBT_H
#define RBT_H

/*
    Generic Red-Black Tree implementation.

    Author: Kamil Kiełbasa
    email: kamilkielbasa73@gmail.com

    LICENCE: GPL 3.0
*/

#include <stddef.h>     /* size_t */
#include <common.h>     /* BYTE */
#include <sys/types.h>  /* ssize_t */
#include <stdbool.h>    /* bool */

typedef BYTE Rbt_color_t;

typedef struct Rbt_node 
{
    struct Rbt_node *parent;        /* pointer to parent    */
    struct Rbt_node *right_son;     /* pointer to right son */
    struct Rbt_node *left_son;      /* pointer to left son  */

    Rbt_color_t color;              /* color of node        */
    size_t size_of;                 /* size of each node    */

    BYTE data[];                    /* placeholder for data */
} Rbt_node;

typedef struct Rbt
{
    Rbt_node *root;                 /* pointer to root      */
    size_t nodes;                   /* number of entries    */
    size_t size_of;                 /* size of each node    */

    compare_f cmp_f;                /* compare function     */
    destructor_f destroy_f;         /* destroy function     */
    data_print_f print_f;           /* print function       */
} Rbt;


/*
    Create RBT.

    PARAMS:
    @IN size_of - size_of data in tree.
    @IN cmp - compare function.
    @IN destroy - your data destructor function.

    RETURN:
    %NULL iff failure.
    %Pointer to RBT iff success.
*/
Rbt *rbt_create(const size_t size_of, const compare_f cmp_f, const destructor_f destroy_f, const data_print_f print_f);


/*
    Destroy all RBT nodes in tree.

    PARAMS:
    @IN tree - pointer to RBT.

    RETURN:
    %This is void function.
*/
void rbt_destroy(Rbt *tree);


/*
    Destroy RBT with all entries (call destructor for each entries).

    PARAMS:
    @IN tree - pointer to RBT.

    RETURN:
    %This is void function.
*/
void rbt_destroy_with_entries(Rbt *tree);


/*
    Insert data to RBT using compare function, if data is not actually in tree.

    PARAMS:
    @IN tree - pointer to RBT.
    @IN data - pointer to entry data.

    RETURN:
    %0 if success.
    %1 if key exists in tree.
    %-1 if failure.
*/
int rbt_insert(Rbt * __restrict__ tree, const void * __restrict__ const data);


/*
    Delete data from RBT using compare function, if data is actually in tree.

    PARAMS:
    @IN tree - pointer to RBT.
    @IN data_key - addr of data with key do delete.

    RETURN:
    %0 if success.
    %1 if key doesn't exists in tree.
    %-1 if failure.
*/
int rbt_delete(Rbt * __restrict__ tree, const void * __restrict__ const data_key);


/*
    Delete data from RBT using compare function, if data is actually in tree.
    (Destructor will be called).

    PARAMS:
    @IN tree - pointer to RBT.
    @IN data_key - addr of data with key do delete.

    RETURN:
    %0 if success.
    %1 if key doesn't exists in tree.
    %-1 if failure.
*/
int rbt_delete_with_entry(Rbt * __restrict__ tree, const void * __restrict__ const data_key);


/*
    Getter of min value using compare function in tree.

    PARAMS:
    @IN tree - pointer to RBT.
    @OUT data - potiner to data.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int rbt_min(const Rbt * __restrict__ const tree, void * __restrict__ data);


/*
    Getter of max value using compare function in tree.

    PARAMS:
    @IN tree - pointer to RBT.
    @OUT data - potiner to data.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int rbt_max(const Rbt * __restrict__ const tree, void * __restrict__ data);


/*
    Search the data in RBT using compare function (data_key equals key in tree).

    PARAMS:
    @IN tree - pointer to RBT.
    @IN data_key - addr of data with search key.
    @out data_out - returned data with addr.

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int rbt_search(const Rbt * __restrict__ const tree, const void * const data_key, const void * data_out);


/*
    Check if key existing in RBT.

    PARAMS:
    @IN tree - pointer to RBT.
    @IN data_key - addr of data with search key.

    RETURN:
    %true if key exist.
    %false if key doesn't exist.
*/
bool rbt_key_exist(const Rbt * __restrict__ const tree, const void * __restrict__ const data_key);


/*
    Convert Red-Black Tree to sorted array.

    PARAMS:
    @IN tree - pointer to tree.
    @OUT array - pointer to array.
    @OUT size - size of array;

    RETURN:
    %0 if success.
    %Negative value if failure.
*/
int rbt_to_array(const Rbt * __restrict__ const tree, void * __restrict__ array, size_t * __restrict__ size);


/*
    Get number of entries.

    PARAMS:
    @IN tree - pointer to tree.

    RETURN:
    %Number of entires if success.
    %-1 if failure.
*/
ssize_t rbt_get_num_entries(const Rbt * const tree);


/*
    Get size of RBT data.

    PARAMS:
    @IN tree - pointer to tree.

    RETURN:
    %Size of data if success.
    %-1 if failure.
*/
ssize_t rbt_get_data_size(const Rbt * const tree);


/*
    Get height of RBT.

    PARAMS:
    @IN tree - pointer to tree.

    RETURN:
    %Height of tree if success.
    %-1 if failure.
*/
int rbt_get_height(const Rbt * const tree);


/*
    Print data from every node in RBT (inorder from min).

    PARAMS:
    @IN tree - pointer to tree.

    RETURN:
    %This is void function.
*/
void rbt_print(const Rbt * const tree);

#endif /* RBT_H */