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


void LibOs_SleepMicroSeconds(unsigned int microSeconds)
{
	/*
		Windows can't sleep less than 1 mini-seconds, this is workaround funcion
	*/
	
	microSeconds = microSeconds / 1000;
	if (microSeconds == 0) {
		microSeconds = 1;
	}
	Sleep(microSeconds);
}

