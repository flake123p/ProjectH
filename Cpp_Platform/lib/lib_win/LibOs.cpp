//#include "stdafx.h" //For porting to "Visual Studio"
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include "LibOs.hpp"
#include "My_Basics.hpp"



void LibOs_SleepSeconds(unsigned int seconds)
{
	Sleep(1000 * seconds);
}

void LibOs_SleepMiliSeconds(unsigned int miliSeconds)
{
	Sleep(miliSeconds);
}
