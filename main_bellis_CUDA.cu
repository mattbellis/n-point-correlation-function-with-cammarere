#include<stdio.h>
#include<stdlib.h>
#include<math.h>


// The user may want to change this 
// This defines the number of bins in the histogram
#define imax 16
#define FLOAT_RAND_MAX (float)RAND_MAX

/////////////////////////////////////////////////////////////
__global__ void binning1d(float *vals, int num_vals_to_bin, float lo, float hi, int nbins, float binwidth, int *bin_indices) {

	int bin = -1;
	float v;

	// The user has to make sure that filled_bins has enough memory allocated
	// for nbins integers

	int idx = blockIdx.x * blockDim.x + threadIdx.x;

	if (idx<num_vals_to_bin) {

		v = vals[idx];
		//v = sin(log(pow(1000,v)));
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
		//printf("value: %f\t",v);
		//printf("bin: %d\n",bin);
		bin = idx; // for DEBUGGING
		bin_indices[idx] = bin;
		//bin_indices[i] = 5;
	}

	bin_indices[4] = 5;

	// bin_indices is a pointer so the values in it will 
	// still be accessible outside of the function.

}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

	// How many random values do we want to process? 
	// This can be set on the command line or you can just
	// edit it and recompile each time
	unsigned long nvals = atoll(argv[1]);
	printf("nvals: %lu\n",nvals);


	// For the random numbers, they will be between 0 and 1
	float lo = 0;
	float hi = 1;
	int nbins = imax;
	float binwidth = (hi-lo)/nbins;
	// This means we'll send this many values to the function
	// to be histogrammed
	int histogram_chunks = 16*16; 

	printf("Filling a histogram with\n");
	printf("Range: %f-%f\n",lo,hi);
	printf("# of bins: %d\n",nbins);
	printf("Bin width: %f\n",binwidth);
	printf("We will histogram in chunks of: %d\n",histogram_chunks);

	// These will be the values we're histogramming
	// I think this is wrong and tries to allocate too much memory
	//float *values_to_be_histogrammed = (float*) malloc(nvals * sizeof(float));
	//unsigned long *bin_indices = (unsigned long*) malloc(nvals * sizeof(unsigned long));
	float *values_to_be_histogrammed = (float*) malloc(histogram_chunks * sizeof(float));
	int *bin_indices = (int*) malloc(histogram_chunks * sizeof(int));

	float *d_values_to_be_histogrammed;
	int *d_bin_indices;
	cudaMalloc ((void**)&d_values_to_be_histogrammed, histogram_chunks * sizeof(float));
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

	for(unsigned long count=0;count<nvals;count++)
	{
		if (count%1000000==0){
			printf("count: %lu\n",count);
		}

		// Fill the array of values that we will histogram
		values_to_be_histogrammed[count_for_histogramming] = rand()/FLOAT_RAND_MAX;

		// DEBUG PRINT
		printf("This point: %lu %f\n",count,values_to_be_histogrammed[count_for_histogramming]);

		// Keep track of this by hand
		count_for_histogramming++;

		// When we have enough, go histogram them!
		if (count_for_histogramming==histogram_chunks || count==nvals-1) {
			printf("Histogramming a chunk of values!\n");

			// Copy over
			cudaMemcpy(d_values_to_be_histogrammed, values_to_be_histogrammed, sizeof(float) * histogram_chunks, cudaMemcpyHostToDevice);


			printf("histogram_chunks: %d\n",histogram_chunks);
			printf("count_for_histogramming: %d\n",count_for_histogramming);
			binning1d<<<16,16>>>(d_values_to_be_histogrammed, count_for_histogramming, lo, hi, nbins, binwidth, d_bin_indices);

			cudaMemcpy(bin_indices, d_bin_indices, sizeof(int) * histogram_chunks, cudaMemcpyDeviceToHost);

			for (int j=0;j<count_for_histogramming;j++) {
				// DEBUG PRINT
				printf("Summing: %d %d\n",j,bin_indices[j]);
				if (bin_indices[j]>=0 && bin_indices[j]<nbins)
					hist[bin_indices[j]]++;
			}
			// Reset the counter
			count_for_histogramming = 0;
		}
	}

	// Print out the histogram
	printf("Printing out the histogram entries\n");
	unsigned long total = 0;
	for (i = 0; i <  imax; i++)  {
		printf("%d %lu\n",i, hist[i]);
		total += hist[i];
	}
	printf("Total entries: %lu\n",total);

	cudaFree(d_values_to_be_histogrammed);
	cudaFree(d_bin_indices);
	free(values_to_be_histogrammed);
	free(bin_indices);

	return 0;
}
