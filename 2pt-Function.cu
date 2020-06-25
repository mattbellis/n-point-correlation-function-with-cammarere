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

#define imax 16
#define FLOAT_RAND_MAX (float)RAND_MAX

static void CheckCudaErrorAux (const char *, unsigned, const char *, cudaError_t);
#define CUDA_CHECK_RETURN(value) CheckCudaErrorAux(__FILE__,__LINE__, #value, value)

/**
 * CUDA kernel that computes reciprocal values for a given vector
 */
__global__ void reciprocalKernel(float *data, unsigned vectorSize) {
	unsigned idx = blockIdx.x*blockDim.x+threadIdx.x;
	if (idx < vectorSize)
		data[idx] = 1.0/data[idx];
}

/**
 * Host function that copies the data and launches the work on GPU
 */
float *gpuReciprocal(float *data, unsigned size)
{
	float *rc = new float[size];
	float *gpuData;

	CUDA_CHECK_RETURN(cudaMalloc((void **)&gpuData, sizeof(float)*size));
	CUDA_CHECK_RETURN(cudaMemcpy(gpuData, data, sizeof(float)*size, cudaMemcpyHostToDevice));
	
	static const int BLOCK_SIZE = 256;
	const int blockCount = (size+BLOCK_SIZE-1)/BLOCK_SIZE;
	reciprocalKernel<<<blockCount, BLOCK_SIZE>>> (gpuData, size);

	CUDA_CHECK_RETURN(cudaMemcpy(rc, gpuData, sizeof(float)*size, cudaMemcpyDeviceToHost));
	CUDA_CHECK_RETURN(cudaFree(gpuData));
	return rc;
}

float *cpuReciprocal(float *data, unsigned size)
{
	float *rc = new float[size];
	for (unsigned cnt = 0; cnt < size; ++cnt) rc[cnt] = 1.0/data[cnt];
	return rc;
}


void initialize(float *data, unsigned size)
{
	for (unsigned i = 0; i < size; ++i)
		data[i] = .5*(i+1);
}

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
__global__
void  distance (float *distances, float  *xi, float *yi, float *zi, float *xj, float *yj, float *zj)
{
	int index = blockDim.x * blockIdx.x + threadIdx.x;

	distances[index] = (float) sqrt( (xi[index] - xj[index]) * (xi[index] - xj[index]) +
				(yi[index] - yj[index]) * (yi[index] - yj[index]) + (zi[index] - zj[index]) * (zi[index] - zj[index]) + 1);
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

	static const int WORK_SIZE = 65530;
	float *data = new float[WORK_SIZE];

	initialize (data, WORK_SIZE);

	float *recCpu = cpuReciprocal(data, WORK_SIZE);
	float *recGpu = gpuReciprocal(data, WORK_SIZE);
	float cpuSum = std::accumulate (recCpu, recCpu+WORK_SIZE, 0.0);
	float gpuSum = std::accumulate (recGpu, recGpu+WORK_SIZE, 0.0);

	/* Verify the results */
	std::cout<<"gpuSum = "<<gpuSum<< " cpuSum = " <<cpuSum<<std::endl;

	/* Free memory */
	delete[] data;
	delete[] recCpu;
	delete[] recGpu;

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


	float *xi = new float[1000];
	float *yi = new float[1000];
	float *zi = new float[1000];
	float *xj = new float[1000];
	float *yj = new float[1000];
	float *zj = new float[1000];

    for (int i = 0; i < 1000; i++)
    {
    	xi[i] = (float)(rand() % 100 + 1);
    	yi[i] = (float)(rand() % 100 + 1);
    	zi[i] = (float)(rand() % 100 + 1);

    	xj[i] = (float)(rand() % 100 + 1);
    	yj[i] = (float)(rand() % 100 + 1);
    	zj[i] = (float)(rand() % 100 + 1);
    }

    cudaMalloc((void**)&xi, 1000 * sizeof(float*));
    cudaMalloc((void**)&yi, 1000 * sizeof(float*));
    cudaMalloc((void**)&zi, 1000 * sizeof(float*));
    cudaMalloc((void**)&xj, 1000 * sizeof(float*));
    cudaMalloc((void**)&yj, 1000 * sizeof(float*));
    cudaMalloc((void**)&zj, 1000 * sizeof(float*));

	float *distances = new float[1000];

	distance<<<1, 1000>>>(distances, xi, yi, zi, xj, yj, zj);

	for (int i = 0; i < 1000; i++)
	{
		printf("distance = %f\n",distances[i]);
	}
	return 0;
}

/**
 * Check the return value of the CUDA runtime API call and exit
 * the application if the call has failed.
 */
static void CheckCudaErrorAux (const char *file, unsigned line, const char *statement, cudaError_t err)
{
	if (err == cudaSuccess)
		return;
	std::cerr << statement<<" returned " << cudaGetErrorString(err) << "("<<err<< ") at "<<file<<":"<<line << std::endl;
	exit (1);
}

