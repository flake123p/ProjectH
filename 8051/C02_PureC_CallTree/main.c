/*

[[[ PURPOSE ]]]
1.  Observe LinkerScript.lin
2.  DummyMain() replace main() for calling: func1, func2, func3
3.  Create new root for un-called function func4(), Root v.s. Root: """NOT""" OVERLAY EACH-OTHER !!!!!!!!!Very Important!!!!!!!!!
4.  Access function address will add reference to call tree, but it's wrong. e.g. func11() doesn't call func41()
5.1 It's func4 who calls func41()
5.2 Remove call reference between func11 & func41
5.3 Add call reference between func4 & func41
6.  Excluding Segments from Overlay Analysis: func5() BUT STILL HAS WARNING!!! BUT STILL HAS WARNING!!! BUT STILL HAS WARNING!!!

*/
typedef void (*FuncPointer)(void);
FuncPointer funcPtrFunc4x;
void func41();
void func42();

void func11();
void func1() { /* 10 ~ 17 */
	char var_1[8]; 	var_1[7] = 0x1;
	func11();
}
		void func11() { /* 18 ~ 1D */
			char var_11[6]; 	var_11[5] = 0x11;
			funcPtrFunc4x = func41;
		}


void func21();
void func2() { /* 10 ~ 11 */
	char var_2[2]; 	var_2[1] = 0x2;
	func21();
}
		void func21() { /* 12 ~ 1B */
			char var_21[10]; 	var_21[9] = 0x21;
			funcPtrFunc4x = func42;
		}


void func3() { /* 10 ~ 1A */
	char var_3[11]; 	var_3[10] = 0x3;
}
void func4() {
	char var_4[4]; 	var_4[3] = 0x4;
	(*funcPtrFunc4x)();
}
		void func41() {
			char var_41[16]; 	var_41[15] = 0x41;
		}
		void func42() {
			char var_42[32]; 	var_42[31] = 0x42;
		}

void func5() {
	char var_5[16]; 	var_5[15] = 0x5;
}	
void DummyMain() {
	func5();
}
int main()
{
	char var_m[8];
	var_m[7] = 0xFA;
	
	//func1();
	//func2();
	//func3();
	
	return 5;
}
