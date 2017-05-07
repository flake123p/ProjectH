

#ifndef _EVERYTHING_LIB_LINUX_HPP_INCLUDED_



// ====== Standard C/Cpp Library ======
#include <stdio.h>
#include <string.h>
#include <stdint.h> // for uint32_t, ...
#include <stdlib.h> // for exit(), ...

// ====== Standard Linux Library ======
#include <sys/types.h>
#include <sys/ipc.h> // for shared memory, ...
#include <sys/shm.h> // for shared memory, ...
#include <sys/time.h> // for gettimeofday(), ...
#include <unistd.h> // for sleep(), ...
#include <pthread.h>

// ====== Basics ======
#include "My_Basics.hpp"

// ====== Platform Library ======
#include "_LibError.hpp"
//#include "LibUart.hpp"
#include "LibThread.hpp"



#define _EVERYTHING_LIB_LINUX_HPP_INCLUDED_
#endif//_EVERYTHING_LIB_LINUX_HPP_INCLUDED_



