/*

[[[ PURPOSE ]]]
1. Observe ?STACK been automatically created in test.M51.
2. Unmark "int abc[25]" in main().
3. Observe ?STACK again.
4. Check it on debugger, observe return address when calling func().

*/

void func()
{
}

int main()
{
	//int abc[25];
	
	func(); //Observe the return address which been stored in stack.
	
	return 5;
}
