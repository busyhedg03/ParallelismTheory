#include <iostream>
#include <cmath>

// #define _FLOAT

#ifdef _FLOAT
#define T float
#define MAX std::fmaxf
#else
#define T double
#define MAX std::fmax
#endif

void print_array(T **A, int size)
{
#pragma acc data copyout(A[:size][:size])
    std::cout.precision(4);
    for (int i = 0; i < size; i += 1)
    {
        for (int j = 0; j < size; j += 1)
            std::cout << A[i][j] << "\t";
        printf("\n");
    }
    printf("\n");
}

void initialize_array(T **A, int size)
{
#pragma acc data present(A[:size][:size])
    A[0][0] = 10.0;
    A[0][size - 1] = 20.0;
    A[size - 1][size - 1] = 30.0;
    A[size - 1][0] = 20.0;

    T step = 10.0 / (size - 1);
#pragma acc data copyin(step) // not helped
#pragma acc parallel loop present(A[:size][:size])
    for (int i = 1; i < size - 1; i++)
    {
        A[0][i] = A[0][0] + step * i; // horizontal
        A[size - 1][i] = A[size - 1][0] + step * i; // horizontal
        A[i][0] = A[0][0] + step * i; // vertical
        A[i][size - 1] = A[0][size - 1] + step * i; // vertical
    }
}

void delete_2d_array(T **A, int size){
    for (int i = 0; i < size; i++)
        delete[] A[i];
    delete[] A;
}

int main()
{
    // Initialization
    int net_size = 12, iter_max = 1e6;
    T accuracy = 1e-6;
    T **Anew = new T *[net_size],
           **A = new T *[net_size];
    for (int i = 0; i < net_size; i++)
        A[i] = new T[net_size];
    for (int i = 0; i < net_size; i++)
        Anew[i] = new T[net_size];

#pragma acc enter data create(A[:net_size][:net_size], Anew[:net_size][:net_size])

    // 10 20 30 20
    initialize_array(A, net_size);
    initialize_array(Anew, net_size);

    print_array(A, net_size); //check initialization

    T error;
    int iter = 0;
#pragma acc enter data create(error)
    std::cout.precision(4);
    do {
#pragma update device(error)
        error = 0.0;

#pragma acc parallel loop collapse(2) present(A[:net_size][:net_size], Anew[:net_size][:net_size]) reduction(max:error)
        for (int j = 1; j < net_size - 1; j++)
            for (int i = 1; i < net_size - 1; i++)
            {
                // Average
                Anew[j][i] = (A[j][i + 1] + A[j][i - 1] + A[j - 1][i] + A[j + 1][i]) * 0.25;
                error = MAX(error, std::abs(Anew[j][i] - A[j][i]));
            }

        // copy array
#pragma acc parallel loop collapse(2) present(A[:net_size][:net_size], Anew[:net_size][:net_size])
        for (int k = 1; k < net_size-1; k++)
            for (int j = 1; j < net_size-1; j++)
                A[k][j] = Anew[k][j];

#pragma update host(error)
        if (!(iter % 15))
            std::cout << "iter=" << iter << ",\terror=" << error << std::endl;
        iter++;

    } while (error > accuracy && iter < iter_max);
    print_array(A, net_size);
#pragma acc exit data delete(A[:net_size][:net_size], Anew[:net_size][:net_size], error)
    delete_2d_array(A, net_size);
    delete_2d_array(Anew, net_size);
    return 0;
}
