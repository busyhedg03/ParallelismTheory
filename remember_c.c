#include <stdio.h>
#include <malloc.h>
#include <math.h>

#define PI (3.141592653589793)

double fill_array(double* arr, long long size) {
	double sum = 0;
	double temp = 2 * PI / size;
#pragma acc data create(arr[:size]) copy(sum) copyin(temp)
	{
#pragma acc kernels
		for (long long i = 0; i < size; i++)
		{
			double x = i * temp;
			arr[i] = sin(x);
		}
#pragma acc kernels
		for (long long i = 0; i < size; i++)
		{
			sum += arr[i];
		}
	}
	return sum;
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
	double sum = fill_array(arr, size);
	print_array(arr, size);
	printf("Sum = %f", sum);
	free(arr);
}
