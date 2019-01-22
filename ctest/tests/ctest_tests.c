#include <ctest.h>
#include <string.h> 
#include <stddef.h> /* size_t */
#include <stdint.h> /* int32_t */


/* manual test for macro T_CHECK */
static void test_macro_t_check(void)
{
	const size_t x = 73;

	T_CHECK(x == x);
	T_CHECK(x != x);
	T_CHECK(x < (x - 1));
	T_CHECK(x < (x - 1));
	T_CHECK(x > (x + 1));
	T_CHECK(x > (x + 1));

	unused(x);


	const char *string = "gnu99";

	T_CHECK(string != NULL);
	T_CHECK(string == NULL);
	T_CHECK(strlen(string) == strlen(string));
	T_CHECK(strlen(string) != strlen(string));
	T_CHECK(strncmp(string, string, strlen(string)) == 0);
	T_CHECK(strncmp(string, string, strlen(string)) != 0);

	unused(string);
}


/* manual test for macro T_EXPECT */
static void test_macro_t_expect(void)
{
	const char *string1 = "gnu99";
	const char *string2 = "ANSI C";

	T_EXPECT(strlen(string1), (size_t)5);
	T_EXPECT(strlen(string1), (size_t)6);
	T_EXPECT(strcmp(string1, string1), 0);
	T_EXPECT(strcmp(string1, string1), -1);
	T_EXPECT(strcmp(string1, string2), 0);
	T_EXPECT(strcmp(string1, string2), -1);

	unused(string1);
	unused(string2);
}


/* manual test for macro T_ASSERT */
static void test_macro_t_assert(void)
{
	void *ptr1 = NULL;

	T_ASSERT(ptr1, NULL);

	int32_t a = 73;
	int32_t b = 37;

	int32_t *ptr2 = &a;
	int32_t *ptr3 = &b; 

	T_ASSERT(ptr2, NULL);
	T_ASSERT(ptr3, NULL);
	T_ASSERT(ptr2, ptr2);
	T_ASSERT(ptr2, ptr3);
	T_ASSERT(*ptr2, *ptr2);
	T_ASSERT(*ptr2, *ptr3);

	unused(a);
	unused(b);
	unused(ptr1);
	unused(ptr2);
	unused(ptr3);
}


/* manual test for macro T_ERROR */
static void test_macro_t_error(void)
{
	const char *string = "gnu99";

	T_ERROR(string == NULL);
	T_ERROR(string != NULL);
}


int main(void)
{
	TEST_INIT("TESTING MANUAL TESTS FOR CTESTS\n");
	TEST(test_macro_t_check());
	TEST(test_macro_t_expect());
	TEST(test_macro_t_assert());
	TEST(test_macro_t_error());
	TEST_SUMMARY();

	return 0;
}
