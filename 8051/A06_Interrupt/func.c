
#include <INTRINS.H>
#include <REG52.H>

unsigned char gVar = 0;

unsigned char func(void)
{
	return gVar;
}

void isr(void) interrupt 1 using 2
{
  gVar++; //Nothing will be pushed and poped!!!
}

void isr2(void) interrupt 2 using 2
{
  B++; //B will be pushed and poped automatically!!!
}