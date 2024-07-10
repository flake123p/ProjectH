/*
	Purpose: 
		Quick Sort (Recursive Version) (Stupid Version)
*/
#include <stdio.h>

#define ARRAY_SIZE (10)
#define SWAP(a,b,temp) temp=a;a=b;b=temp

int target[ARRAY_SIZE] = {4, 10, 8, 7, 1, 9, 2, 6, 3, 5};

void SimpleQuickSort(int *targetArray, int arraySize)
{
	int midNum = targetArray[0];
	int searchHeadPtr = 1;
	int searchTailPtr = arraySize - 1;
	int temp;
	
	if (searchTailPtr < searchHeadPtr) {
		return;
	}
	
	while (1) {
		// Find number bigger than midNum
		for (; searchHeadPtr < arraySize - 1; searchHeadPtr++) {
			if (targetArray[searchHeadPtr] > midNum)
				break;
		}
		
		// Find number smaller than midNum
		for (; searchTailPtr > 0; searchTailPtr--) {
			if (targetArray[searchTailPtr] < midNum)
				break;
		}
		
		if (searchHeadPtr < searchTailPtr) {
			temp = targetArray[searchHeadPtr];
			targetArray[searchHeadPtr] = targetArray[searchTailPtr];
			targetArray[searchTailPtr] = temp;
		} else {
			break;
		}
	}
	
	if (searchTailPtr > 0) {
		temp = targetArray[0];
		targetArray[0] = targetArray[searchTailPtr];
		targetArray[searchTailPtr] = temp;
	}
	
	// Previous Part
	if (searchTailPtr > 1) {
		SimpleQuickSort(targetArray, searchTailPtr);
	}
	// Post Part
	if (searchTailPtr < arraySize - 2) {
		SimpleQuickSort(&(targetArray[searchTailPtr+1]), arraySize-1-searchTailPtr);
	}
}

void SimpleBubbleSort(int *targetArray, int arraySize)
{
	int temp;
	
	for (int i = 0; i < arraySize - 1; i++) {
		for (int j = 0; j < arraySize - 1 - i; j++) {
			if (targetArray[j] > targetArray[j+1]) {
				SWAP(targetArray[j], targetArray[j+1], temp);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	//
	// print target[] before sort
	//
	printf("[BEFORE]target[] = ");
	for (int i = 0; i < ARRAY_SIZE; i++) {
		printf(" %d", target[i]);
	}
	printf("\n");
	
	//
	// Do Sort
	//
	SimpleQuickSort(target, ARRAY_SIZE);
	
	//
	// print target[] after sort
	//
	printf("[AFTER] target[] = ");
	for (int i = 0; i < ARRAY_SIZE; i++) {
		printf(" %d", target[i]);
	}
	printf("\n");

	return 0;
}
