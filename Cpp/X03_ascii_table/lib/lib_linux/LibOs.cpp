
#include <unistd.h> // unsigned int sleep(unsigned int seconds);   http://man7.org/linux/man-pages/man3/usleep.3.html


void LibOs_SleepSeconds(unsigned int seconds)
{
	sleep(seconds);
}

void LibOs_SleepMiliSeconds(unsigned int miliSeconds)
{

}

