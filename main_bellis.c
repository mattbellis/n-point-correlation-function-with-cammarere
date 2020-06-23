#include<stdio.h>
#include<stdlib.h>
#include<math.h>


// The user may want to change this 
// This defines the number of bins in the histogram
#define imax 16
#define FLOAT_RAND_MAX (float)RAND_MAX

/////////////////////////////////////////////////////////////
int binning1d(float vals[], int num_vals_to_bin, float lo, float hi, int nbins, float binwidth, unsigned long bin_indices[]) {

    // The user has to make sure that filled_bins has enough memory allocated
    // for nbins integers

    int bin = -1;
    float v;
    for (int i=0;i<num_vals_to_bin;i++) {
        v = vals[i];
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
        bin_indices[i] = bin;
    }

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
    int histogram_chunks = 1024*1024; 

    printf("Filling a histogram with\n");
    printf("Range: %f-%f\n",lo,hi);
    printf("# of bins: %d\n",nbins);
    printf("Bin width: %f\n",binwidth);
    printf("We will histogram in chunks of: %d\n",histogram_chunks);

    // These will be the values we're histogramming
    float *values_to_be_histogrammed = (float*) malloc(nvals * sizeof(float));
    unsigned long *bin_indices = (unsigned long*) malloc(nvals * sizeof(unsigned long));

    int i = 0; 

    static unsigned long hist[imax];

    int idx = 0;

    printf("Allocated the memory for the histogram.\n");

    // Zero out the entries in the histogram
    for (i=0;i<imax;i++) {
        hist[i] = 0;
    }

    printf("Zeroed the memory in the histogram.\n");

    printf("Filling the memory with %lu entries .\n",nvals);
    printf("RAND_MAX: %d\n",RAND_MAX);

    int count_for_histogramming = 0;
    int nvals_in_histogram = 0;

    for(unsigned long count=0;count<nvals;count++)
    {
        if (count%1000000==0){
            printf("count: %d\n",count);
        }

        // Fill the array of values that we will histogram
        values_to_be_histogrammed[count_for_histogramming] = rand()/FLOAT_RAND_MAX;

        // DEBUG PRINT
        //printf("This point: %d %f\n",count,values_to_be_histogrammed[count_for_histogramming]);

        // Keep track of this by hand
        count_for_histogramming++;

        // When we have enough, go histogram them!
        if (count_for_histogramming==histogram_chunks || count==nvals-1) {
            printf("Histogramming a chunk of values!\n");

            binning1d(values_to_be_histogrammed, count_for_histogramming, lo, hi, nbins, binwidth, bin_indices);

            for (int j=0;j<count_for_histogramming;j++) {
                // DEBUG PRINT
                //printf("Summing: %d\n",j);
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
        printf("%d %d\n",i, hist[i]);
        total += hist[i];
    }
    printf("Total entries: %d\n",total);

    return 0;
}
