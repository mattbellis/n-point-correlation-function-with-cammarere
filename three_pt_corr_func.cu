#include<stdio.h>
#include<stdlib.h>
#include<math.h>


// The user may want to change this 
// This defines the number of bins in the histogram
#define imax 128
//#define imax 16
#define FLOAT_RAND_MAX (float)RAND_MAX

/////////////////////////////////////////////////////////////
void binning3d_CPU(float *x0, float *y0, float *z0,  int num_vals_to_bin, float lo, float hi, int nbins, float binwidth, int *bin_indices) {

	int bin = -1;
	float v;

	// The user has to make sure that filled_bins has enough memory allocated
	// for nbins integers

	for (int idx = 0; idx<num_vals_to_bin; idx++ ) {

		v = vals[idx];
		//v = sin(log(pow(1000,v)));
		//v = sin(log(pow(1000,v)));
		//v = sin(log(pow(1000,v)));
		//v = sin(log(pow(1000,v)));
        //v = abs(v);

		// DEBUG PRINT
        //printf("%f\n",v);

		bin = -1;
		if (v<lo) {
			bin=-1;
		}
		else if (v>hi) {
			bin = -999;
		}
		else {
			bin = (int)((v-lo)/binwidth);
		}   
		//bin = idx; // for DEBUGGING
		bin_indices[idx] = bin;
	}

	// bin_indices is a pointer so the values in it will 
	// still be accessible outside of the function.

}

