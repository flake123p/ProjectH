
code unsigned char table[] = {0xF1, 0xF2};

unsigned char func(unsigned char arg0)
{
	// MOVC example
	return arg0 + table[0];
}