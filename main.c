/*
 ============================================================================
 Name        : 2pt-Function.cu
 Author      : Michael Cammarere & Matt Bellis
 Version     :
 Copyright   : 
 Description : CUDA compute reciprocals
 ============================================================================
 */

#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <math.h>
#include<stdio.h>
#define N 1000
#define imax 16

/*This function finds the minimum of an array of floats.
 * It takes the input of the array.
 */

float* ArrMin(float *arr, int size)
{
	//Declaring min & setting it as the 1st value in the array
	float *minVal = &arr[0];

	//For loop to find the minimum
	for (int count = 1; count < size; count++)
	{
		if (&arr[count] < minVal)
		{
			minVal = &arr[count];
		}
	}
	return minVal;
}

/*This function finds the maximum of an array of floats.
 * It takes the input of the array.
 */

float* ArrMax(float *arr, int size)
{
	//Declaring max & setting it as the 1st value in the array
	float* maxVal = &arr[0];

	//For loop to find the minimum
	for (int count = 1; count < size; count++)
	{
		if (&arr[count] > maxVal)
		{
			maxVal = &arr[count];
		}
	}
	return maxVal;
}

/* This function computes the distance between 2 sets of points */
__global__ void
DistCalc (float *distances, float  *xi, float *yi, float *zi, float *xj, float *yj, float *zj, int numElements)
{

	int index = blockDim.x * blockIdx.x + threadIdx.x;

		float x = (xi[index] - xj[index]) * (xi[index] - xj[index]);
		float y = (yi[index] - yj[index]) * (yi[index] - yj[index]);
		float z = (zi[index] - zj[index]) * (zi[index] - zj[index]);

		distances[index] = sqrtf( x + y + z);
}


void Bins(float vals[], int valsInBin, float min, float max, int numBins, float binWidth, unsigned long binIndices[])
{


    // The user has to make sure that filled_bins has enough memory allocated
    // for numBins integers

    int bin = -1;
    float value;

    for (int count = 0; count < valsInBin; count++) {

    	value = vals[count];

        bin = -1;

        if (value < min) {
            bin = -1;
        }
        else if (value > max) {
            bin = -999;
        }
        else {
            bin = (int)((value - min) / binWidth);
        }

        binIndices[count] = bin;
    }

    // bin_indices is a pointer so the values in it will
    // still be accessible outside of the function.

}

int main(int argc, char *argv[])
{
	//min test
	float *testArr = new float[5];
	testArr[0] = 1.0;
	testArr[1] = 2.0;
	testArr[2] = 3.0;
	testArr[3] = 4.0;
	testArr[4] = 5.0;

	float *min = new float[1];
	float *max = new float[1];

	min = ArrMin(testArr,5);
	max = ArrMax(testArr,5);

	printf("MIN = %f\n",*min);
	printf("MAX = %f\n",*max);

	int numElements = 1000;

    printf("[Distance calculation of %d elements]\n", numElements);

	float *xi, *yi, *zi, *xj, *yj, *zj, *distances;
	float *cxi, *cyi, *czi, *cxj, *cyj, *czj, *cdistances;

    // Allocate host memory
	xi = (float*)malloc(sizeof(float) * N);
    yi = (float*)malloc(sizeof(float) * N);
    zi = (float*)malloc(sizeof(float) * N);

	xj = (float*)malloc(sizeof(float) * N);
    yj = (float*)malloc(sizeof(float) * N);
    zj = (float*)malloc(sizeof(float) * N);

    distances = (float*)malloc(sizeof(float) * N);


    for (int i = 0; i < 1000; i++)
    {
    	xi[i] = (float)(rand() % 100 + 1);
    	yi[i] = (float)(rand() % 100 + 1);
    	zi[i] = (float)(rand() % 100 + 1);

    	xj[i] = (float)(rand() % 100 + 1);
    	yj[i] = (float)(rand() % 100 + 1);
    	zj[i] = (float)(rand() % 100 + 1);
    }

    // Allocate device memory
    cudaMalloc((void**)&cxi, N * sizeof(float));
    cudaMalloc((void**)&cyi, N * sizeof(float));
    cudaMalloc((void**)&czi, N * sizeof(float));
    cudaMalloc((void**)&cxj, N * sizeof(float));
    cudaMalloc((void**)&cyj, N * sizeof(float));
    cudaMalloc((void**)&czj, N * sizeof(float));

    cudaMalloc((void**)&cdistances, N * sizeof(float));

    // Transfer data from host to device memory
    cudaMemcpy(cxi,xi, N * sizeof(float),cudaMemcpyHostToDevice);
    cudaMemcpy(cyi,yi, N * sizeof(float),cudaMemcpyHostToDevice);
    cudaMemcpy(czi,zi, N * sizeof(float),cudaMemcpyHostToDevice);
    cudaMemcpy(cxj,xj, N * sizeof(float),cudaMemcpyHostToDevice);
    cudaMemcpy(cyj,yj, N * sizeof(float),cudaMemcpyHostToDevice);
    cudaMemcpy(czj,zj, N * sizeof(float),cudaMemcpyHostToDevice);


    // Executing kernel
    int threadsPerBlock = 256;
    int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;

    DistCalc<<<blocksPerGrid, threadsPerBlock>>>(cdistances, cxi, cyi, czi, cxj, cyj, czj,numElements);

    cudaMemcpy(distances,cdistances, N * sizeof(float),cudaMemcpyDeviceToHost);

	for (int i = 0; i < 1000; i += 100)
	{
		printf("distance = %f\n",distances[i]);
	}

}
