//#pragma NOAREGS 

sfr PSW           = 0xD0;

unsigned char macc(unsigned char x, unsigned char y, unsigned char z)
{
	return x*y+z;
}

unsigned char func1(unsigned char x, unsigned char y, unsigned char z)
{
	x++;
	
	return macc(x,y,z);
}

unsigned char abxx(void)
{
	unsigned char a;

	a = macc(1,2,3); //a=5
	
	a = func1(1,2,a);
	
	return a;
}

unsigned char main(void) using 1
{
	unsigned char a;
	
	PSW = 0x08;

	a = abxx(); //correct anwser is 9, abxx() may using 1 to fix this bug.   [ Or use #pragma NOAREGS, it's the better way. ]
	
	return a;
}
