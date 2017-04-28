typedef enum {
	t1,
	t2,
	t3,
}TEST_t;

unsigned char main(void)
{
	TEST_t test = t3;
	
	unsigned char a;

	a = sizeof(test);
	
	
	return a + test;
}
