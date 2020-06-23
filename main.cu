#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* 2pt Correlation Function */

/*This function finds the minimum of an array of floats.
 * It takes the input of the array and an already declared minimum varable.
 */

float* ArrMin(float *arr)
{
	//Declaring the min variable
	float *min = (float*)malloc(sizeof(float));

	// Finding size of the input array
	int size = sizeof(arr) / sizeof(arr[0]);

	//Declaring the 1st value in the array the temp min
	memcpy(min,&arr[0], sizeof(float*));

	//For loop to find the minimum
	for (int count = 1; count < size; count++)
	{
		if (arr[count] < min)
		{
			min = arr[count];
		}

	}
	return min;
}

/*This function finds the maximum of an array of floats.
 * It takes the input of the array and an already declared minimum varable.
 */

float* ArrMax(float *arr)
{
	//Declaring the max variable
	float *max;

	// Finding size of the input array
	int size = sizeof(arr) / sizeof(arr[0]);

	//Declaring the 1st value in the array the temp max
	max = arr[0];

	//For loop to find the minimum
	for (int count = 1; count < size; count++)
	{
		if (arr[count] > max)
		{
			max = arr[count];
		}

	}
	return max;
}

int main()
{
	float *testArr;
	float copyArr[5] = {1.0,2.0,3.0,4.0,5.0};

	testArr = (float*)malloc(sizeof(float) * 5);

	memcpy((void**)&testArr,copyArr,sizeof(float)*5);

	float *min, *max;

	cudaMemcpy(min,ArrMin(testArr),sizeof(float),cudaMemcpyHostToDevice);
	cudaMemcpy(max,ArrMax(testArr),sizeof(float),cudaMemcpyHostToDevice);

	printf("Max = %f",max);
	printf("Min = %f",min);

}
