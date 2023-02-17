#include <stdio.h>
#include <malloc.h>
#include <math.h>

#define PI (3.141592653589793)

void fill_array(double* arr, long long size) {
#pragma acc kernels
	{
		for (long long i = 0; i < size; i++)
		{
			double x = i * 2 * PI / size;
			arr[i] = sin(x);
		}
	}
}

void print_array(double* arr, int size) {
	for (long long i = 0; i < size; i += 10000)
	{
		printf("%f ", arr[i]);
	}
	printf("\n");
}

int main()
{
#define size 1e7
	double* arr = (double*)malloc(size * sizeof(double));
	fill_array(arr, size);
	print_array(arr, size);
	free(arr);
}
