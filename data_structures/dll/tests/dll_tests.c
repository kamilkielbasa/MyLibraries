#include <dll.h>
#include <ctest.h>
#include <stdint.h> 
#include <assert.h>

typedef struct S
{
	int64_t a;
	int64_t b;
} S;

int main(void)
{
	TEST_INIT("TESTING DOUBLY LINKED LIST");
	TEST_SUMMARY();

	return 0;
}