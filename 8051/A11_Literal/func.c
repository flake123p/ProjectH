code char C_LIT_EOF[] = "EOFAAA";
//const char C_LIT_EOF[] = "EOFAAA";  "const" not equal to "code"!!!
//                                    "const" not equal to "code"!!!
//                                    "const" not equal to "code"!!!
unsigned char func(unsigned char arg0)
{
	return arg0 + C_LIT_EOF[5];
}