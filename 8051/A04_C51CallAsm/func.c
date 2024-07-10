
unsigned char ASM_FUNC(unsigned char arg0);

unsigned char func(unsigned char arg0)
{
	ASM_FUNC(3);
	
	return arg0 + 3;
}