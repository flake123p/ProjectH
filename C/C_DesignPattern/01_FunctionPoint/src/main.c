/*
	Purpose: 
		Hook function at beginning, irrelevant to following flow.
*/
#include <stdio.h>

typedef void (*Step1)(void);
typedef void (*Step2)(void);
typedef void (*Step3)(void);

struct Drive {
	Step1 one;
	Step2 two;
	Step3 three;
};

// Car functions for hook
void CarStep1(void) { printf("CarStep1\n"); }
void CarStep2(void) { printf("CarStep2\n"); }
void CarStep3(void) { printf("CarStep3\n"); }

// Car functions for hook
void PlaneStep1(void) { printf("PlaneStep1\n"); }
void PlaneStep2(void) { printf("PlaneStep2\n"); }
void PlaneStep3(void) { printf("PlaneStep3\n"); }

void DriveCar(Drive *dev)
{
	dev->one = CarStep1;
	dev->two = CarStep2;
	dev->three = CarStep3;
}

void DrivePlane(Drive *dev)
{
	dev->one = PlaneStep1;
	dev->two = PlaneStep2;
	dev->three = PlaneStep3;
}

struct Drive traffic;

void Hook(void)
{
	//DriveCar(&traffic);
	DrivePlane(&traffic);
}

int main(int argc, char *argv[])
{
	Hook();

	// Customize flow 3 -> 1 -> 2
	(*traffic.three)();
	(*traffic.one)();
	(*traffic.two)();

	return 0;
}
