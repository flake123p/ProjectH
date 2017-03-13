#pragma SYMBOLS
#include "boot.h"
extern void app_main (void);
extern int app_func1 (int i1, int i2);
extern int app_func2(int i3, int i4);
extern xdata char c1;
//extern xdata int v1;
//int v1 _at_ 0x08;
//#pragma XCROM
xdata int ar[4];
xdata int v_test=18;
//xdata int v_test;
//bdata   unsigned char system_config_flag;
//sbit    system_app_fg            = 	system_config_flag^1;

//extern bit asdf;
//extern bit asdff;
//extern bit system_app_fg;

extern char aazz;
extern bit aazz0;
extern bit aazz1;
extern bit aazz2;
extern char aacc;
extern bit aacc0;
extern bit aacc1;

int abxx(void){
	return 7;
}

void boot_func4 (void)  {
  xdata int f4_a [5];

  f4_a[0] = 0;  
}

void boot_func3 (void)  {
  xdata int f3_a [5];

  f3_a[0] = 0;  
}

void boot_func1 (void) {
  xdata int f1_a [5];
  
  f1_a[0] = 0;  
	ar[0] = app_func2 (3, 4);
}
//void boot_func3 (void);
void boot_func2 (void)  {
  xdata int f2_a [5];

  f2_a[0] = 0;  
	boot_func3();
}
int boot_func5 (void)  {
	return 5;
}


xdata int v1 _at_ 0x1009;

void main (void)  {

aazz = 0xFF;
aazz0 = 0;
aacc = 0xF0;
aacc0 = 1;
	
   app_main  ();     // call main appliation function (initialize)
	//printf("test :%d\n", v_test);
   boot_func1 ();
   boot_func2 ();
		boot_func5();
	//system_app_fg =SYSTEM_APP_BB;

	//c1=0x12;
	v1=16;
   ar[1] = app_func1 (3, 4);
   ar[2] = app_func2 (5, 6);
	 v_test = ar[1];
	
	//asdf = 1;
	//asdff = 0;
	// boot_func3 ();
   while (1)  {
     ;
   }
}
