#ifndef RBT_H
#define RBT_H

/*
    Generic Red-Black Tree implementation.

    Author: Kamil Kiełbasa
    email: kamilkielbasa73@gmail.com

    LICENCE: GPL 3.0
*/

#include <stddef.h> /* size_t */
#include <common.h> /* __unused__ */
#include <sys/types.h> /* ssize_t */

typedef char BYTE;
typedef BYTE Rbt_color_t;
typedef int (*compare_f)(const void *, const void *);
typedef void (*destructor_f)(void *);

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
Rbt* rbt_create(size_t size_of, compare_f cmp_f, destructor_f destroy_f);

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
    Destroy all RBT nodes in tree (recursive version).

    PARAMS:
    @IN tree - pointer to RBT.

    RETURN:
    %This is void function.
*/
void rbt_rek_destroy(Rbt *tree);

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

#endif /* RBT_H */