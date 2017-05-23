/*
	Hi Leon,

	This is the syntax I am using:

	#pragma userclass(code = LDR)

	and then in the "User classes" box in the LX51 tab:

	CODE_LDR(C:0x0080)

	Regards,
	Ole
*/
extern char xdata aaa;
extern char xdata bbb;
extern char xdata ccc;

char main(void)
{
	aaa = 4;
	bbb = 6;
	ccc = 8;
	return aaa + bbb + ccc;
}
