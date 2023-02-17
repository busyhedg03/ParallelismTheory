#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

#define PI (3.141592653589793)


void fill_array(double *arr, long long size)
{
	double step = 2 * PI / size;
#pragma acc data copyin(step)
	{
#pragma acc kernels
		for (long long i = 0; i < size; i++)
		{
			double x = i * step;
			arr[i] = sin(x);
		}
	}
}

double sum_array(double *arr, long long size)
{
	double sum = 0;
#pragma acc data copy(sum)
	{
#pragma acc loop reduction(+ \
						   : sum)
		for (long long i = 0; i < size; i++)
		{
			sum += arr[i];
		}
		return sum;
	}
}

void print_array(double *arr, int size)
{
	for (long long i = 0; i < size; i += 1e5)
	{
		printf("%0.2f ", arr[i]);
	}
	printf("\n");
}

int main()
{
#ifndef __PGI
	clock_t begin = clock();
#endif
#define size 1e7
	double *arr = (double *)malloc(size * sizeof(double));
#pragma acc data create(arr[:(long long)size])
	{
		fill_array(arr, size);
		double sum = sum_array(arr, size);
#ifndef __PGI
		print_array(arr, size);
#endif
		printf("Sum = %0.17f\n", sum); // 9 - float, 17 - double
		free(arr);
	}
#ifndef __PGI
	clock_t end = clock();
	double time_spent = (double)(end - begin) * 1000 / CLOCKS_PER_SEC;
	printf("The elapsed time is %f ms\n", time_spent);
#endif
}
