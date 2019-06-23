#ifndef _COMMON_H_
#define _COMMON_H_

/*
    Common macros implementation

    Author: Kamil Kiełbasa
    email: kamilkielbasa73@gmail.com

    LICENCE: GPL 3.0
*/


#include <stdio.h> 
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


/* Basic type for copying memory */
typedef char BYTE;


/* Compare function */
typedef int (*compare_f)(const void *, const void *);


/* Destructor function */
typedef void (*destructor_f)(void *);


/* Print function */
typedef void (*data_print_f)(void *);


/* 
    Check types casting to pointers and cast return value to void.
    Pointers arithmetins of different types doesn't exist.
*/
#define __check_type(t1, t2) ((void)(!((t1 *)0 - (t2 *)0)))


/* 
    Checking types of variables.

    Example of usage:

    int i = 0;
    double d = 3.1454;

    check_types(i, i);  // correct   
    check_types(i, d);  // fatal error
*/
#define check_types(v1, v2) __check_type(typeof(v1), typeof(v2))


/* Static assertion */
#define static_assert(condition) ((void)sizeof(char[1 - 2*!!(condition)]))


/* Unused variable */
#define unused(x) ((void)(x))


/* getting only name of the current file */
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


/* Printing error message with ending current process */
#define ERROR(msg, val) \
    do { \
        (void)fprintf(stderr, "%s.\t\t FILE=%s LINE=%d\n", msg, __FILENAME__, __LINE__); \
        return val; \
    } while (0) \


/* Printing error message with ending current process */
#define VERROR(msg) \
    do { \
        (void)fprintf(stderr, "%s.\t\t FILE=%s LINE=%d\n", msg, __FILENAME__, __LINE__); \
        return; \
    } while (0) \


/* Free and NULL */
#define FREE(ptr) \
    do { \
        free(ptr); \
        (ptr) = NULL; \
    } while (0) \


/* Get the array size if declared on stack */
#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])


/* Assign dst and src if type is the same */
#define __ASSIGN__(dst, src, size) \
    do { \
        _Pragma("GCC diagnostic push"); \
        _Pragma("GCC diagnostic ignored \"-Wstrict-aliasing\""); \
        check_types(dst, src); \
        switch (size) \
        { \
            case 1: *(uint8_t *)&dst = *(uint8_t *)&src; break; \
            case 2: *(uint16_t *)&dst = *(uint16_t *)&src; break; \
            case 4: *(uint32_t *)&dst = *(uint32_t *)&src; break; \
            case 8: *(uint64_t *)&dst = *(uint64_t *)&src; break; \
            default: (void)memcpy((void *)&dst, (void *)&src, (size_t)size); \
        } \
        _Pragma("GCC diagnostic pop"); \
    } while (0);


/* Swap dst and src if type is the same */
#define SWAP(dst, src, size) \
    do { \
        check_types(dst, src); \
        BYTE __buffer__[size]; \
        __ASSIGN__(__buffer__[0], dst, size); \
        __ASSIGN__(dst, src, size); \
        __ASSIGN__(src, __buffer__[0], size); \
    } while (0);


/* use this macro instead of inline */
#define ___inline___ inline __attribute__(( always_inline ))


/* tell compiler the this function or variable could not be used */
#define ___unused___ __attribute__(( unused ))


/* get max if the types are the same */
#define MAX(a, b) \
    __extension__ \
    ({ \
        typeof(a) __a = (a); \
        typeof(b) __b = (b); \
        check_types(__a, __b); \
        __a > __b ? __a : __b; \
    })


/* get min if the types are the same */
#define MIN(a, b) \
    __extension__ \
    ({ \
        typeof(a) __a = (a); \
        typeof(b) __b = (b); \
        check_types(__a, __b); \
        __a < __b ? __a : __b; \
    })


#endif /* _COMMON_H_ */
