
#include <unistd.h> // unsigned int sleep(unsigned int seconds);   http://man7.org/linux/man-pages/man3/usleep.3.html


void LibOs_SleepSeconds(unsigned int seconds)
{
	sleep(seconds);
}

void LibOs_SleepMiliSeconds(unsigned int miliSeconds)
{
	// int usleep(useconds_t usec);  The type useconds_t is an unsigned integral type capable of storing values at least in the range [0, 1,000,000]
	usleep((useconds_t)(1000 * miliSeconds));
}

void LibOs_SleepMicroSeconds(unsigned int microSeconds)
{
	usleep((useconds_t)microSeconds);
}
