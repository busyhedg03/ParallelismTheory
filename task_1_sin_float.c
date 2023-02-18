#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

#define PI (3.141592653589793)

void fill_array(float *arr, int size)
{
	double step = 2 * PI / size;
#pragma acc data copyin(step)
	{
#pragma acc parallel loop present(arr[:(int)size])
		for (int i = 0; i < size; i++)
		{
			double x = i * step;
			arr[i] = sin(x);
		}
	}
}

double sum_array(float *arr, int size)
{
	double sum = 0;
#pragma acc data copy(sum)
	{
#pragma acc parallel loop reduction(+: sum)
		for (int i = 0; i < size; i++)
		{
			sum += arr[i];
		}
	}
	return sum;
}

void print_array(float *arr, int size)
{
#pragma acc exit data copyout(arr[:(int)size])
	for (int i = 0; i < size; i += 1e5)
	{
		printf("%0.2f ", arr[i]);
	}
	printf("\n");
}

int main()
{
	clock_t begin = clock();
#define size 1e7
	float *arr = (float *)malloc(size * sizeof(float));
#pragma acc enter data create(arr[:(int)size])
	{
		fill_array(arr, size);
		double sum = sum_array(arr, size);
		//print_array(arr, size); //для отладки содержимого массива
		printf("Sum = %0.17f\n", sum);
		free(arr);
#pragma acc exit data delete(arr [0:(int)size])
	}
	clock_t end = clock();
	double time_spent = (double)(end - begin) * 1000 / CLOCKS_PER_SEC;
	printf("The elapsed time is %f ms\n", time_spent);
}