#pragma SYMBOLS

// App1.c:  Application Extension that is called from Boot_Loader
#include <stdio.h>
//#include "boot.h"
extern void boot_func1 (void);   // function in Boot_Loader called from Application
extern void boot_func2 (void);   // function in Boot_Loader called from Application
extern void boot_func3 (void);   // function in Boot_Loader called from Application
//extern void boot_func4 (void);   // function in Boot_Loader called from Application
extern xdata int v_test;

xdata char c1, c2;
xdata char *p1 = &c1;
xdata char *p2 = &c2;
xdata int v1 _at_ 0x1009;
xdata char aaa = 4;
xdata char bbb = 0xE1;
//extern  bdata   unsigned char system_config_flag;
//extern  bit     system_app_fg;

//extern  bit     system_app_fg;

extern char aazz;
extern bit aazz0;
extern bit aazz1;
extern bit aazz2;
extern char aacc;
extern bit aacc0;
extern bit aacc1;

extern int abxx(void);

//sfr system_config_flag = 0x28;
//sbit    system_app_fg            = 	system_config_flag^0;

void appmain (void)  {
  p1 = &c1;
  p2 = &c2;
  *p1 = *p2;
	v1 = 30;
	//system_app_fg=SYSTEM_APP_BB;
	//system_app_fg=1;
	aazz = 0x5F;
	aazz1 = 0;
	aacc = 0xF0;
	aacc1 = 1;

  boot_func2 ();
  boot_func1 ();
	//boot_func4();
  v_test =10;      
	aaa = 77;
		abxx();
}


int app_func1 (int i1, int i2)  {
	boot_func1();
  return (i1 + i2);
}

int app_func2 (int i3, int i4)  {
  return (i3 + i4);
}
