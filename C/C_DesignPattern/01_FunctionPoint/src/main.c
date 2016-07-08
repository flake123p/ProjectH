/*
	OS Define
	
	Web Resource:
	http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
*/
#include <stdio.h>

typedef void (*Step1)(void);
typedef void (*Step2)(void);
typedef void (*Step3)(void);
typedef void (*Step4)(void);

struct Drive {
	Step1 one;
	Step2 two;
	Step3 three;
	Step4 four;
};

void CarStep1(void) { printf("into the car\n"); }
void CarStep2(void) { printf("check side mirror\n"); }
void CarStep3(void) { printf("seat belt\n"); }
void CarStep4(void) { printf("drive\n"); }

void PlaneStep1(void) { printf("into the airplane\n"); }
void PlaneStep2(void) { printf("check ticket\n"); }
void PlaneStep3(void) { printf("seat belt\n"); }
void PlaneStep4(void) { printf("fly\n"); }

void DriveAutoMobile(struct Drive drive)
{
	drive.one();
	drive.two();
	drive.three();
	drive.four();
}

void DriveCar(Drive *dev)
{
	dev->one = CarStep1;
	dev->two = CarStep2;
	dev->three = CarStep3;
	dev->four = CarStep4;
}

void DrivePlane(Drive *dev)
{
	dev->one = PlaneStep1;
	dev->two = PlaneStep2;
	dev->three = PlaneStep3;
	dev->four = PlaneStep4;
}

int main(int argc, char *argv[])
{
	struct Drive traffic;
	
	printf("here to drive\n");
	DriveCar(&traffic);
	DriveAutoMobile(traffic);
	
	printf("\n\nhere to fly\n");
	DrivePlane(&traffic);
	DriveAutoMobile(traffic);

	return 0;
}
