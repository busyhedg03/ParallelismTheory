#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <chrono>

void fill_array(float *arr, int size)
{
	double step = 2 * M_PI / size;
#pragma acc data copyin(step)
	{
#pragma acc parallel loop present(arr[:size])
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
#pragma acc exit data copyout(arr[:size])
	std::cout.precision(2);
	for (int i = 0; i < size; i += 1e5)
	{
		std::cout << arr[i] << " ";
	}
	printf("\n");
}

int main()
{
	auto begin_main = std::chrono::steady_clock::now();
	const int size = 1e7;
	float *arr = new float[size];

#pragma acc enter data create(arr[:size])
	auto begin_fill_array = std::chrono::steady_clock::now();
	fill_array(arr, size);
	auto end_fill_array = std::chrono::steady_clock::now();

	auto begin_sum_array = std::chrono::steady_clock::now();
	double sum = sum_array(arr, size);
	auto end_sum_array = std::chrono::steady_clock::now();
	// print_array(arr, size); //to debug the contents of an array
	std::cout.precision(17);
	std::cout << "Sum = " << sum << std::endl;
	delete [] arr;
#pragma acc exit data delete(arr [:size])
	auto end_main = std::chrono::steady_clock::now();
	int time_spent_main = std::chrono::duration_cast<std::chrono::milliseconds>(end_main - begin_main).count();
	int time_spent_fill_array = std::chrono::duration_cast<std::chrono::milliseconds>(end_fill_array - begin_fill_array).count();
	int time_spent_sum_array = std::chrono::duration_cast<std::chrono::microseconds>(end_sum_array - begin_sum_array).count();
	std::cout << "The elapsed time is:\nmain\t\t\t" << time_spent_main << " ms\n" 
		<< "\tfill_array\t" << time_spent_fill_array << " ms\n" 
		<< "\tsum_array\t" << time_spent_sum_array << " us\n";
	return 0;
}