__global__ void binning3d(float *vals, int num_vals_to_bin, float lo, float hi, int nbins, float binwidth, int *bin_indices) {

	int bin = -1;
	float v;

	// The user has to make sure that filled_bins has enough memory allocated
	// for nbins integers

	int idx = blockIdx.x * blockDim.x + threadIdx.x;

	if (idx<num_vals_to_bin) {

		v = vals[idx];
		//v = sin(log(pow(1000,v)));
		//v = sin(log(pow(1000,v)));
		//v = sin(log(pow(1000,v)));
		//v = sin(log(pow(1000,v)));
        //v = abs(v);
		bin = -1;
		if (v<lo) {
			bin=-1;
		}
		else if (v>hi) {
			bin = -999;
		}
		else {
			bin = (int)((v-lo)/binwidth);
		}   
		//bin = idx; // for DEBUGGING
		bin_indices[idx] = bin;
	}

	// bin_indices is a pointer so the values in it will 
	// still be accessible outside of the function.

}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

	// How many random values do we want to process? 
	// This can be set on the command line or you can just
	// edit it and recompile each time
	bool GPU_FLAG = false;
	char* CPU_OR_GPU = argv[1];
	if (strcmp(CPU_OR_GPU,"gpu")==0) {
		GPU_FLAG = true;
	}
	else if (strcmp(CPU_OR_GPU,"cpu")!=0 && strcmp(CPU_OR_GPU,"gpu")!=0) {
		printf("First argument must be 'gpu' or 'cpu'!\n");
		exit(-1);
	}

	unsigned long nvals = atoll(argv[2]);
	printf("nvals: %lu\n",nvals);


	// For the random numbers, they will be between 0 and 1
	float lo = 0;
	float hi = 1;
	int nbins = imax;
	float binwidth = (hi-lo)/nbins;
	// This means we'll send this many values to the function
	// to be histogrammed
	//int histogram_chunks = 16*16; 
	//int histogram_chunks = 256*256; 
	int histogram_chunks = 512*512; 
	//int histogram_chunks = 1024*1024; 

	printf("Filling a histogram with\n");
	printf("Range: %f-%f\n",lo,hi);
	printf("# of bins: %d\n",nbins);
	printf("Bin width: %f\n",binwidth);
	printf("We will histogram in chunks of: %d\n",histogram_chunks);

	// These will be the values we're histogramming
	// I think this is wrong and tries to allocate too much memory
	float *x0 = (float*) malloc(histogram_chunks * sizeof(float));
	float *y0 = (float*) malloc(histogram_chunks * sizeof(float));
	float *z0 = (float*) malloc(histogram_chunks * sizeof(float));
	float *x1 = (float*) malloc(histogram_chunks * sizeof(float));
	float *y1 = (float*) malloc(histogram_chunks * sizeof(float));
	float *z1 = (float*) malloc(histogram_chunks * sizeof(float));

	int *bin_indices = (int*) malloc(histogram_chunks * sizeof(int));

	float *d_x0, *d_y0, *d_z0, *d_x1, *d_y1, *d_z1;
	int *d_bin_indices;
	cudaMalloc ((void**)&d_x0, histogram_chunks * sizeof(float));
	cudaMalloc ((void**)&d_y0, histogram_chunks * sizeof(float));
	cudaMalloc ((void**)&d_z0, histogram_chunks * sizeof(float));
	cudaMalloc ((void**)&d_x1, histogram_chunks * sizeof(float));
	cudaMalloc ((void**)&d_y1, histogram_chunks * sizeof(float));
	cudaMalloc ((void**)&d_z1, histogram_chunks * sizeof(float));
	cudaMalloc ((void**)&d_bin_indices, histogram_chunks * sizeof(int));

	cudaMemset (d_bin_indices, -1, histogram_chunks * sizeof(int));

	int i = 0; 

	static unsigned long hist[imax];

	printf("Allocated the memory for the histogram.\n");

	// Zero out the entries in the histogram
	for (i=0;i<imax;i++) {
		hist[i] = 0;
	}

	printf("Zeroed the memory in the histogram.\n");

	printf("Filling the memory with %lu entries .\n",nvals);
	printf("RAND_MAX: %d\n",RAND_MAX);

	int count_for_histogramming = 0;

	int ichunk = 0;
	for(unsigned long count=0;count<nvals;count++)
	{
		//if (count%10000000==0){
			//printf("count: %lu\n",count);
		//}

		// Fill the array of values that we will histogram
		//if (ichunk==0) 
			x0[count_for_histogramming] = rand()/FLOAT_RAND_MAX;
			y0[count_for_histogramming] = rand()/FLOAT_RAND_MAX;
			z0[count_for_histogramming] = rand()/FLOAT_RAND_MAX;
			x1[count_for_histogramming] = rand()/FLOAT_RAND_MAX;
			y1[count_for_histogramming] = rand()/FLOAT_RAND_MAX;
			z1[count_for_histogramming] = rand()/FLOAT_RAND_MAX;
		// A lot of time is spent generating random numbers.

		// DEBUG PRINT
		//printf("This point: %lu %f\n",count,x0[count_for_histogramming]);

		// Keep track of this by hand
		count_for_histogramming++;

		// When we have enough, go histogram them!
		if (count_for_histogramming==histogram_chunks || count==nvals-1) {
			//printf("Histogramming a chunk of values!\n");
			//printf("histogram_chunks: %d\n",histogram_chunks);
			//printf("count_for_histogramming: %d\n",count_for_histogramming);

			if (GPU_FLAG) {
				// Copy over
				//printf("Using the GPU!\n");
				//if (ichunk==0) {
					cudaMemcpy(d_x0, x0, sizeof(float) * histogram_chunks, cudaMemcpyHostToDevice);
					cudaMemcpy(d_y0, y0, sizeof(float) * histogram_chunks, cudaMemcpyHostToDevice);
					cudaMemcpy(d_z0, z0, sizeof(float) * histogram_chunks, cudaMemcpyHostToDevice);
					cudaMemcpy(d_x1, x1, sizeof(float) * histogram_chunks, cudaMemcpyHostToDevice);
					cudaMemcpy(d_y1, y1, sizeof(float) * histogram_chunks, cudaMemcpyHostToDevice);
					cudaMemcpy(d_z1, z1, sizeof(float) * histogram_chunks, cudaMemcpyHostToDevice);
				//	printf("Copied!\n");
				//}

				binning3d<<<1024,1024>>>(d_x0, count_for_histogramming, lo, hi, nbins, binwidth, d_bin_indices);

				cudaMemcpy(bin_indices, d_bin_indices, sizeof(int) * histogram_chunks, cudaMemcpyDeviceToHost);
			}
			else {
				binning3d_CPU(x0, count_for_histogramming, lo, hi, nbins, binwidth, bin_indices);
			}

			for (int j=0;j<count_for_histogramming;j++) {
				// DEBUG PRINT
				//printf("Summing: %d %d\n",j,bin_indices[j]);
				if (bin_indices[j]>=0 && bin_indices[j]<nbins)
					hist[bin_indices[j]]++;
			}
			// Reset the counter
			count_for_histogramming = 0;
			ichunk++;
		}
	}

	// Print out the histogram
	printf("Printing out the histogram entries\n");
	unsigned long total = 0;
	for (i = 0; i <  imax; i++)  {
        // DEBUG PRINT
		//printf("%d %lu\n",i, hist[i]);
		total += hist[i];
	}
	printf("Total entries: %lu\n",total);

	cudaFree(d_x0);
	cudaFree(d_bin_indices);
	free(x0);
	free(bin_indices);

	return 0;
}
