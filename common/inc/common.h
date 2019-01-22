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
        (void)printf("%s.\t\t FILE=%s LINE=%d\n", msg, __FILENAME__, __LINE__); \
        return val; \
    } while (0) \


/* Free and NULL */
#define FREE(ptr) \
    do { \
        free(ptr); \
        ptr = NULL; \
    } while (0) \


/* Get the array size if declared on stack */
#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])


/* Swap a and b if type is the same */
#define SWAP(a, b) \
    do { \
        check_types(a, b); \
        typeof(a) __temp = (a); \
        (a) = (b); \
        (b) = __temp; \
    } while (0);


#endif /* _COMMON_H_ */
