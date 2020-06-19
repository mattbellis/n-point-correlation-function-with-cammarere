#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdlib.h>
#define RAND_MAX 1000

float min(float arr[], int size)
{
	float minimum = arr[0];

	for (int count = 0; count < size; count++)
	{
		if (arr[count] < minimum)
		{
			minimum = arr[count];
		}

	}

	return minimum;
}

float max(float arr[], int size)
{
	float maximum  = arr[0];

	for (int count = 0; count < size; count++)
	{
		if (arr[count] > maximum)
		{
			maximum = arr[count];
		}

	}

	return maximum;
}

void bins(int numBins, float arr[], float min, float max, int size, float returnArr[size])
{
	size += 1;

	returnArr[0] = min;

	float add = (max - min) / (size-1) ;

	for(int count = 1; count < size; count++)
	{
		returnArr[count] = min + (add * count);
	}

}

void histogram2(float sort[], float hist[], float bins[],int binSize, int sortSize)
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

void histogram(float *data, float low, float high, float nbins, float *frequencies)
{
    printf("Function: histogram\n\n");

    float width = high - low;
    printf("Width successfully created.");

    int size = sizeof(data) / sizeof(data[0]);
    printf("Size successfully created.");

    int index;

    for (int i = 0; i < size; i++)
    {
        index = (data[i] - low) / width;
        frequencies[index]++;

    }

    printf("Data successfully histogrammed.");
}

float* distance(float * Xi, float * Yi, float * Zi, float * Xj, float * Yj, float * Zj)
{
    return (float*)malloc(sqrt((((Xi-Xj),2) + pow((Yi-Yj),2) + pow((Zi-Zj),2))));

}

