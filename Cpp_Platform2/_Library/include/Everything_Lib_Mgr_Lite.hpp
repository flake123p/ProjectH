

#ifndef _EVERYTHING_LIB_MGR_LITE_HPP_INCLUDED_

//Keep these before every header files
#define ENABLE_LIB_MEM ( 1 )

//#include "stdafx.h" //For porting to "Visual Studio"

// ====== Standard C/Cpp Library ======
#include <stdio.h>  // printf, scanf, NULL
#include <string.h>
#include <stdint.h> // uint32_t, ...
#include <stdlib.h> // malloc, free, rand
#include <stdarg.h>

// ====== Basics ======
#include "My_Basics.hpp"

// ====== Platform Library ======
/*
	Library writing guide:
		1.) Create Demo()
		2.) Create Dump()
*/
#include "_LibLinkedList.hpp"
#include "_LibError.hpp"

#define _EVERYTHING_LIB_MGR_LITE_HPP_INCLUDED_
#endif//_EVERYTHING_LIB_MGR_LITE_HPP_INCLUDED_

