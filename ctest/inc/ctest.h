#ifndef _CTEST_H_
#define _CTEST_H_

/*
    ctest framework implementation

    Author: Kamil Kiełbasa
    email: kamilkielbasa73@gmail.com

    LICENCE: GPL 3.0
*/

#include <stdio.h> /* size_t */
#include <string.h> 
#include <common.h> 


static size_t __failed;
static size_t __failed_instructions;
static size_t __success_instructions;
static size_t __failed_tests;
static size_t __success_tests;


/* PRIVATE MACROS */

/* Console colors */
#define COLOR_DEFAULT "\x1B[0m"
#define COLOR_RED     "\x1B[31m"
#define COLOR_GREEN   "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"

#define __to_string(s) #s
#define to_string(s) __to_string(s)

/* getting only name of the current file */
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
 

/* PUBLIC MACROS */

/*
    Check if expr is true. Otherwise increment flags and print communication.

    Example of usage:

    const char* str = "gnu99";

    T_CHECK(strlen(str) == 5);  // TRUE
    T_CHECK(strlen(str) == 6);  // FALSE
*/
#define T_CHECK(expr) \
    do { \
        if (!(expr)) \
        { \
            ++__failed; \
            ++__failed_instructions; \
            (void)printf(COLOR_RED "CHECK FAILED" COLOR_YELLOW "(%s)\t\t" COLOR_DEFAULT "FILE=%s LINE=%d\n", \
                         to_string(expr), __FILENAME__, __LINE__); \
        } \
        else \
        { \
            ++__success_instructions; \
        } \
    } while (0)


/*
    Expect that value return by func will equal val. Otherwise increment flags and print communcation.
    Macro also check types. In another case will happen compile-time error.

    Example of usage:

    const char* str = "gnu99";

    T_EXPECT(strlen(str), 5);   // PASSED
	T_EXPECT(strlen(str), 6);   // FAILED
*/
#define T_EXPECT(func, val) \
    do { \
        check_types(func, val); \
        typeof(func) __ret = func; \
        typeof(val) __val = val; \
        if (__ret != __val) \
        { \
            ++__failed; \
            ++__failed_instructions; \
            (void)printf(COLOR_RED "EXPECT FAILED" COLOR_YELLOW "(%s != %s)\t\t" COLOR_DEFAULT "FILE=%s LINE=%d\n", \
                         to_string(func), to_string(val), __FILENAME__, __LINE__); \
        } \
        else \
        { \
            ++__success_instructions; \
        } \
    } while (0)


/*
    Macro will check if val equal expt. Otherwise increment flags and print communication.

    Example of usage:

    const char* str = "gnu99";

    T_EXPECT(str, str);    // PASSED
	T_EXPECT(str, NULL);   // FAILED
*/
#define T_ASSERT(val, expt) \
        do { \
        typeof(val) __val = val; \
        typeof(expt) __expt = expt; \
        if (__val != __expt) \
        { \
            ++__failed; \
            ++__failed_instructions; \
            (void)printf(COLOR_RED "ASSERT FAILED" COLOR_YELLOW "(%s != %s)\t\t" COLOR_DEFAULT "FILE=%s LINE=%d\n", \
                         to_string(val), to_string(expt), __FILENAME__, __LINE__); \
        } \
        else \
        { \
            ++__success_instructions; \
        } \
    } while (0)


/*
    Macro will check the expr. If true, failed flags increment, print communication 
    and return from current function. Otherwise nothing happen.

    Example of usage:

    void *ptr = NULL;

    T_EXPECT(ptr == NULL);    // FAILED
*/
#define T_ERROR(expr) \
    do { \
        if (expr) \
        { \
            ++__failed; \
            ++__failed_instructions; \
            (void)printf(COLOR_RED "ERROR FAILED" COLOR_YELLOW "(%s)\t\t" COLOR_DEFAULT "FILE=%s LINE=%d\n", \
                         to_string(expr), __FILENAME__, __LINE__); \
            return; \
        } \
    } while (0) 


/*
    Macro takes function and run it. If happen any failed, increment flags.

    Example of usage:

    static void func(void)
    {
        ...
    }

    TEST(func());
*/
#define TEST(func) \
    do { \
        __failed = 0; \
        func; \
        if (__failed > 0) \
        { \
            ++__failed_tests; \
        } \
        else \
        { \
            ++__success_tests; \
        } \
    } while (0)


/*
    Macro memset flags to zero.
*/
#define TEST_INIT(msg) \
    do { \
        (void)printf(COLOR_YELLOW "\n**************************************" \
                     "**************************************\n" COLOR_DEFAULT); \
        (void)printf(COLOR_BLUE "---------------> %s <---------------\n" COLOR_DEFAULT, msg); \
        __success_tests = __failed_tests = 0; \
    } while (0)


/*
    Macro print proper message after run tests.
*/
#define TEST_SUMMARY() \
    do { \
        (void)printf(COLOR_GREEN "TESTS PASSED = %zu\t\t" COLOR_DEFAULT \
                     COLOR_RED "TESTS FAILED = %zu\n" COLOR_DEFAULT, \
                     __success_tests, __failed_tests); \
        (void)printf(COLOR_GREEN "INSTRUCTIONS PASSED = %zu\t" COLOR_DEFAULT \
                     COLOR_RED "INSTRUCTIONS FAILED = %zu\n" COLOR_DEFAULT, \
                     __success_instructions, __failed_instructions); \
        (void)printf(COLOR_YELLOW "**************************************" \
                                  "**************************************\n" COLOR_DEFAULT); \
        unused(__failed); \
        unused(__failed_instructions); \
        unused(__success_instructions); \
        unused(__failed_tests); \
        unused(__success_tests); \
    } while (0)

#endif /* _CTEST_H_ */
