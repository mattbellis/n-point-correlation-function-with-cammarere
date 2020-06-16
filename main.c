#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

	double add = (max - min) / (size-1) ;

	for(int count = 1; count < size; count++)
	{
		returnArr[count] = min + (add * count);
	}

}

void histogram(double sort[], double hist[], double bins[],int binSize, int sortSize)
{
    int upOne;

	for (int out = 0; out < sortSize; out++)
	{
		for (int in = 0; in < binSize-1; in++)
		{
            upOne = in + 1;

			if (sort[out] >= bins[in] && sort[out] < bins[upOne])
			{
				hist[out]++;
			}

		}
	}

}

double distance(double Xi, double Yi, double Zi, double Xj, double Yj, double Zj)
{
    return sqrt((((Xi-Xj),2) + pow((Yi-Yj),2) + pow((Zi-Zj),2)));

}

void DeleteIndex(double arr[],int index)
{
    int size = sizeof(arr)/sizeof(arr[0]);

    double newarr[size-1];

    double val;

    int j = 0;

    for (int i = 0; i < size; i++)
    {
        if (i != index)
        {
           val = arr[i];

           newarr[j] = val;

           j++;
        }

    }
    memcpy(arr,newarr,sizeof(newarr));
}

void ArrAdd(int size, double target[], double add[])
{
    printf("Function: ArrAdd\n\n");

    double newarr[size];
    printf("newarr array created\n");

    for (int i = 0; i < size; i++)
    {
        newarr[i] += target[i] + add[i];
    }

    printf("newarr contains added Array values\n");

    memcpy(target,newarr,sizeof(newarr));

    printf("newarr successfully copied to target\n");
}

void getDistDiff(char fileI[], char fileJ[], int numGalI, int numGalJ, int numBins,double distances[numGalJ * numGalI])
{
    printf("Function: getDistDiff\n\n");

    double I_Xs[numGalI];
    double I_Ys[numGalI];
    double I_Zs[numGalI];
    printf("I Coordinate arrays created\n");

    CoordAssign(numGalI, I_Xs, I_Ys, I_Zs, fileI);

    printf("I Coordinates have been assigned\n");

    double J_Xs[numGalJ];
    double J_Ys[numGalJ];
    double J_Zs[numGalJ];
    printf("J Coordinate arrays created\n");

    CoordAssign(numGalJ, J_Xs, J_Ys, J_Zs, fileJ);

    printf("J Coordinates have been assigned\n");

    double Xi;
    double Yi;
    double Zi;

    double Xj;
    double Yj;
    double Zj;

    int out;
    int in;

    double hist;

    int count = 0;


    for (out = 0; out < I_Xs; out++)
    {
        Xi = I_Xs[out];
        Yi = I_Ys[out];
        Zi = I_Zs[out];

        for (in = 0; in < J_Xs; in++)
        {
            Xj = J_Xs[in];
            Yj = J_Ys[in];
            Zj = J_Zs[in];

            distances[count] = distance(Xi, Yi, Zi, Xj, Yj, Zj);
            count++;

        }

    }


}

void getDistSame(char fileI[], char fileJ[], int numGalI, int numGalJ, int numBins,double distances[numGalJ * numGalI-1])
{
    double I_Xs[numGalI];
    double I_Ys[numGalI];
    double I_Zs[numGalI];

    CoordAssign(numGalI, I_Xs, I_Ys, I_Zs, fileI);

    double J_Xs[numGalJ];
    double J_Ys[numGalJ];
    double J_Zs[numGalJ];

    CoordAssign(numGalJ, J_Xs, J_Ys, J_Zs, fileJ);

    double Xi;
    double Yi;
    double Zi;

    double Xj;
    double Yj;
    double Zj;

    int out;
    int in;

    double hist;

    int count = 0;


    for (out = 0; out < I_Xs; out++)
    {
        Xi = I_Xs[out];
        Yi = I_Ys[out];
        Zi = I_Zs[out];

        for (in = 0; in < J_Xs; in++)
        {
            if (in != out)
            {
            Xj = J_Xs[in];
            Yj = J_Ys[in];
            Zj = J_Zs[in];

            distances[count] = distance(Xi, Yi, Zi, Xj, Yj, Zj);
            count++;
            }
        }

    }


}

void CoordAssign(int numGal, double x[numGal], double y[numGal], double z[numGal], char * fileName[])
{
    FILE * myFile = fopen(fileName, "r+");
    printf("myFile pointer made");

    if(myFile == NULL)
    {
    printf("ERROR: File is null");
    }

    /*myFile = fopen(fileName, "r+");*/
    printf("file is opened");

    for (int i = 0; i < numGal; i++)
    {
        fscanf(myFile, "%d\t", &x[i] );
        fscanf(myFile, "%d\t", &y[i] );
        fscanf(myFile, "%d\n", &z[i] );

    }
    printf("file is seperated into X, Y, Z");
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

    printf("Min = %f\n",minimum);
    printf("Max = %f\n",maximum);

    double checkArr[size+2];
    bins(size, test, minimum, maximum,size,checkArr);

    printf("TEST = %f\n",checkArr[size]);

    double histo[size];

    histogram(test,histo,checkArr,size+2,100);

    for (int i = 0; i < size; i += 10)
    {
        printf("HISTO = %f\n",histo[i]);
    }

    printf("DIST = %f\n",distance(1.0,2.0,3.0,4.0,5.0,6.0));

    DeleteIndex(test,99);

    maximum = max(test,size-1);

    printf("Max = %f\n",maximum);

    double test2[99] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0,
    15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0,
    30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0,
    46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0,
    61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0, 69.0, 70.0, 71.0, 72.0, 73.0, 74.0,
    75.0, 76.0, 77.0, 78.0, 79.0, 80.0, 81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0, 88.0,
    89.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0};

    ArrAdd(99,test2,test);

    printf("Max = %f\n\n",test2[50]);

    double distances[1000*1000];

    FILE * FileI;
    FILE * FileJ;
    getDistDiff('1000gals.dat.txt', '1000galsRAND.dat.txt', 1000, 1000, 100,distances);

    return 0;
}
