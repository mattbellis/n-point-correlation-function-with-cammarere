#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double min(double arr[], int size)
{
	double minimum = arr[0];

	for (int count = 0; count < size; count++)
	{
		if (arr[count] < minimum)
		{
			minimum = arr[count];
		}

	}

	return minimum;
}

double max(double arr[], int size)
{
	double maximum  = arr[0];

	for (int count = 0; count < size; count++)
	{
		if (arr[count] > maximum)
		{
			maximum = arr[count];
		}

	}

	return maximum;
}

void bins(int numBins, double arr[], double min, double max, int size, double returnArr[size])
{
	size += 1;

	returnArr[0] = min;

	double add = (max - min) / size;

	for(int count = 1; count < size; count++)
	{
		returnArr[count] = min + (add * count);
	}

}

void histogram(int size, double hist[size], double arr[],double minimum, double maximum)
{
	double checkArr[size + 1];

	for (int out = 0; out < sizeof(arr); out++)
	{
		for (int in = 0; in < sizeof(size)-1; in++)
		{
			if (arr[out] >= checkArr[in] && arr[out] < checkArr[in+1])
			{
				hist[out]++;
			}

		}
	}

	return hist;
}
int main(int argc, char *argv[])
{
    double test[100] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0,
    15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0,
    30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0,
    46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0,
    61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0, 69.0, 70.0, 71.0, 72.0, 73.0, 74.0,
    75.0, 76.0, 77.0, 78.0, 79.0, 80.0, 81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0, 88.0,
    89.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0, 100.0};

    int size = sizeof(test)/sizeof(test[0]);

    double minimum = min(test,size);
    double maximum = max(test,size);

    printf("Min = %f",minimum);
    printf("Max = %f",maximum);

    double checkArr[size+2];
    bins(size, test, minimum, maximum,size,checkArr);

    printf("TESTSSSS = %f",checkArr[size+1]);
    return 0;
}