void DeleteIndex(float arr[],int index)
{
    int size = sizeof(arr)/sizeof(arr[0]);

    float newarr[size-1];

    float val;

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

void ArrAdd(int size, float* target[], float* add[])
{
    printf("Function: ArrAdd\n\n");

    printf("newarr array created\n");

    for (int i = 0; i < size; i++)
    {
        *target[i] = *target[i] + *add[i];
    }

    printf("newarr contains added Array values\n");

    printf("newarr successfully copied to target\n");
}

//void CoordAssign1(int numGal, float * x[], float * y[], float * z[], char * fileName[])
//{
//    FILE * myFile = fopen(fileName, "r");
//    printf("myFile pointer made");
//
//    if(myFile == NULL)
//    {
//    printf("ERROR: File is null");
//    }
//
//    /*myFile = fopen(fileName, "r+");*/
//    printf("file is opened");
//
//    for (int i = 0; i < numGal; i++)
//    {
//        fscanf(myFile, "%d\t", &x[i] );
//        fscanf(myFile, "%d\t", &y[i] );
//        fscanf(myFile, "%d\n", &z[i] );
//
//    }
//    printf("file is seperated into X, Y, Z");
//}
//
//void CoordAssign2(char filename[], float x[], float y[], float z[],int numGal)
//{
//    printf("Function: CoordAssign\n\n");
//
//    FILE * data;
//    data = fopen(filename, "r");
//    printf("Data set opened.\n");
//
//    printf("Data set size obtained.\n");
//
//    float numX = 0;
//    float numY = 0;
//    float numZ = 0;
//
//    for (int i = 0; i < numGal; i++)
//    {
//        fscanf(data, "%f\t%f\t%f\n", numX,numY,numZ);
//
//        x[i] = numX;
//        y[i] = numY;
//        z[i] = numZ;
//    }
//    printf("Coodinates were successfully assigned\n\n");
//
//}

//void CoordAssign(char filename[], float x[], float y[], float z[])
//{
//    ifstream data;
//    data.open(filename);
//    int count = 0;
//    while (!data.eof())
//    {
//        cin >> x[count];
//        cin >> y[count];
//        cin >> z[count];
//        count++;
//    }
//
//
//}
void getDist(char fileI[], char fileJ[], int numGalI, int numGalJ, int numBins,float distances[numGalJ * numGalI], int same)
{

    printf("Function: getDistDiff\n\n");

    float * I_Xs[numGalI];
    float * I_Ys[numGalI];
    float * I_Zs[numGalI];
    printf("I Coordinate arrays created\n");

    for (int i = 0; i < numGalI; i++)
    {
        I_Xs[i] = (float*)malloc(((float)rand()/(float)(RAND_MAX))* 1000);
        I_Ys[i] = (float*)malloc(((float)rand()/(float)(RAND_MAX))* 1000);
        I_Zs[i] = (float*)malloc(((float)rand()/(float)(RAND_MAX))* 1000);
    }

    //CoordAssign(fileI, I_Xs, I_Ys, I_Zs);

    printf("I Coordinates have been assigned\n");

    float * J_Xs[numGalJ];
    float * J_Ys[numGalJ];
    float * J_Zs[numGalJ];
    printf("J Coordinate arrays created\n");

    //CoordAssign(fileJ, J_Xs, J_Ys, J_Zs);

    for (int j = 0; j < numGalJ; j++)
    {
        J_Xs[j] = (float*)malloc(((float)rand()/(float)(RAND_MAX))* 1000);
        J_Ys[j] = (float*)malloc(((float)rand()/(float)(RAND_MAX))* 1000);
        J_Zs[j] = (float*)malloc(((float)rand()/(float)(RAND_MAX))* 1000);
    }

    printf("J Coordinates have been assigned\n");

    float * Xi;
    float * Yi;
    float * Zi;

    float * Xj;
    float * Yj;
    float * Zj;

    int out;
    int in;

    float hist[50];
    for (int count = 0; count < 50; count++)
    {
        hist[count] = 0;

    }

//    printf("Size IX: %d\n",sizeof(I_Xs)/sizeof(I_Xs[0]));
//    printf("Size IY: %d\n",sizeof(I_Ys)/sizeof(I_Ys[0]));
//    printf("Size IZ: %d\n",sizeof(I_Zs)/sizeof(I_Zs[0]));
//
//    printf("Size JX: %d\n",sizeof(J_Xs)/sizeof(J_Xs[0]));
//    printf("Size JY: %d\n",sizeof(J_Ys)/sizeof(J_Ys[0]));
//    printf("Size JZ: %d\n",sizeof(J_Zs)/sizeof(J_Zs[0]));
    printf("Lots of variables successfully created \n");
    for (out = 0; out < numGalI; out++)
    {
        float* distances[numGalJ];
        float* histTemp[50];

        Xi = (float*)malloc(I_Xs[out]);
        Yi = (float*)malloc(I_Ys[out]);
        Zi = (float*)malloc(I_Zs[out]);

        printf("Xi, Yi, Zi created\n");

        for (in = 0; in < numGalJ; in++)
        {
            Xj = (float*)malloc(J_Xs[out]);
            Yj = (float*)malloc(J_Ys[out]);
            Zj = (float*)malloc(J_Zs[out]);

            distances[in] = distance(Xi,Yi,Zi,Xj,Yj,Zj);
        }
        printf("XJ, YJ, ZJ created.\nDistances made.\n");
        histogram(distances,0.0,1400.0,50,histTemp);

        ArrAdd(numGalJ,hist,histTemp);
        free(distances);
        free(histTemp);
        free(Xi);
        free(Yi);
        free(Zi);
        free(Xj);
        free(Yi);
        free(Zi);

    }


}

int main(int argc, char *argv[])
{
    float test[100] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0,
    15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0,
    30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0,
    46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0,
    61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0, 69.0, 70.0, 71.0, 72.0, 73.0, 74.0,
    75.0, 76.0, 77.0, 78.0, 79.0, 80.0, 81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0, 88.0,
    89.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0, 100.0};

    int size = sizeof(test)/sizeof(test[0]);

    float minimum = min(test,size);
    float maximum = max(test,size);

    printf("Min = %f\n",minimum);
    printf("Max = %f\n",maximum);

    float checkArr[size+2];
    bins(size, test, minimum, maximum,size,checkArr);

    printf("TEST = %f\n",checkArr[size]);

    float histo[size];

    for (int i = 0; i < size; i += 10)
    {
        printf("HISTO = %f\n",histo[i]);
    }

    //printf("DIST = %f\n",distance(1.0,2.0,3.0,4.0,5.0,6.0));

    DeleteIndex(test,99);

    maximum = max(test,size-1);

    printf("Max = %f\n",maximum);

    float test2[99] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0,
    15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0,
    30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0,
    46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0,
    61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0, 69.0, 70.0, 71.0, 72.0, 73.0, 74.0,
    75.0, 76.0, 77.0, 78.0, 79.0, 80.0, 81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0, 88.0,
    89.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0};

    ArrAdd(99,test2,test);

    printf("Max = %f\n\n",test2[50]);

    float distances[1000*1000];


    //CoordAssign("1000gals.dat.txt",x,y,z);
    getDist("1000gals.dat.txt", "1000galsRAND.dat.txt", 1000, 1000, 100,distances,0);
    return 0;
}
